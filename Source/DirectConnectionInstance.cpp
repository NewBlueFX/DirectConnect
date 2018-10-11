/* DirectConnectionInstance.cpp
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

#include <sstream>
#include "DirectConnectionInstance.h"
#include "DirectConnectionGeneric.h"
#include "DirectConnectionTypes.h"
#include "DirectConnectionIPCSettings.h"
#include "DirectConnectionIPCClient.h"
#include <cstdlib>
#include <random>
#include <chrono>


namespace DCTL
{

namespace
{

DCTLInstanceID MakeUniqueInstanceID(void* pThis)
{
	// Construct a random generator engine from a time-based seed (obtain a seed from the system clock):
	const unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	std::default_random_engine generator(seed);

	// Take random value
	const DCTLInstanceID randomValue = generator();

	// And another crazy step to make a random value - multiply on the pointer value.
	return (reinterpret_cast<DCTLInstanceID>(pThis) * randomValue);
}

}


DirectConnectionInstance::DirectConnectionInstance(DirectConnectionIPCClient* pClient, Channel channel, bool* pbInitialized)
	// We need unique ID for client, because it will be used to make unique shared memory name.
	// Many clients can be started on different architectures (x86, x86_64) and we should
	// work stable in all cases.
	: m_instanceId(MakeUniqueInstanceID(this))
{
	bool bInitialized = false;
	NBAssert(pClient);
	if (pClient)
	{
		DCResult result = pClient->CreateInstance(m_instanceId);

		if (result == DCResult::DCERR_OK)
		{
			m_videoFrameSharedMemory.InitSharedMemory(
				Utility::MakeSharedResourceName(VIDEO_RESOURCE_NAME, m_instanceId),
				true,
				VIDEO_FRAME_RESOURCE_SIZE);

			m_audioFrameSharedMemory.InitSharedMemory(
				Utility::MakeSharedResourceName(AUDIO_RESOURCE_NAME, m_instanceId),
				true,
				AUDIO_FRAME_RESOURCE_SIZE);

            NBAssert(m_videoFrameSharedMemory.IsInited());
            NBAssert(m_audioFrameSharedMemory.IsInited());

			bInitialized = result == DCResult::DCERR_OK;

			if (bInitialized)
			{
				m_channel = channel;
				m_pClient = pClient;
				m_pClient->AddRef();
			}
		}
	}

	if (pbInitialized)
	{
		*pbInitialized = bInitialized;
	}
}

DirectConnectionInstance::~DirectConnectionInstance()
{
	if (m_pClient)
	{
		if (m_videoFrameSharedMemory.IsInited())
		{
			m_pClient->FreeSharedMemory(m_instanceId, DCTLSM_RENDER_VIDEO_FRAME);
		}

		if (m_audioFrameSharedMemory.IsInited())
		{
			m_pClient->FreeSharedMemory(m_instanceId, DCTLSM_RENDER_AUDIO_FRAME);
		}

		m_pClient->DestroyInstance(m_instanceId);

		m_pClient->Release();
		m_pClient = nullptr;
	}

	// Free own SharedMemory objects after these objects were destroyed on Standalone side.
	m_videoFrameSharedMemory.FreeSharedMemory();
	m_audioFrameSharedMemory.FreeSharedMemory();

	m_channel = 0;
}

HRESULT STDMETHODCALLTYPE DirectConnectionInstance::QueryInterface(const IID& iid, void** ppOut)
{
	CheckPointer(ppOut, E_POINTER);

	if (iid == IID_DirectConnection || iid == IID_IUnknown)
	{
		*ppOut = static_cast<IDirectConnection*>(this);
	}
    else if (iid == IID_DirectConnectionNTXExtension)
    {
		*ppOut = static_cast<IDirectConnectionNTXExtension*>(this);
    }
	else
	{
		*ppOut = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppOut)->AddRef();

	return S_OK;
}

ULONG STDMETHODCALLTYPE DirectConnectionInstance::AddRef()
{
	return static_cast<ULONG>(::InterlockedIncrement(&m_nRefCount));
}

ULONG STDMETHODCALLTYPE DirectConnectionInstance::Release()
{
	LONG refCount = static_cast<LONG>(::InterlockedDecrement(&m_nRefCount));
	if (refCount == 0)
	{
		delete this;
		return 0;
	}

	return static_cast<ULONG>(refCount);
}

DCResult DirectConnectionInstance::InitializeDirectConnection(int nWidth, int nHeight, double dFrameRate,
	double dPixelAspectRatio, bool bInterlaced, int nSampleRate, int nNumChannels, double* pdStartTime)
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->InitializeDirectConnection(m_instanceId, m_channel, nWidth,
		nHeight, dFrameRate, dPixelAspectRatio, bInterlaced, nSampleRate, nNumChannels, pdStartTime) : DCERR_ServerShutdown;
}

DCResult DirectConnectionInstance::GetVideoFrame(VideoFrameParams* frame, double dTime)
{
	NBAssert(frame);
	CheckPointer(frame, DCERR_Failed);
	NBAssert(m_pClient);
	CheckPointer(m_pClient, DCERR_ServerShutdown);

	std::lock_guard<std::recursive_mutex> lock(m_videoFrameSharedMemoryLock);

	NBAssert((frame->m_pfFormat == VPF_BGRA8) && (frame->m_nDepth == 4));	// Now only BGRA is supported.

	const unsigned int bufferSizeInBytes = static_cast<unsigned int>(NBAbs(frame->m_nHeight * frame->m_nSpan));

	// Reallocate the shared memory if it is not enough
	if (bufferSizeInBytes > m_videoFrameSharedMemory.GetCurrentSize())
	{
		// Free buffer on server side.
		m_pClient->FreeSharedMemory(m_instanceId, DCTLSM_RENDER_VIDEO_FRAME);
		m_videoFrameSharedMemory.ResizeBuffer(bufferSizeInBytes);
	}

	DCResult result = DCResult::DCERR_Failed;
	void* sharedBuffer = m_videoFrameSharedMemory.GetBuffer();
	NBAssert(sharedBuffer);
	if (sharedBuffer)
	{
		result = m_pClient->GetVideoFrame(m_instanceId, m_channel, frame, dTime);

		if (result == DCResult::DCERR_OK || result == DCERR_OK_VideoSettingsNotMatch)
		{
			memcpy(frame->m_pBuffer, sharedBuffer, NBAbs(frame->m_nHeight * frame->m_nSpan));
		}
	}

	return result;
}

DCResult DirectConnectionInstance::GetAudioFrame(AudioFrameParams* frame, double dTime)
{
	NBAssert(m_pClient);
	CheckPointer(m_pClient, DCERR_ServerShutdown);

    std::lock_guard<std::recursive_mutex> lock(m_audioFrameSharedMemoryLock);

	DCResult result = DCResult::DCERR_Failed;
	const unsigned int bufferSizeInBytes = static_cast<unsigned int>(frame->m_nChannels * frame->m_nSampleCount * sizeof(float));

	// Reallocate the shared memory if it is not enough
	if (bufferSizeInBytes > m_audioFrameSharedMemory.GetCurrentSize())
	{
		// Free buffer on server side.
		m_pClient->FreeSharedMemory(m_instanceId, DCTLSM_RENDER_AUDIO_FRAME);
		m_audioFrameSharedMemory.ResizeBuffer(bufferSizeInBytes);
	}

	float* sharedBuffer = reinterpret_cast<float*>(m_audioFrameSharedMemory.GetBuffer());
	NBAssert(sharedBuffer);
	if (sharedBuffer)
	{
		result = m_pClient->GetAudioFrame(m_instanceId, m_channel, frame, dTime);

		if (result == DCResult::DCERR_OK)
		{
			// Use frame->m_nSampleCount for the copy operation, because it can be changed.
			memcpy(frame->m_pBuffer, sharedBuffer, frame->m_nSampleCount * frame->m_nChannels * sizeof(float));
		}
	}

	return result;
}

DCResult DirectConnectionInstance::GetMetadataFrame(char** ppszData)
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->GetMetadataFrame(m_instanceId, m_channel, ppszData) : DCERR_ServerShutdown;
}

DCResult DirectConnectionInstance::FreeMetadataFrame(char* pszData)
{
	NBAssert(m_pClient);
    return m_pClient ? m_pClient->FreeMetadataFrame(pszData) : DCERR_ServerShutdown;
}

DCResult DirectConnectionInstance::SendMetadataFrame(const char* pszMetadata)
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->SendMetadataFrame(m_instanceId, m_channel, pszMetadata) : DCERR_ServerShutdown;
}

DCTLInstanceID DirectConnectionInstance::GetInstanceID()
{
	return m_instanceId;
}

DCResult DirectConnectionInstance::SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface)
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->SetHostServer(pHostServerInterface) : DCERR_ServerShutdown;
}

DCResult DirectConnectionInstance::FreeSharedMemory(DCTLSharedMemoryType type)
{
    NBAssert(m_pClient);
	return m_pClient ? m_pClient->FreeSharedMemory(m_instanceId, type) : DCERR_ServerShutdown;
}

DCTL::DCResult DirectConnectionInstance::GetVideoSettings(int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced)
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->GetVideoSettings(m_instanceId, m_channel, pnWidth, pnHeight, pdFrameRate, pdPixelAspectRatio, pbInterlaced) : DCERR_Failed;
}

DCResult DirectConnectionInstance::GetAudioSettings(int* pnSampleRate, int* pnNumChannels)
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->GetAudioSettings(m_instanceId, m_channel, pnSampleRate, pnNumChannels) : DCERR_Failed;
}

}
