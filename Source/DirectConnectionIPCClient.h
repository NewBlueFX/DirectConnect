//	DirectConnectionIPCClient.h
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

#include "DirectConnectionInterface.h"
#include "DirectConnectionTypes.h"


namespace DCTL
{

// Forward declaration
class DirectConnectionIPCClientBase;
__interface IDirectConnectionNTXHostServer;

// IPC - Inter-Process Communication.
// DirectConnectionIPCClient holds Win/OSX client object which works between processes.
// This wraps platform-dependent client and calls it.
// Also DirectConnectionIPCClient manages the connection between client on this side and server on
// the Standalone side. The Standalone is the application of TitlerLive or OvrStream.
// This is thread-safe object, because the initialization is performed in c-r, and the lifetime is
// managed by reference counter.
class DirectConnectionIPCClient
{
	DirectConnectionIPCClient(const DirectConnectionIPCClient &str);	// not implemented
	DirectConnectionIPCClient& operator=(const DirectConnectionIPCClient &str);	// not implemented

public:

	// Initialize a IPC client on given port.
	DirectConnectionIPCClient(Port serverPortOnStandaloneSide, bool bConnectToServer, bool* pbInitialized);

	// Any object that references this connector can safely call it as long as it keeps the reference count up.
	ULONG AddRef();
	ULONG Release();

	// Get the client port on which it was initialized
	Port GetClientPort() const;

	// All following calls are valid if 'bConnectToServer' was equal true.

	// Process id with which we connected.
	DCTLProcessID GetProcessID() const;

	// Check if process is running right now.
	bool IsProcessRunning() const;

    // Calls to IPC engine to create instance.
    DCResult CreateInstance(DCTLInstanceID instanceId);
    DCResult DestroyInstance(DCTLInstanceID instanceId);
	// Calls "client to server" for DirectConnection instance
	DCResult InitializeDirectConnection(DCTLInstanceID instanceId, int dcChannel,
		int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio, bool bInterlaced,
		int nSampleRate, int nNumChannels, double* pdStartTime);
	DCResult GetVideoSettings(DCTLInstanceID instanceId, int dcChannel, int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced);
    DCResult GetAudioSettings(DCTLInstanceID instanceId, int dcChannel, int* pnSampleRate, int* pnNumChannels);
	DCResult GetVideoFrame(DCTLInstanceID instanceId, int dcChannel, VideoFrameParams* pFrame, double dTime);
	DCResult GetAudioFrame(DCTLInstanceID instanceId, int dcChannel, AudioFrameParams* pFrame, double dTime);
	DCResult GetMetadataFrame(DCTLInstanceID instanceId, int dcChannel, char** ppszBuffer);
	DCResult FreeMetadataFrame(char* pszData);
	DCResult SendMetadataFrame(DCTLInstanceID instanceId, int dcChannel, const char* pszMetadata);
	DCResult FreeSharedMemory(DCTLInstanceID instanceId, DCTLSharedMemoryType type);
	DCResult SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface);
	DCResult SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata);

private:

	// Use Release() instead of direct removing.
	~DirectConnectionIPCClient();

	DirectConnectionIPCClientBase* m_pPlatformClient = nullptr;	// Platform dependent client
	LONG m_nRefCount = 1;					// Ref count for AddRef/Release.
};

}	// end namespace DCTL
