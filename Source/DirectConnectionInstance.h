//	DirectConnectionInstance.h
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
#include "DirectConnectionNTXInterfaces.h"
#include "SharedMemory.h"
#include "DirectConnectionCrossProcessObjects.h"
#include <mutex>


namespace DCTL
{

// Forward declaration
class DirectConnectionIPCClient;

// Implementation of IDirectConnection interface that based on Inter-Process Communication.
// Also this class creates own SharedMemory objects (video and audio) to interact with the Standalone (server) side.
class DirectConnectionInstance
	: public IDirectConnection
    , public IDirectConnectionNTXExtension
{
	DirectConnectionInstance(const DirectConnectionInstance &str);	// not implemented
	DirectConnectionInstance& operator=(const DirectConnectionInstance &str);	// not implemented

public:

    // c-r initializes DirectConnectionInstance object completely. If the initialization will fail,
    // you could know it using 'pbInitialized'.
    // This object is created with one reference.
    // DirectConnectionInstance will keep one reference of DirectConnectionIPCClient if the initialization is successful.
	DirectConnectionInstance(DirectConnectionIPCClient* pConnector, Channel channel, bool* pbInitialized);

	// For IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& iid, void** ppOut) override;
	virtual ULONG STDMETHODCALLTYPE AddRef() override;
	virtual ULONG STDMETHODCALLTYPE Release() override;

	// IDirectConnection
	virtual DCResult InitializeDirectConnection(int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio, bool bInterlaced,
		int nSampleRate, int nNumChannels, double* pdStartTime) override;
	virtual DCResult GetVideoSettings(int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced) override;
    virtual DCResult GetAudioSettings(int* pnSampleRate, int* pnNumChannels) override;
	virtual DCResult GetVideoFrame(VideoFrameParams* pFrame, double dTime) override;
	virtual DCResult GetAudioFrame(AudioFrameParams* pFrame, double dTime) override;
	virtual DCResult GetMetadataFrame(char** ppszData) override;
	virtual DCResult FreeMetadataFrame(char* pszData) override;
	virtual DCResult SendMetadataFrame(const char* pszMetadata) override;

    // IDirectConnectionNTXExtension
	virtual DCTLInstanceID GetInstanceID() override;
	virtual DCResult SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface) override;
    virtual DCResult FreeSharedMemory(DCTLSharedMemoryType type) override;

protected:
	// Use Release() instead of direct removing.
	virtual ~DirectConnectionInstance();

	const DCTLInstanceID m_instanceId = 0;              // ID of this DC instance. This is unique ID and is used to make names for shared memory objects.

	CSharedMemory m_videoFrameSharedMemory;             // Shared memory for video frame
	CSharedMemory m_audioFrameSharedMemory;             // Shared memory for audio frame

    // Mutexes for video and audio memory objects.
    // A mutex is enough (not semaphore) for our task, because with this memory
    // will work only this DirectConnectionInstance. The Standalone (server) side will work
    // with this memory only during GetVideoFrame()/GetAudioFrame() calls.
    // So it looks that is safe enough to use a mutex.
	std::recursive_mutex m_videoFrameSharedMemoryLock;	// Mutex for video shared memory
	std::recursive_mutex m_audioFrameSharedMemoryLock;	// Mutex for audio shared memory

	DirectConnectionIPCClient* m_pClient = nullptr;     // DirectConnectionInstance will keep one reference of DirectConnectionIPCClient if the initialization is successful.
	Channel m_channel = 0;                              // Current channel for this connection
	LONG m_nRefCount = 1;                               // Ref count for AddRef/Release.
};

}	// end namespace DCTL
