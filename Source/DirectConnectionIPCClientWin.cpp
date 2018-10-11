//	DirectConnectionIPCClientWin.cpp
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

#include "DirectConnectionIPCClientWin.h"
#include "DirectConnectionIPCSettings.h"
#include "DirectConnectionNTXInterfaces.h"
#include "DirectConnectionGeneric.h"
#include "DirectConnectionTLInterface_h.h"	// To declare GetVideoFrame()/GetVideoSettings() and other RPC functions
#include "DirectConnectionTLHostInterface_h.h"	// To declare DirectConnectRPCClientConnected() function.


namespace DCTL
{
static std::map<DCTLClientID, DirectConnectionIPCClientWin*> g_winClients;
static std::recursive_mutex g_winClientsLock;

// Anonymous namespace for helpful and internal functions
namespace
{

// RPC calls to other process.
void ClientDisconnectTryCatch(handle_t IDL_handle, DCTLClientID clientID, int serverPort)
{
	RpcTryExcept
	{
		::DirectConnectionClientDisconnect(IDL_handle, clientID, serverPort);
	}
	RpcExcept(1)
	{
		// TODO add error to log.
	}
	RpcEndExcept
}

bool ClientConnectTryCatch(handle_t IDL_handle, DCTLClientID clientID, int serverPort)
{
	bool res = false;

	RpcTryExcept
	{
		::DirectConnectionClientConnect(IDL_handle, clientID, serverPort);
		res = true;
	}
	RpcExcept(1)
	{
		// TODO add error to log.
	}
	RpcEndExcept

	return res;
}

DCResult FreeSharedMemoryTryCatch( /* [in] */ handle_t IDL_handle, /* [in] */ DCTLClientID clientID,
	/* [in] */ DCTLInstanceID instanceID, /* [in] */ DCTLSharedMemoryType type)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		result = static_cast<DCResult>(::DirectConnectionFreeSharedMemory(IDL_handle, clientID, instanceID, type));
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult CreateInstanceTryCatch(
	/* [in] */ handle_t IDL_handle,
	/* [in] */ DCTLClientID clientID,
	/* [in] */ DCTLInstanceID instanceID)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		::DirectConnectionCreateInstance(IDL_handle, clientID, instanceID);
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult DestroyInstanceTryCatch(
	/* [in] */ handle_t IDL_handle,
	/* [in] */ DCTLClientID clientID,
	/* [in] */ DCTLInstanceID instanceID)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		::DirectConnectionDestroyInstance(IDL_handle, clientID, instanceID);
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult InitializeDirectConnectionTryCatch(handle_t IDL_handle, DCTLClientID clientID,
	DCTLInstanceID instanceID, int dcChannel,
	int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio, bool bInterlaced,
	int nSampleRate, int nNumChannels,
	double* pdStartTime)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		result = static_cast<DCResult>(::DirectConnectionInitialize(IDL_handle, clientID, instanceID, dcChannel,
			nWidth, nHeight, dFrameRate, dPixelAspectRatio, bInterlaced, nSampleRate, nNumChannels, pdStartTime));
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult GetVideoSettingsTryCatch(handle_t IDL_handle, DCTLClientID clientID, DCTLInstanceID instanceID,
	int dcChannel, int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		boolean bInterlaced = 0;

		result = static_cast<DCResult>(::DirectConnectionGetVideoSettings(IDL_handle, clientID, instanceID,
			dcChannel, pnWidth, pnHeight, pdFrameRate, pdPixelAspectRatio, &bInterlaced));

		if (pbInterlaced)
		{
			*pbInterlaced = bInterlaced ? true : false;
		}
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult GetAudioSettingsTryCatch(handle_t IDL_handle, DCTLClientID clientID, DCTLInstanceID instanceID,
	int dcChannel, int* pnSampleRate, int* pnNumChannels)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		result = static_cast<DCResult>(::DirectConnectionGetAudioSettings(IDL_handle, clientID, instanceID, dcChannel, pnSampleRate, pnNumChannels));
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult GetVideoFrameTryCatch(handle_t IDL_handle, DCTLClientID clientID,
	DCTLInstanceID instanceID, int dcChannel, VideoFrameParams* pFrame, double dTime)
{
	NBAssert(pFrame);
	CheckPointer(pFrame, DCERR_Failed);

	DCResult result = DCResult::DCERR_OK;
	unsigned char* pBuffer = pFrame->m_pBuffer;
	pFrame->m_pBuffer = nullptr;
	DCTLVideoFrameParamsIDL frameStub = Utility::ConvertDCFrameToIDL(*pFrame);

	RpcTryExcept
	{
		result = static_cast<DCResult>(::DirectConnectionGetVideoFrame(IDL_handle, clientID, instanceID, dcChannel, &frameStub, dTime));

		// Update VideoFrameParams from RPC structure.
		*pFrame = Utility::ConvertIDLFrameToDC(frameStub);
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	pFrame->m_pBuffer = pBuffer;

	return result;
}

DCResult GetAudioFrameTryCatch(handle_t IDL_handle, DCTLClientID clientID,
	DCTLInstanceID instanceID, int dcChannel, AudioFrameParams* pFrame, double dTime)
{
	DCResult result = DCResult::DCERR_OK;

	DCTLAudioFrameParamsIDL frameStub;

	static_assert(	sizeof(frameStub.m_nSampleRate) +
					sizeof(frameStub.m_nChannels) +
					sizeof(frameStub.m_nSampleCount) == sizeof(DCTLAudioFrameParamsIDL),
					"Seems someone changed the size of the DCTLAudioFrameParamsIDL, do not forget to change the related code!");
	static_assert(	sizeof(pFrame->m_pBuffer) +
					sizeof(pFrame->m_nSampleRate) +
					sizeof(pFrame->m_nChannels) +
					sizeof(pFrame->m_nSampleCount) == sizeof(AudioFrameParams),
					"Seems someone changed the size of the AudioFrameParams, do not forget to change the related code!");

	// Set up a Interface frame from the Direct Connection frame.
	frameStub.m_nSampleRate = pFrame->m_nSampleRate;
	frameStub.m_nChannels = pFrame->m_nChannels;
	frameStub.m_nSampleCount = pFrame->m_nSampleCount;

 	RpcTryExcept
 	{
 		result = static_cast<DCResult>(::DirectConnectionGetAudioFrame(IDL_handle, clientID, instanceID, dcChannel, &frameStub, dTime));

		// Update AudioFrameParams from RPC structure
		pFrame->m_nSampleRate = frameStub.m_nSampleRate;
		pFrame->m_nChannels = frameStub.m_nChannels;
		pFrame->m_nSampleCount = frameStub.m_nSampleCount;
 	}
 	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
 	}
 	RpcEndExcept

	return result;
}

DCResult GetMetadataFrameTryCatch(handle_t IDL_handle, DCTLClientID clientID,
	DCTLInstanceID instanceID, int dcChannel, long* pBufferSize, unsigned char** ppBuffer)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		result = static_cast<DCResult>(::DirectConnectionGetMetadataFrame(IDL_handle, clientID, instanceID, dcChannel, pBufferSize, ppBuffer));
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult SendMetadataFrameTryCatch(handle_t IDL_handle, DCTLClientID clientID,
	DCTLInstanceID instanceID, int dcChannel, const unsigned char* pszMetadata)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		result = static_cast<DCResult>(::DirectConnectionSendMetadataFrame(IDL_handle, clientID, instanceID, dcChannel, pszMetadata));
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

DCResult SendGlobalMetadataFrameTryCatch(handle_t IDL_handle, int dcChannel, const unsigned char* pszMetadata)
{
	DCResult result = DCResult::DCERR_OK;

	RpcTryExcept
	{
		result = static_cast<DCResult>(::DirectConnectionSendGlobalMetadataFrame(IDL_handle, dcChannel, pszMetadata));
	}
	RpcExcept(1)
	{
		result = DCResult::DCERR_ServerShutdown;
	}
	RpcEndExcept

	return result;
}

}

DirectConnectionIPCClientWin::DirectConnectionIPCClientWin(Port serverPortOnStandaloneSide, DCTLClientID clientID, bool* pbInitialized)
	: m_clientID(clientID)
{
	bool bInitialized = m_rpcBaseClient.initClient(RPC_DC_PROTOCOL, serverPortOnStandaloneSide);

	if (pbInitialized)
	{
		*pbInitialized = bInitialized;
	}

	{
		// Add to map in any case.
		std::lock_guard<std::recursive_mutex> lock(g_winClientsLock);
		g_winClients[m_clientID] = this;
	}
}

DirectConnectionIPCClientWin::~DirectConnectionIPCClientWin()
{
	{
		// Remove from map current object.
		std::lock_guard<std::recursive_mutex> lock(g_winClientsLock);
		g_winClients.erase(m_clientID);
	}

	if (GetExplicitHandle() && m_rpcBaseServer.getPort())
	{
		ClientDisconnectTryCatch(GetExplicitHandle(), m_clientID, m_rpcBaseServer.getPort());
	}

	if (m_processHandle)
	{
		::CloseHandle(m_processHandle);
		m_processHandle = nullptr;
	}

	m_processID = 0;

	m_rpcBaseClient.deinitClient();
	m_rpcBaseServer.deinitServer(DirectConnectionTLHostInterface_v1_0_s_ifspec);

	if (m_pHostServerInterface)
	{
		m_pHostServerInterface->Release();
		m_pHostServerInterface = nullptr;
	}
}

bool DirectConnectionIPCClientWin::ConnectToServer()
{
	bool bResult = false;

	NBAssert(m_rpcBaseServer.getPort() == 0);	// Do not use DirectConnectionIPCClientWin() twice. Behavior is undefined.

	// Check if the rpc client is initialized
	NBAssert(GetExplicitHandle());
	if (GetExplicitHandle() && m_rpcBaseServer.getPort() == 0)
	{
		bResult = m_rpcBaseServer.initServer(RPC_DC_PROTOCOL,
			RPC_DC_START_HOST_PORT, DirectConnectionTLHostInterface_v1_0_s_ifspec, false);

		if (bResult)
		{
			// Now try to connect to server,
			// it will call DirectConnectRPCClientConnected() on our side (it will fill m_processHandle && m_processID),
			// if everything is okay.
			bResult = ClientConnectTryCatch(GetExplicitHandle(), m_clientID, m_rpcBaseServer.getPort());
			NBAssert(bResult);
			if (bResult)
			{
				bResult = m_processHandle && m_processID;
			}
		}
	}

	return bResult;
}

Port DirectConnectionIPCClientWin::GetClientPort() const
{
	return m_rpcBaseClient.getPort();
}

DCTLProcessID DirectConnectionIPCClientWin::GetProcessID() const
{
	return m_processID;
}

bool DirectConnectionIPCClientWin::IsProcessRunning() const
{
	bool bResult = false;

	if (m_processHandle)
	{
		bResult = ::WaitForSingleObject(m_processHandle, 0) == WAIT_TIMEOUT;
	}

	return bResult;
}

DCResult DirectConnectionIPCClientWin::CreateInstance(DCTLInstanceID instanceID)
{
	return CreateInstanceTryCatch(GetExplicitHandle(), m_clientID, instanceID);
}

DCResult DirectConnectionIPCClientWin::DestroyInstance(DCTLInstanceID instanceID)
{
	return DestroyInstanceTryCatch(GetExplicitHandle(), m_clientID, instanceID);
}

DCResult DirectConnectionIPCClientWin::InitializeDirectConnection(
	DCTLInstanceID instanceID, int dcChannel, int nWidth, int nHeight, double dFrameRate,
	double dPixelAspectRatio, bool bInterlaced, int nSampleRate, int nNumChannels, double* pdStartTime)
{
	return InitializeDirectConnectionTryCatch(GetExplicitHandle(), m_clientID,
		instanceID, dcChannel, nWidth, nHeight, dFrameRate, dPixelAspectRatio, bInterlaced,
		nSampleRate, nNumChannels, pdStartTime);
}

DCResult DirectConnectionIPCClientWin::GetVideoSettings(DCTLInstanceID instanceID, int dcChannel, int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced)
{
	return GetVideoSettingsTryCatch(GetExplicitHandle(), m_clientID, instanceID, dcChannel,
		pnWidth, pnHeight, pdFrameRate, pdPixelAspectRatio, pbInterlaced);
}

DCResult DirectConnectionIPCClientWin::GetAudioSettings(DCTLInstanceID instanceID, int dcChannel, int* pnSampleRate, int* pnNumChannels)
{
	return GetAudioSettingsTryCatch(GetExplicitHandle(), m_clientID, instanceID, dcChannel, pnSampleRate, pnNumChannels);
}

DCTL::DCResult DirectConnectionIPCClientWin::GetVideoFrame(DCTLInstanceID instanceID, int dcChannel, VideoFrameParams* pFrame, double dTime)
{
	return GetVideoFrameTryCatch(GetExplicitHandle(), m_clientID, instanceID, dcChannel, pFrame, dTime);
}

DCResult DirectConnectionIPCClientWin::GetAudioFrame(DCTLInstanceID instanceID, int dcChannel, AudioFrameParams* pFrame, double dTime)
{
	return GetAudioFrameTryCatch(GetExplicitHandle(), m_clientID, instanceID, dcChannel, pFrame, dTime);
}

DCResult DirectConnectionIPCClientWin::GetMetadataFrame(DCTLInstanceID instanceID, int dcChannel, char** ppszBuffer)
{
	DCResult result = DCResult::DCERR_Failed;

	if (ppszBuffer)
	{
		long lSize = 0;
		unsigned char* pBuffer = nullptr;

		result = GetMetadataFrameTryCatch(GetExplicitHandle(), m_clientID, instanceID, dcChannel, &lSize, &pBuffer);

		if (result == DCResult::DCERR_OK)
		{
			*ppszBuffer = new char[lSize + 1];
			memcpy(*ppszBuffer, pBuffer, lSize);
			(*ppszBuffer)[lSize] = 0;	// null-terminated, because ppszName is 'char'
			::MIDL_user_free(pBuffer);
		}
	}

	return result;
}

DCResult DirectConnectionIPCClientWin::FreeMetadataFrame(char* pszData)
{
	NBAssert(pszData);
	delete[] pszData;
    return DCResult::DCERR_OK;
}

DCResult DirectConnectionIPCClientWin::SendMetadataFrame(DCTLInstanceID instanceID, int dcChannel, const char* pszMetadata)
{
	return SendMetadataFrameTryCatch(GetExplicitHandle(), m_clientID, instanceID, dcChannel, reinterpret_cast<const unsigned char*>(pszMetadata));
}

DCResult DirectConnectionIPCClientWin::FreeSharedMemory(DCTLInstanceID instanceID, DCTLSharedMemoryType type)
{
	return FreeSharedMemoryTryCatch(GetExplicitHandle(), m_clientID, instanceID, type);
}

DCResult DirectConnectionIPCClientWin::SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata)
{
	return SendGlobalMetadataFrameTryCatch(GetExplicitHandle(), dcChannel, reinterpret_cast<const unsigned char*>(pszMetadata));
}

void DirectConnectionIPCClientWin::ProcessLoaded(DCTLClientID clientID, int serverPort, DWORD processID)
{
	NBAssert(serverPort == GetClientPort());
	NBAssert(clientID == m_clientID);
	NBAssert(processID);

    static_assert(sizeof(DCTLProcessID) == sizeof(DWORD), "Should be the same!");
	m_processID = processID;

	m_processHandle = ::OpenProcess(SYNCHRONIZE, FALSE, m_processID);
	NBAssert(m_processHandle);
}

IDirectConnectionNTXHostServer* DirectConnectionIPCClientWin::GetHostServerInterface() const
{
	std::lock_guard<std::recursive_mutex> lock(m_hostServerInterfaceLock);

	if (m_pHostServerInterface)
	{
		m_pHostServerInterface->AddRef();
	}

	return m_pHostServerInterface;
}

DCResult DirectConnectionIPCClientWin::SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface)
{
	std::lock_guard<std::recursive_mutex> lock(m_hostServerInterfaceLock);

	if (pHostServerInterface)
	{
		// We will keep one reference on server interface.
		pHostServerInterface->AddRef();
	}

	if (m_pHostServerInterface)
	{
		// Release previous reference (if it exists).
		m_pHostServerInterface->Release();
	}

	m_pHostServerInterface = pHostServerInterface;

	return DCResult::DCERR_OK;
}

handle_t DirectConnectionIPCClientWin::GetExplicitHandle() const
{
	return m_rpcBaseClient.getExplicitHandle();
}

}	// end namespace DCTL

using namespace DCTL;

// Implemented RPC call from server
void DirectConnectionClientConnected(
	/* [in] */ handle_t /*IDL_handle*/,
	/* [in] */ DCTLClientID clientID,
	/* [in] */ int serverPort,
	/* [in] */ DCTLProcessID processID,
	/* [out] */ DCTLProcessID *hostProcessId)
{
	// Lock map to be sure that all objects will be alive during this call.
	std::lock_guard<std::recursive_mutex> lock(g_winClientsLock);
	auto iter = g_winClients.find(clientID);
	NBAssert(iter != g_winClients.end());
	if (iter != g_winClients.end())
	{
		(*iter).second->ProcessLoaded(clientID, serverPort, processID);
		*hostProcessId = ::GetCurrentProcessId();
	}
}

// Implemented GetHostFrame call from server. Returns NLE video frame for Title Designer
int DirectConnectionGetHostFrame(
	/* [in] */ handle_t IDL_handle,
	/* [in] */ DCTLClientID clientID,
	/* [in] */ DCTLInstanceID instanceID,
	/* [in] */ int nVideoFrameType,
	/* [out][in] */ DCTLVideoRenderInfoIDL *pRenderInfo,
	/* [out][in] */ DCTLVideoFrameParamsIDL *pFrame)
{
	NBAssert(pRenderInfo);
	NBAssert(pFrame);

	int nResult = DCERR_Failed;

	if (pRenderInfo && pFrame)
	{
		// Get only host reference to take frame.
		IDirectConnectionNTXHostServer* pHostServer = nullptr;
		{
			// Lock map to be sure that all objects will be alive during this scope.
			std::lock_guard<std::recursive_mutex> lock(g_winClientsLock);
			auto iter = g_winClients.find(clientID);
			NBAssert(iter != g_winClients.end());
			if (iter != g_winClients.end())
			{
				pHostServer = (*iter).second->GetHostServerInterface();
			}
		}

		if (pHostServer)
		{
			// Convert RPC structures to DCTL structures
			VideoRenderInfo renderInfo = Utility::ConvertIDLRenderInfoToDC(*pRenderInfo);
			VideoFrameParams frame = Utility::ConvertIDLFrameToDC(*pFrame);

			nResult = pHostServer->GetHostFrame(instanceID, nVideoFrameType, &renderInfo, &frame);

			if (nResult == DCERR_OK)
			{
				// And vice-versa (DCTL structures -> RPC structures)
				*pRenderInfo = Utility::ConvertDCRenderInfoToIDL(renderInfo);
				*pFrame = Utility::ConvertDCFrameToIDL(frame);
			}

			pHostServer->Release();
		}
	}

	return nResult;
}
