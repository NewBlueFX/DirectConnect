//	DirectConnectionLibLink.cpp

/*
MIT License

Copyright (c) 2016,2018 NewBlue, Inc. <https://github.com/NewBlueFX>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "DirectConnectionLibLink.h"
#include "DirectConnectionInstanceCreator.h"
#include "DirectConnectionIPCClient.h"
#include "DirectConnectionGeneric.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <locale>
#include <codecvt>


#ifdef _WIN32

#   include <shellapi.h>

#	define LIBLINKBASE_NEW_BLUE_REG_PATH_PREFIX	L"Software\\NewBlue\\"

#	ifdef _WIN64
#		define LIBLINKBASE_INSTALL_PATH_KEY_NAME	L"Install Path 64"
#	else 
#		define LIBLINKBASE_INSTALL_PATH_KEY_NAME	L"Install Path"
#	endif
#elif __APPLE__
// While nothing
#else
#   error Unsupported platform
#endif


namespace DCTL
{

namespace
{

bool CheckServerConnection(Port port)
{
	// The most reliable way to check connection, it is to create the client for given port and call any function.
	bool bConnected = false;
	DirectConnectionIPCClient* pRpcClient = new DirectConnectionIPCClient(port, false, &bConnected);

	if (bConnected)
	{
		// Send any metadata, we do not need if server will process it.
		// We need to know if server is alive.
		bConnected = (pRpcClient->SendGlobalMetadataFrame(-1, "") != DCERR_ServerShutdown);
	}

	pRpcClient->Release();

	return bConnected;
}

}

DirectConnectionLibLink::DirectConnectionLibLink(const wchar_t* pwszHostName, const wchar_t* pwszSkuName, 
	bool bLaunchAppOnDemand, bool bEnableMultipleAppInstances)
	: m_hostName(pwszHostName)
	, m_wideSkuName(pwszSkuName)
	, m_skuName(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> >().to_bytes(pwszSkuName))
	, m_bLaunchAppOnDemand(bLaunchAppOnDemand)
	, m_bEnableMultipleAppInstances(bEnableMultipleAppInstances)
{
	// PID used for generating unique shared_memory name
	const long long llPID = m_bEnableMultipleAppInstances ? DCTL::Utility::GetCurrentProcessId() : -1;
	m_sHostPID = DCTL::Utility::ProcessIdToString(llPID);
}

DirectConnectionLibLink::~DirectConnectionLibLink()
{
}

void DirectConnectionLibLink::Clear()
{
	std::lock_guard<std::recursive_mutex> lock(m_instanceCreatorsLock);

	m_instanceCreators.clear();
}

std::list<PortAndChannels> DirectConnectionLibLink::GetAvailableSources()
{
	// We will check and will launch only one time Standalone per this application.
	static bool standaloneLaunched = false;

	if (!standaloneLaunched)
	{
		static std::recursive_mutex globalCritSec;

		std::lock_guard<std::recursive_mutex> lock(globalCritSec);

		if (!standaloneLaunched)
		{
			if (m_bLaunchAppOnDemand)
			{
				if (!StandaloneApplication::IsLaunched(m_skuName.c_str(), m_sHostPID.data()))
				{
					StandaloneApplication::LaunchStandaloneApplication(m_skuName.c_str(), m_sHostPID.data());
				}

				standaloneLaunched = true;
			}
		}
	}

	auto portAndChannels = ServerPortAndChannels::GetAvailableSources(m_skuName.c_str(), m_sHostPID.data());

	for (auto it = portAndChannels.begin(); it != portAndChannels.end();)
	{
		// Try to connect to each port to be sure that such port is alive.
		// For example, two Standalone-s will be launched and one is crashed,
		// the info for crashed app will be presented in portAndChannels.
		if (CheckServerConnection(it->first))
		{
			// Working port
			++it;
		}
		else
		{
			// Unworking port
			it = portAndChannels.erase(it);
		}
	}

	return portAndChannels;
}

IDirectConnection* DirectConnectionLibLink::CreateInstance(Port port, Channel channel)
{
	std::lock_guard<std::recursive_mutex> lock(m_instanceCreatorsLock);

	for (auto it = m_instanceCreators.begin(); it != m_instanceCreators.end();)
	{
		// Just in case, check that process is running.
		// If it is not so, remove such DirectConnectionInstanceCreator.
		if (!(*it)->IsProcessRunning())
		{
			// Remove this instance
			it = m_instanceCreators.erase(it);
		}
		else
		{
			++it;
		}
	}

	auto it = std::find_if(m_instanceCreators.begin(), m_instanceCreators.end(),
		[&port](const InstanceCreatorPtr& ptr) { return ptr->GetStandaloneClientPort() == port; });

	DirectConnectionInstanceCreator* instanceCreator = nullptr;

	if (it == m_instanceCreators.end())
	{
		bool bInitialized = false;
		// DirectConnectionInstanceCreator was not created for this port, create now.
		auto creator = std::make_shared<DirectConnectionInstanceCreator>(port, &bInitialized);

		NBAssert(bInitialized);	// if result is false, seems DirectConnection output was destroyed between calls -
								// GetAvailableSources() and CreateInstance(). It is normal,
								// but need to check this case anyway, to be sure.

		if (bInitialized)
		{
			m_instanceCreators.push_back(creator);

			instanceCreator = creator.get();
		}
	}
	else
	{
		instanceCreator = (*it).get();
	}

	IDirectConnection* pInstance = nullptr;

	if (instanceCreator)
	{
		DirectConnectionInstance* pDCInstance = instanceCreator->CreateInstance(channel);
		NBAssert(pDCInstance);	// Client is down?
		if (pDCInstance)
		{
			pDCInstance->QueryInterface(IID_DirectConnection, reinterpret_cast<void**>(&pInstance));
			NBAssert(pInstance);	// dev, self-check
			pDCInstance->Release();
		}
	}

	return pInstance;
}

DCResult DirectConnectionLibLink::CloseNTXProcesses()
{
	std::lock_guard<std::recursive_mutex> lock(m_instanceCreatorsLock);

	DCResult res = DCResult::DCERR_Failed;
	const char* pszData = "<newblue_ext command=\"exitApp\" />";

	for (auto it = m_instanceCreators.begin(); it != m_instanceCreators.end(); ++it)
	{
		// Just in case, check that process is running.
		// If it is, we end the ntx process
		if ((*it)->IsProcessRunning())
		{
			// Kill ntx process
			res = (*it)->SendGlobalMetadataFrame(-1, pszData);
		}
	}

	return res;
}

}	// end namespace DCTL
