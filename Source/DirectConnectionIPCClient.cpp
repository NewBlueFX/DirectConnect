//	DirectConnectionIPCClient.cpp
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

#include "DirectConnectionIPCClient.h"
#include <random>
#include <chrono>

#ifdef _WIN32
#   include "DirectConnectionIPCClientWin.h"
#elif __APPLE__
#   include "DirectConnectionIPCClientOSX.h"
#else
#   error Unsupported platform
#endif

namespace DCTL
{

namespace
{

DCTLClientID MakeUniqueClientID(void* pThis, int port)
{
	// Construct a random generator engine from a time-based seed (obtain a seed from the system clock):
	const unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	std::default_random_engine generator(seed);

	// Take random value
	const DCTLClientID randomValue = static_cast<DCTLClientID>(generator());

	// And another crazy step to make a random value - multiply on the pointer value (and port).
	return (reinterpret_cast<DCTLClientID>(pThis) * randomValue * port);
}

}

DirectConnectionIPCClient::DirectConnectionIPCClient(Port serverPortOnStandaloneSide, bool bConnectToServer, bool* pbInitialized)
{
	const DCTLClientID clientID = MakeUniqueClientID(this, serverPortOnStandaloneSide);		// Uniq client Id, which client will use for IPC calls.
	bool bInitialized = false;

#ifdef _WIN32
	m_pPlatformClient = new DirectConnectionIPCClientWin(serverPortOnStandaloneSide, clientID, &bInitialized);
#elif __APPLE__
	m_pPlatformClient = new DirectConnectionIPCClientOSX(serverPortOnStandaloneSide, clientID, &bInitialized);
#else
#   error Unsupported platform
#endif

	if (bInitialized && bConnectToServer)
	{
		bInitialized = m_pPlatformClient->ConnectToServer();
	}

	if (!bInitialized)
	{
		delete m_pPlatformClient;
		m_pPlatformClient = nullptr;
	}

	if (pbInitialized)
	{
		*pbInitialized = bInitialized;
	}
}

ULONG DirectConnectionIPCClient::AddRef()
{
	return static_cast<ULONG>(::InterlockedIncrement(&m_nRefCount));
}

ULONG DirectConnectionIPCClient::Release()
{
	LONG refCount = static_cast<LONG>(::InterlockedDecrement(&m_nRefCount));
	if (refCount == 0)
	{
		delete this;
		return 0;
	}

	return static_cast<ULONG>(refCount);
}

DirectConnectionIPCClient::~DirectConnectionIPCClient()
{
	if (m_pPlatformClient)
	{
		delete m_pPlatformClient;
		m_pPlatformClient = nullptr;
	}
}

Port DirectConnectionIPCClient::GetClientPort() const
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->GetClientPort() : 0;
}

DCTLProcessID DirectConnectionIPCClient::GetProcessID() const
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->GetProcessID() : 0;
}

bool DirectConnectionIPCClient::IsProcessRunning() const
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->IsProcessRunning() : false;
}

DCResult DirectConnectionIPCClient::CreateInstance(DCTLInstanceID instanceId)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->CreateInstance(instanceId) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::DestroyInstance(DCTLInstanceID instanceId)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->DestroyInstance(instanceId) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::InitializeDirectConnection(
	DCTLInstanceID instanceId, int dcChannel, int nWidth, int nHeight, double dFrameRate,
	double dPixelAspectRatio, bool bInterlaced, int nSampleRate, int nNumChannels, double* pdStartTime)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->InitializeDirectConnection(
		instanceId, dcChannel, nWidth, nHeight, dFrameRate, dPixelAspectRatio, bInterlaced,
		nSampleRate, nNumChannels, pdStartTime) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::GetVideoSettings(DCTLInstanceID instanceId, int dcChannel, int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->GetVideoSettings(instanceId, dcChannel,
		pnWidth, pnHeight, pdFrameRate, pdPixelAspectRatio, pbInterlaced) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::GetAudioSettings(DCTLInstanceID instanceId, int dcChannel, int* pnSampleRate, int* pnNumChannels)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->GetAudioSettings(instanceId, dcChannel, pnSampleRate, pnNumChannels) : DCERR_ServerShutdown;
}

DCTL::DCResult DirectConnectionIPCClient::GetVideoFrame(DCTLInstanceID instanceId, int dcChannel, VideoFrameParams* pFrame, double dTime)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->GetVideoFrame(instanceId, dcChannel, pFrame, dTime) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::GetAudioFrame(DCTLInstanceID instanceId, int dcChannel, AudioFrameParams* pFrame, double dTime)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->GetAudioFrame(instanceId, dcChannel, pFrame, dTime) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::GetMetadataFrame(DCTLInstanceID instanceId, int dcChannel, char** ppszBuffer)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->GetMetadataFrame(instanceId, dcChannel, ppszBuffer) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::FreeMetadataFrame(char* pszData)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->FreeMetadataFrame(pszData) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::SendMetadataFrame(DCTLInstanceID instanceId, int dcChannel, const char* pszMetadata)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->SendMetadataFrame(instanceId, dcChannel, pszMetadata) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::FreeSharedMemory(DCTLInstanceID instanceId, DCTLSharedMemoryType type)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->FreeSharedMemory(instanceId, type) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->SetHostServer(pHostServerInterface) : DCERR_ServerShutdown;
}

DCResult DirectConnectionIPCClient::SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata)
{
	NBAssert(m_pPlatformClient);
	return m_pPlatformClient ? m_pPlatformClient->SendGlobalMetadataFrame(dcChannel, pszMetadata) : DCERR_ServerShutdown;
}

}	// end namespace DCTL
