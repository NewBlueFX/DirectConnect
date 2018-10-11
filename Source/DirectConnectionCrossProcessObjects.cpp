//	DirectConnectionCrossProcessObjects.cpp
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

#include "DirectConnectionCrossProcessObjects.h"
#include <string>
#include <cassert>
#include <locale>
#include <codecvt>
#include <sstream>
#include <algorithm>
#ifdef _WIN32
#include <shellapi.h>
#endif


#ifdef _WIN32
#   define LIBLINKBASE_NEW_BLUE_REG_PATH_PREFIX     L"Software\\NewBlue\\"
    // We use always 64 bit path, because 32 bit Standalone does not exist.
#   define LIBLINKBASE_INSTALL_PATH_KEY_NAME		L"Install Path 64"
#endif

namespace DCTL
{

// namespace only for DirectConnectionCrossProcessObjects classes.
namespace
{
// For StandaloneApplication class.
const char* g_szUniqAppId = "AFF11E8D_95AB_4B8B_97AD_74E4B3330A31";

// For StandaloneApplication class.
const char* g_szUniqServerDataId = "7CFFCE4C_4F5E_4295_A116_0D69C6087354";

// Concatenate several strings to make uniq name
std::string MakeUniqName(const char* pszSkuName, const char* pHostPID, const char* pszId)
{
	std::string sHostPID;
	if (pHostPID)
		sHostPID = std::string(pHostPID);

	std::string sResultName = std::string(pszSkuName) + "_" + pszId;
	if (!sHostPID.empty())
		sResultName += sHostPID;

	return sResultName;
}

// Concatenate several strings to make uniq name for app.
std::string MakeUniqStandaloneApplicationName(const char* pszSkuName, const char* pHostPID)
{
	return MakeUniqName(pszSkuName, pHostPID, g_szUniqAppId);
}

// Concatenate several strings to make uniq name for server data.
std::string MakeUniqServerPortAndChannelsName(const char* pszSkuName, const char* pHostPID)
{
	return MakeUniqName(pszSkuName, pHostPID, g_szUniqServerDataId);
}

#ifdef _WIN32
// We read from 64 bit registry, because 32 bit binaries do not exist.
std::wstring GetFullPathKey64(HKEY hkRootKey, const std::wstring& keyPath, const std::wstring& keyName)
{
	std::wstring content;

	HKEY key = NULL;
	long error = ::RegOpenKeyExW(hkRootKey, keyPath.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &key);

	if (!error)
	{
		DWORD type = REG_SZ;
		DWORD dwSize = 0;

		error = ::RegQueryValueExW(key, keyName.c_str(), NULL, &type, NULL, &dwSize);

		if (error == ERROR_SUCCESS && dwSize > 0)
		{
			BYTE *pBuff = new (std::nothrow) BYTE[dwSize + 2];
			if (pBuff)
			{
				error = ::RegQueryValueExW(key, keyName.c_str(), NULL, &type, pBuff, &dwSize);
				if (!error && dwSize > 1)
				{
					content = std::wstring((wchar_t *)pBuff, dwSize / sizeof(WCHAR) - 1); //to avoid zero terminator
				}

				delete[] pBuff;
				pBuff = NULL;
			}
		}

		::RegCloseKey(key);
	}

	return content;
}

std::wstring FullGetRegistryKey64(const std::wstring& keyPath, const std::wstring& keyName)
{
	std::wstring&& result = GetFullPathKey64(HKEY_CURRENT_USER, keyPath, keyName);

	if (result.empty())
	{
		// For back compatibility try search in HKEY_LOCAL_MACHINE
		result = GetFullPathKey64(HKEY_LOCAL_MACHINE, keyPath, keyName);
	}

	return result;
}

// Return the path from registry from "Install Path 64" key, if it will be found.
// For example this function reads the key "Install Path 64" from - HKEY_CURRENT_USER\SOFTWARE\NewBlue\OvrStream.
// or, HKEY_CURRENT_USER\Software\NewBlue\TitlerLive.
// This function returns the path with last slash always.
std::wstring GetAbsoluteInstallPath(const char* pszSkuName)
{
    // convert UTF8 string to UTF16.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;
    std::wstring wideSkuName = converter.from_bytes(pszSkuName);

	std::wstring&& fullRegKeyPath = LIBLINKBASE_NEW_BLUE_REG_PATH_PREFIX + wideSkuName;
	std::wstring&& path = FullGetRegistryKey64(fullRegKeyPath, LIBLINKBASE_INSTALL_PATH_KEY_NAME);

	if (!path.empty())
	{
		if ((path[path.size() - 1] != L'\\') && (path[path.size() - 1] != L'/'))
		{
			path += L"\\";
		}
	}

	return path;
}

    
std::wstring GetAbsoluteStandaloneFilePath(const char* pszSkuName)
{
    std::wstring resultFilePath;
    // Take installation path where Standalone is located
    std::wstring appFolder = GetAbsoluteInstallPath(pszSkuName);

    if (!appFolder.empty())
    {
		std::wstring exeName;
		if (stricmp(pszSkuName, "OvrStream") == 0)
		{
			exeName = L"OvrStream.exe";
		} 
		else if (stricmp(pszSkuName, "NTX") == 0)
		{
			exeName = L"NTXServer.exe";
		}
		else
		{
			exeName = L"TitlerLiveStandalone.exe";
		}
        // Make full path of executable Standalone
        resultFilePath = appFolder + exeName;
    }

    return resultFilePath;
}

#elif __APPLE__
// Return the Standalone file path
std::string GetAbsoluteStandaloneFilePath(const char* pszSkuName)
{
    std::string resultFilePath;
    // Use these hardcoded paths, because they are not changed.
    if (strcasecmp(pszSkuName, "OvrStream") == 0)
    {
        resultFilePath = "/Library/Application Support/NewBlue/OvrStream/NewBlue OvrStream.app";
    }
    else if (strcasecmp(pszSkuName, "NTX") == 0)
    {
        resultFilePath = "/Library/Application Support/NewBlue/NTX/NewBlue NTXServer.app";
    }
    else
    {
        resultFilePath = "/Library/Application Support/NewBlue/TitlerLive/NewBlue Titler Live.app";
    }

    return resultFilePath;
}
#else
#   error Unsupported platform
#endif

// Convert following string (for example) to std::vector<PortAndChannels>:
// <port_0>;<channel_0>,\n
// <port_1>;<channel_0>,<channel_1>,<channel_2>,\n
// It's look like (in shared memory):
// 156;0,
// 18897;0,1,2,
std::list<PortAndChannels> GetAvailableDataFromString(const char* pszString)
{
	std::list<PortAndChannels> result;

	if (pszString && pszString[0])
	{
		// Parse the string which can look like:
		// 156;0,
		// 18897;0,1,2,
		std::istringstream stringStream(pszString);

		while (!stringStream.eof())
		{
			int port = 0;
			std::vector<int> channels;

			// Firstly parse a port value
			stringStream >> port;
			stringStream.ignore();	// skip ';'

			// Now parse channels values
			while (!stringStream.eof() && stringStream.peek() != '\n')
			{
				int channel = 0;
				stringStream >> channel;
				stringStream.ignore();	// skip ','
				channels.push_back(channel);
			}

			stringStream.ignore();	// skip '\n'

			if (port != 0 && !channels.empty())
			{
				result.push_back(std::make_pair(port, channels));
			}
		}
	}

	return result;
}

std::string ConvertPortAndChannelsToString(const std::list<PortAndChannels>& data)
{
	// We will save ports and channels in this format:
	// <port_0>;<channel_0>,\n
	// <port_1>;<channel_0>,<channel_1>,<channel_2>,\n
	std::stringstream strStream;

	for (const auto& portData : data)
	{
		strStream << portData.first << ';';

		for (const auto& channel : portData.second)
		{
			strStream << channel << ',';
		}

		strStream << '\n';
	}

	return strStream.str();
}

}


// StandaloneApplication class implementation 
StandaloneApplication::StandaloneApplication(const char* pszSkuName, const char* pHostPID, bool* pbResult)
{
	// Make uniq name for global object which will identify an app
	const std::string& uniqName = MakeUniqStandaloneApplicationName(pszSkuName, pHostPID);

	// Try to initialize global system object (create shared memory).
	const bool res = m_systemObject.InitSharedMemory(uniqName, true, 1);
    NBAssert(res);  // System object was not created? Check it!
	if (pbResult)
	{
		*pbResult = res;
	}
}

StandaloneApplication::~StandaloneApplication()
{
	m_systemObject.FreeSharedMemory();
}

bool StandaloneApplication::IsLaunched(const char* pszSkuName, const char* pHostPID)
{
    NBAssert(pszSkuName);
    CheckPointer(pszSkuName, false);

	// Make uniq name for global object which will identify an app
	const std::string& uniqName = MakeUniqStandaloneApplicationName(pszSkuName, pHostPID);

	// Try to initialize global system object, we will try only to open shared memory (not create),
	// because we need to know if app exists.
	return CSharedMemory().InitSharedMemory(uniqName, false, 1);
}

bool StandaloneApplication::LaunchStandaloneApplication(const char* pszSkuName, const char* pHostPID)
{
    NBAssert(pszSkuName);
    CheckPointer(pszSkuName, false);
	// LaunchStandaloneApplication should be called when app is not launched.
	NBAssert(!StandaloneApplication::IsLaunched(pszSkuName, pHostPID));

	bool result = false;

#ifdef _WIN32
    // Make full path of executable Standalone
    const std::wstring& appFilePath = GetAbsoluteStandaloneFilePath(pszSkuName);
	NBAssert(!appFilePath.empty());	// EXE file not found!
    if (!appFilePath.empty())
    {
        const bool bSkuNTX = (stricmp(pszSkuName, "NTX") == 0);

		const std::wstring& appFolder = GetAbsoluteInstallPath(pszSkuName);

        // Preselect DirectConnection as output device when we launch from plugin. 
		std::wstring appParams = L"-OUTPUTDEVICE DirectConnection";

		if (bSkuNTX)
		{
			// If this is an NTX plugin build, do not show the UI
			appParams += L" -SHOWUI 0";
		}
		if (pHostPID)
		{
			std::string sHostPID(pHostPID);
			appParams += (L" -HOSTPID " + std::wstring(sHostPID.begin(), sHostPID.end()));
		}

        // From MSDN for ShellExecute:
        // If the function succeeds, it returns a value greater than 32.
        // If the function fails, it returns an error value that indicates the cause of the failure.

        HINSTANCE hInstance = ::ShellExecuteW(nullptr, L"open", appFilePath.c_str(), appParams.c_str(), appFolder.c_str(), SW_SHOWNORMAL);

        result = (reinterpret_cast<size_t>(hInstance) > 32);
    }
#elif __APPLE__
    // Make full path of executable Standalone
    const std::string& appFilePath = GetAbsoluteStandaloneFilePath(pszSkuName);
	NBAssert(!appFilePath.empty());	// app file not found!
    if (!appFilePath.empty())
    {
        const bool bSkuNTX = (strcasecmp(pszSkuName, "NTX") == 0);
        std::string sHostPID;
        if(pHostPID)
        {
            sHostPID = std::string(pHostPID);
        }
        
        CFStringRef refHostPID = CFStringCreateWithCString(NULL, sHostPID.data(), kCFStringEncodingASCII);
        // Use deprecated functions to avoid the usage of Obj-C objects here.
        FSRef appRef;
        FSPathMakeRef(reinterpret_cast<const UInt8 *>(appFilePath.c_str()), &appRef, NULL);
        
        // Preselect DirectConnection as output device when we launch from plugin
        CFStringRef vArgs[6] = {CFSTR("-OUTPUTDEVICE"),		CFSTR("DirectConnection"),
                                CFSTR("-HOSTPID"),			refHostPID,
								// If this is an NTX plugin build, do not show the UI
								CFSTR("-SHOWUI"),			CFSTR("0")};

        
        CFArrayRef argsArr;
        CFArrayRef args = CFArrayCreate(NULL, reinterpret_cast<const void**>(vArgs), bSkuNTX ? 6 : 4, NULL);

        LSApplicationParameters params = {};

        params.application = &appRef;
        // kLSLaunchNewInstance enables launch new app instance
        params.flags = kLSLaunchAsync | kLSLaunchDontAddToRecents | kLSLaunchNewInstance;
        params.argv = args;

        ProcessSerialNumber processId = {0};
        OSStatus err = LSOpenApplication(&params, &processId);

        // No longer need
        CFRelease(args);

        result = (err == noErr);
    }
#else
#   error Unsupported platform
#endif

	return result;
}


// ServerPortAndChannels class implementation 
ServerPortAndChannels::ServerPortAndChannels(const char* pszSkuName, const char* pHostPID)
	: m_skuName(pszSkuName)
{
	if (pHostPID)
		m_sHostPID = std::string(pHostPID);

#ifdef TITLER_LIVE_NTX
	NBAssertStr(!m_sHostPID.empty(), "For NTX Host's PID must exist !");
#endif
}

ServerPortAndChannels::~ServerPortAndChannels()
{
	if (m_systemObject.IsInited())
	{
		// Clear previous values from shared memory.
		SetPortAndChannels(PortAndChannels());
		m_systemObject.FreeSharedMemory();
	}
}

bool ServerPortAndChannels::SetPortAndChannels(const PortAndChannels& portAndChannels)
{
	bool result = false;

	// To avoid unnecessary operations when previous data is the same.
	if (m_portAndChannelsValid && m_currentPortAndChannels == portAndChannels)
	{
		// Just return SUCCESS
		result = true;
	}
	else
	{
		if (!m_systemObject.IsInited())
		{
			const std::string& uniqName = MakeUniqServerPortAndChannelsName(m_skuName.c_str(), m_sHostPID.data());
			result = m_systemObject.InitSharedMemory(uniqName, true, 64 * 1024);	// 64 Kb will be enough
			NBAssert(result);	// need to check
			if (!result)
			{
				result = m_systemObject.InitSharedMemory(uniqName, false, 64 * 1024);
			}
		}
		else
		{
			result = true;
		}

		if (result)
		{
			// We will save ports and channels in this format:
			// <port_0>;<channel_0>,\n
			// <port_1>;<channel_0>,<channel_1>,<channel_2>,\n

			// Take current data from shared memory.
			std::list<PortAndChannels> currentData =
				GetAvailableDataFromString(reinterpret_cast<const char*>(m_systemObject.GetBuffer()));

			if (m_portAndChannelsValid)
			{
				// Search previous data, it should exist.
				auto it = std::find_if(currentData.begin(), currentData.end(),
					[&](const PortAndChannels& data) { return data.first == m_currentPortAndChannels.first; });

				NBAssert(it != currentData.end());	// check it, it should not alert, because we wrote it before.

				if (it != currentData.end())
				{
					// Remove old values of port and channels from 'currentData'.
					currentData.erase(it);
				}
			}

			// And push new values, if they are valid.
			if (portAndChannels.first > 0)
			{
				bool bAddNew = true;

				// This is special case for OSX, when Standalone will try to remove the dead ports.
				for (const DCTL::PortAndChannels& item : currentData)
				{
					if (item == portAndChannels)
					{
						bAddNew = false;
						break;
					}
				}

				if (bAddNew)
				{
					currentData.push_back(portAndChannels);
				}

				m_portAndChannelsValid = true;
				m_currentPortAndChannels = portAndChannels;
			}
			else
			{
				m_portAndChannelsValid = false;
				m_currentPortAndChannels.first = 0;
				m_currentPortAndChannels.second.clear();
			}

			// Convert 'currentData' to string
			const std::string& convertedData = ConvertPortAndChannelsToString(currentData);

			// TODO: now it is not important, but in future we need to implement some functional for
			// synchronization of two calls GetAvailableDataFromString(m_systemObject.GetBuffer()) and
			// this call of strncpy(shared_memory_dest, ...) between several processes.

			NBAssert(convertedData.size() + 1 < static_cast<size_t>(m_systemObject.GetCurrentSize()));

			char* pszDestination = static_cast<char*>(m_systemObject.GetBuffer());
			NBAssert(pszDestination);
			if (convertedData.empty())
			{
				pszDestination[0] = 0;
			}
			else
			{
				const size_t maxSharedMemorySize = static_cast<size_t>(m_systemObject.GetCurrentSize() - 1);
				const size_t copiedSize = std::min(convertedData.size(), maxSharedMemorySize);

				strncpy(pszDestination, convertedData.c_str(), maxSharedMemorySize);

				pszDestination[copiedSize] = 0;	// fill null-character
			}
		}
	}

	return result;
}

std::list<PortAndChannels> ServerPortAndChannels::GetAvailableSources(const char* pszSkuName, const char* pHostPID)
{
	std::list<PortAndChannels> result;

	// Make name for system object
	const std::string& uniqName = MakeUniqServerPortAndChannelsName(pszSkuName, pHostPID);
	CSharedMemory systemObject;

	// Open existing shared memory
	bool opened = systemObject.InitSharedMemory(uniqName, false, 64 * 1024);

	if (opened)
	{
		// Take sources from shared memory.
		result = GetAvailableDataFromString(reinterpret_cast<const char*>(systemObject.GetBuffer()));
	}

	return result;
}

}
