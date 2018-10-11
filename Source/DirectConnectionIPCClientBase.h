/*	DirectConnectionIPCClientBase.h
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


namespace DCTL
{

// Forward declaration
__interface IDirectConnectionNTXHostServer;

// Base interface for IPC client
class DirectConnectionIPCClientBase
{
public:

	virtual ~DirectConnectionIPCClientBase() {}

	// Connect to server with given port.
	virtual bool ConnectToServer() = 0;

	// Get the client port on which it was initialized
	virtual Port GetClientPort() const = 0;

	// Process id with which we connected. This is valid after the ConnectToServer() call.
	virtual DCTLProcessID GetProcessID() const = 0;

	// Check if process is running right now. The result of this call is valid after the ConnectToServer() call.
	virtual bool IsProcessRunning() const = 0;

    // Calls to IPC engine to create/remove instance.
	virtual DCResult CreateInstance(DCTLInstanceID instanceId) = 0;
	virtual DCResult DestroyInstance(DCTLInstanceID instanceId) = 0;
	// Calls "client to server" for DirectConnection instance
	virtual DCResult InitializeDirectConnection(DCTLInstanceID instanceId, int dcChannel,
		int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio, bool bInterlaced,
		int nSampleRate, int nNumChannels, double* pdStartTime) = 0;
	virtual DCResult GetVideoSettings(DCTLInstanceID instanceId, int dcChannel, int* pnWidth, int* pnHeight,
		double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced) = 0;
	virtual DCResult GetAudioSettings(DCTLInstanceID instanceId, int dcChannel, int* pnSampleRate, int* pnNumChannels) = 0;
	virtual DCResult GetVideoFrame(DCTLInstanceID instanceId, int dcChannel, VideoFrameParams* pFrame, double dTime) = 0;
	virtual DCResult GetAudioFrame(DCTLInstanceID instanceId, int dcChannel, AudioFrameParams* pFrame, double dTime) = 0;
	virtual DCResult GetMetadataFrame(DCTLInstanceID instanceId, int dcChannel, char** ppszBuffer) = 0;
	virtual DCResult FreeMetadataFrame(char* pszData) = 0;
	virtual DCResult SendMetadataFrame(DCTLInstanceID instanceId, int dcChannel, const char* pszMetadata) = 0;
	virtual DCResult FreeSharedMemory(DCTLInstanceID instanceId, DCTLSharedMemoryType type) = 0;
	virtual DCResult SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface) = 0;
	virtual DCResult SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata) = 0;
};

}	// end namespace DCTL
