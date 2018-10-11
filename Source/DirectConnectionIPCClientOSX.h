/*	DirectConnectionIPCClientOSX.h
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

#include "DirectConnectionTypes.h"
#include "DirectConnectionInterface.h"
#include "DirectConnectionIPCClientBase.h"
#include <map>
#include <memory>
#include <mutex>


namespace DCTL
{

// Forward declaration
struct DirectConnectionIPCClientOSXPrivateData;
struct DirectConnectionInstanceDataOSX;

// IPC - Inter-Process Communication.
// The implementation of IPC client for OSX system.
// This is used Distributed Objects technology for IPC between client side and server side (Standalone app).
class DirectConnectionIPCClientOSX : public DirectConnectionIPCClientBase
{
	DirectConnectionIPCClientOSX(const DirectConnectionIPCClientOSX &str);	// not implemented
	DirectConnectionIPCClientOSX& operator=(const DirectConnectionIPCClientOSX &str);	// not implemented

    // Declaration of internal types.
    typedef std::shared_ptr<DirectConnectionInstanceDataOSX> SharedInstanceData;
    typedef std::map<DCTLInstanceID, SharedInstanceData> SharedInstanceDataMap;

public:

	// Initialize a IPC client on given port.
	DirectConnectionIPCClientOSX(Port serverPortOnStandaloneSide, DCTLClientID clientID, bool* pbInitialized);
	virtual ~DirectConnectionIPCClientOSX();

	// Connect to server. This call creates new Obj-C client which will work with server.
	virtual bool ConnectToServer() override;

    // Get the client port on which it was initialized
	virtual Port GetClientPort() const override;

	// Process id with which we connected. This is valid after the ConnectToServer() call.
	virtual DCTLProcessID GetProcessID() const override;

	// Check if process is running right now. The result of this call is valid after the ConnectToServer() call.
	virtual bool IsProcessRunning() const override;

    // Calls to IPC engine to create instance.
	virtual DCResult CreateInstance(DCTLInstanceID instanceId) override;
	virtual DCResult DestroyInstance(DCTLInstanceID instanceId) override;
	// Calls "client to server" for DirectConnection instance
	virtual DCResult InitializeDirectConnection(DCTLInstanceID instanceId, int dcChannel,
		int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio, bool bInterlaced,
		int nSampleRate, int nNumChannels, double* pdStartTime) override;
	virtual DCResult GetVideoSettings(DCTLInstanceID instanceId, int dcChannel, int* pnWidth,
		int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced) override;
	virtual DCResult GetAudioSettings(DCTLInstanceID instanceId, int dcChannel, int* pnSampleRate, int* pnNumChannels) override;
	virtual DCResult GetVideoFrame(DCTLInstanceID instanceId, int dcChannel, VideoFrameParams* pFrame, double dTime) override;
	virtual DCResult GetAudioFrame(DCTLInstanceID instanceId, int dcChannel, AudioFrameParams* pFrame, double dTime) override;
	virtual DCResult GetMetadataFrame(DCTLInstanceID instanceId, int dcChannel, char** ppszBuffer) override;
	virtual DCResult FreeMetadataFrame(char* pszData) override;
	virtual DCResult SendMetadataFrame(DCTLInstanceID instanceId, int dcChannel, const char* pszMetadata) override;
	virtual DCResult FreeSharedMemory(DCTLInstanceID instanceId, DCTLSharedMemoryType type) override;
	virtual DCResult SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface) override;
	virtual DCResult SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata) override;

private:

    // Thread-safe function to take SharedInstanceData from m_instances.
    SharedInstanceData GetInstanceData(DCTLInstanceID instanceId) const;

    // Free resources that associated with this instance
    DCResult DisconnectInstance(const SharedInstanceData& pInstanceData, DCTLInstanceID instanceId) const;

    // We hide Obj-C objects inside this structure to avoid compilation errors when this header file is compiled by c++ compiler.
    DirectConnectionIPCClientOSXPrivateData* m_opaqueData = nullptr;

    SharedInstanceDataMap m_instances;
    mutable std::recursive_mutex m_instancesLock;

	DCTLProcessID m_processID = 0;      // Process Id with which we connected. Filled in ConnectToServer().
    const Port m_clientPort = 0;        // Port from c-r. It is used to make the protocol name and we will try to work with this protocol.
	const DCTLClientID m_clientID = 0;	// Uniq client Id, which we use for RPC calls.
};

}	// end namespace DCTL
