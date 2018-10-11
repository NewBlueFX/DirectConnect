/*	DirectConnectionIPCClientWin.h
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


#pragma once

#include "DirectConnectionNTXInterfaces.h"
#include "DirectConnectionIPCClientBase.h"
#include "RpcBaseClient.h"
#include "RpcBaseServer.h"
#include <map>		// std::map
#include <mutex>	// recursive_mutex/lock_guard

namespace DCTL
{

// This class is RPC Win client.
class DirectConnectionIPCClientWin : public DirectConnectionIPCClientBase
{
	DirectConnectionIPCClientWin(const DirectConnectionIPCClientWin &str);	// not implemented
	DirectConnectionIPCClientWin& operator=(const DirectConnectionIPCClientWin &str);	// not implemented

public:

	// Initialize a RPC client on given port.
	DirectConnectionIPCClientWin(Port serverPortOnStandaloneSide, DCTLClientID clientID, bool* pbInitialized);
	virtual ~DirectConnectionIPCClientWin();

	// Connect to server with given port.
	virtual bool ConnectToServer() override;

	// Get the client port on which it was initialized
	virtual Port GetClientPort() const override;

	// Process id with which we connected. This is valid after the ConnectToServer() call.
	virtual DCTLProcessID GetProcessID() const override;

	// Check if process is running right now. The result of this call is valid after the ConnectToServer() call.
	virtual bool IsProcessRunning() const override;

    // Calls to IPC engine to create instance.
	virtual DCResult CreateInstance(DCTLInstanceID instanceID) override;
	virtual DCResult DestroyInstance(DCTLInstanceID instanceID) override;
	// Calls "client to server" for DirectConnection instance
	virtual DCResult InitializeDirectConnection(DCTLInstanceID instanceID, int dcChannel,
		int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio, bool bInterlaced,
		int nSampleRate, int nNumChannels, double* pdStartTime) override;
	virtual DCResult GetVideoSettings(DCTLInstanceID instanceID, int dcChannel, int* pnWidth, int* pnHeight,
		double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced) override;
	virtual DCResult GetAudioSettings(DCTLInstanceID instanceID, int dcChannel, int* pnSampleRate, int* pnNumChannels) override;
	virtual DCResult GetVideoFrame(DCTLInstanceID instanceID, int dcChannel, VideoFrameParams* pFrame, double dTime) override;
	virtual DCResult GetAudioFrame(DCTLInstanceID instanceID, int dcChannel, AudioFrameParams* pFrame, double dTime) override;
	virtual DCResult GetMetadataFrame(DCTLInstanceID instanceID, int dcChannel, char** ppszBuffer) override;
	virtual DCResult FreeMetadataFrame(char* pszData) override;
	virtual DCResult SendMetadataFrame(DCTLInstanceID instanceID, int dcChannel, const char* pszMetadata) override;
	virtual DCResult FreeSharedMemory(DCTLInstanceID instanceID, DCTLSharedMemoryType type) override;
	virtual DCResult SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata) override;

	// Sets the server interface used to make calls back to the NLE host
	virtual DCResult SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface) override;

	// Internal function.
	void ProcessLoaded(DCTLClientID clientID, int serverPort, DWORD processID);

	// Returns AddRef-d interface.
	IDirectConnectionNTXHostServer* GetHostServerInterface() const;

private:
	// Get client RPC explicit handle (Win)
	handle_t GetExplicitHandle() const;

	DCTLProcessID m_processID = 0;		// Process Id with which we connected.
	RpcBaseClient m_rpcBaseClient;		// RPC object (client) which connects to server.
	RpcBaseServer m_rpcBaseServer;		// Server on client side
	HANDLE m_processHandle = nullptr;	// Process Handle.
	const DCTLClientID m_clientID = 0;	// Uniq client Id, which we use for RPC calls.

	IDirectConnectionNTXHostServer* m_pHostServerInterface = nullptr;		// Host server interface for making calls back to NLE
	mutable std::recursive_mutex m_hostServerInterfaceLock;					// Mutex for 'm_pHostServerInterface' object.
};

}	// end namespace DCTL
