/*	DirectConnectionIPCClientOSX.cpp
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

#include "DirectConnectionIPCClientOSX.h"
#include "DirectConnectionIPCInterface.h"
#include "DirectConnectionIPCSettings.h"

#include <iostream>
#include <mutex>

#include <sys/types.h>
#include <sys/wait.h>

namespace
{

// C++ wrapper for NSAutoreleasePool object.
#if !__has_feature(objc_arc)
class CAutoreleasePool
{
public:
	CAutoreleasePool()
	{
		pool = [NSAutoreleasePool new];
	}
	~CAutoreleasePool()
	{
		[pool drain];
	}
private:
	NSAutoreleasePool* pool;
};
#else
class CAutoreleasePool
{
public:
    CAutoreleasePool(){;}
};
#endif

bool isProxyValid(id proxy)
{
    if (proxy && [proxy isProxy])
    {
        NSConnection* conn = [(NSDistantObject *)proxy connectionForProxy];
        if (conn)
        {
            return [conn isValid];
        }
    }

    return false;
}

void AllocString(const char* pszIn, char** ppszOutText)
{
    if(ppszOutText)
    {
        const size_t uiInLen = (pszIn && pszIn[0]) ? strlen(pszIn) : 0;

        *ppszOutText = new char[uiInLen + 1];

        if(uiInLen)
        {
            memcpy(*ppszOutText, pszIn, uiInLen);
        }

        (*ppszOutText)[uiInLen] = 0;
    }
}

void FreeString(const char* pszText)
{
    if (pszText)
    {
        delete [] pszText;
    }
}

}

@interface NewBlueDirectConnectionClientV3 : NSObject <NewBlueDirectConnectionClientProtocolV3>
{
    int m_lastEvent;
    DCTL::IDirectConnectionNTXHostServer* m_pHostServerInterface;
}

@end


@implementation NewBlueDirectConnectionClientV3

+ (NSString*) makeServerName:(int)port
{
    return [[NSString alloc] initWithFormat:@"%s%d", DCTL::SERVER_NAME, port];
}

- (id) init
{
    if (self = [super init])
    {
        m_lastEvent = 0;
        m_pHostServerInterface = nullptr;
    }

    return self;
}

- (void) dealloc
{
    NBAssert(m_pHostServerInterface == nullptr);    // Should be released before!
    if (m_pHostServerInterface)
    {
        m_pHostServerInterface->Release();
        m_pHostServerInterface = nullptr;
    }
#if !__has_feature(objc_arc)
    [super dealloc];
#endif
}

- (oneway void) serverEvent:(in int)event
{
    // This is not used now.
    m_lastEvent = event;
}

- (pid_t) getProcessId
{
    return getpid();
}


- (bool) getFrameFromHostForClientId:(in DCTLClientID)clientId
						  instanceId:(in DCTLInstanceID)instanceId
                      videoFrameType:(in int)frameType
						  renderInfo:(inout DCTL::VideoRenderInfo*)pRenderInfo
						       frame:(inout DCTL::VideoFrameParams*)pFrame
{
    bool bResult = false;
    NBAssert(m_pHostServerInterface);   // Should be set already!
    if (m_pHostServerInterface)
    {
        bResult = (m_pHostServerInterface->GetHostFrame(instanceId, frameType, pRenderInfo, pFrame) == S_OK);

        if (pFrame)
        {
            // Do not send any pointer, IPC can change it. Also this can lead to a crash.
            pFrame->m_pBuffer = nullptr;
        }
    }

    return bResult;
}

- (void) setHostServerInterface:(DCTL::IDirectConnectionNTXHostServer*)pHostServerInterface
{
    if (pHostServerInterface != m_pHostServerInterface)
    {
        if (m_pHostServerInterface)
        {
            m_pHostServerInterface->Release();
        }

        m_pHostServerInterface = pHostServerInterface;

        if (m_pHostServerInterface)
        {
            m_pHostServerInterface->AddRef();
        }
    }
}

@end

namespace DCTL
{

// Private structure to hide Obj-C objects from c++ compiler.
struct DirectConnectionIPCClientOSXPrivateData
{
    id /*<NewBlueDirectConnectionServerProtocolV4>*/ serverProxy = nil;
    NewBlueDirectConnectionClientV3* /*NewBlueDirectConnectionClientV3*/ clientObject = nil;
};

// Private structure to wrap connection and instance proxies
struct DirectConnectionInstanceDataOSX
{
    id /*NewBlueDirectConnectionInstanceProtocolV2*/ instance = nil;
    id connection = nil;
    NSString* connectionId = nil;
};


DirectConnectionIPCClientOSX::DirectConnectionIPCClientOSX(Port serverPortOnStandaloneSide, DCTLClientID clientID, bool* pbInitialized)
    : m_opaqueData(new DirectConnectionIPCClientOSXPrivateData)
    , m_clientPort(serverPortOnStandaloneSide)
    , m_clientID(clientID)
{
    CAutoreleasePool pool;
    NSString* serverName = [NewBlueDirectConnectionClientV3 makeServerName:serverPortOnStandaloneSide];

    NSDistantObject* proxy = [NSConnection rootProxyForConnectionWithRegisteredName:serverName host:nil];

    if (isProxyValid(proxy))
    {
        [proxy setProtocolForProxy:@protocol(NewBlueDirectConnectionServerProtocolV4)];
#if !__has_feature(objc_arc)
        m_opaqueData->serverProxy = [proxy retain];
#else
        m_opaqueData->serverProxy = proxy;
#endif
    }

    if (pbInitialized)
    {
        *pbInitialized = m_opaqueData->serverProxy != nil;
    }
#if !__has_feature(objc_arc)
    [serverName release];
#endif
}

DirectConnectionIPCClientOSX::~DirectConnectionIPCClientOSX()
{
    CAutoreleasePool pool;

    NBAssertStr(m_instances.empty(), "Not all instances were removed! Check your code!");

    while (!m_instances.empty())
    {
        DestroyInstance(m_instances.begin()->first);
    }

    if (isProxyValid(m_opaqueData->serverProxy) && m_opaqueData->clientObject)
    {
        // Try a gentle shutdown.
        @try
        {
            [m_opaqueData->serverProxy clientDisconnected:m_clientID];
        }
        @catch (NSException* e)
        {
            NBAssert(!"stopServer");
        }
    }

    if (m_opaqueData->serverProxy)
    {
#if !__has_feature(objc_arc)
        [m_opaqueData->serverProxy release];
#endif
        m_opaqueData->serverProxy = nil;
    }

    if (m_opaqueData->clientObject)
    {
        [m_opaqueData->clientObject setHostServerInterface:nullptr];
#if !__has_feature(objc_arc)
        [m_opaqueData->clientObject release];
#endif
        m_opaqueData->clientObject = nil;
    }

    delete m_opaqueData;
    m_opaqueData = nullptr;
}

bool DirectConnectionIPCClientOSX::ConnectToServer()
{
    CAutoreleasePool pool;
    bool bResult = false;

    NBAssert(m_opaqueData->clientObject == nil);	// Do not use DirectConnectionIPCClient::ConnectToServer() twice.

    if (isProxyValid(m_opaqueData->serverProxy))
    {
        if (!m_opaqueData->clientObject)
        {
            @try
            {
                m_processID = [m_opaqueData->serverProxy getProcessId];
                NBAssert(m_processID);

                if (m_processID)
                {
                    m_opaqueData->clientObject = [NewBlueDirectConnectionClientV3 new];

                    [m_opaqueData->serverProxy clientConnected:m_opaqueData->clientObject clientId:m_clientID];

                    bResult = true;
                }
            }
            @catch (NSException* e)
            {
                NBAssert(!"ConnectToServer, getProcessId or clientConnected fails.");
            }
        }
        else
        {
            bResult = true;
        }
    }

    return bResult;
}

Port DirectConnectionIPCClientOSX::GetClientPort() const
{
    return m_clientPort;
}

DCTLProcessID DirectConnectionIPCClientOSX::GetProcessID() const
{
    return m_processID;
}

bool DirectConnectionIPCClientOSX::IsProcessRunning() const
{
    bool bResult = false;

    if (m_processID)
    {
        const int sig = 0;
		// If sig is 0, then no signal is sent, but existence and permission
		// checks are still performed; this can be used to check for the
		// existence of a process ID or process group ID that the caller is
		// permitted to signal.
		const int result = ::kill(m_processID, sig);

        if (result != -1)
        {
            // process is still running
            bResult = true;
        }
    }

    return bResult;
}

DCResult DirectConnectionIPCClientOSX::CreateInstance(DCTLInstanceID instanceId)
{
    CAutoreleasePool pool;
    DCResult result = DCResult::DCERR_Failed;

    if (isProxyValid(m_opaqueData->serverProxy) && m_opaqueData->clientObject)
    {
        @try
        {
            NSString* connectionId = [m_opaqueData->serverProxy createConnectionIdForInstance];

            id newConnectionProxy = [NSConnection rootProxyForConnectionWithRegisteredName:connectionId host:nil];
            NBAssert(newConnectionProxy);
            if (newConnectionProxy)
            {
                [newConnectionProxy setProtocolForProxy:@protocol(NewBlueDirectConnectionInstanceProtocolV2)];

                // Create instance on server connection
                id<NewBlueDirectConnectionInstanceProtocolV2> instanceProxy = [m_opaqueData->serverProxy createInstanceForConnectionId:connectionId
                                                                                                                              clientId:m_clientID
                                                                                                                            instanceId:instanceId];
                NBAssert(instanceProxy);
                if (instanceProxy)
                {
                    SharedInstanceData pSharedInstanceData = std::make_shared<DirectConnectionInstanceDataOSX>();

#if !__has_feature(objc_arc)
                    // We keep our own reference on this object,
                    // otherwise if the server will close connection, we will keep the dead reference.
                    [(NSObject*)instanceProxy retain];
#endif

                    pSharedInstanceData->instance = instanceProxy;
                    pSharedInstanceData->connectionId = [connectionId copy];
#if !__has_feature(objc_arc)
                    pSharedInstanceData->connection = [newConnectionProxy retain];
#else
                    pSharedInstanceData->connection = newConnectionProxy;
#endif

                    // And push this new shared instance data to our map
                    std::lock_guard<std::recursive_mutex> lock(m_instancesLock);

                    m_instances[instanceId] = pSharedInstanceData;

                    result = DCResult::DCERR_OK;
                }
            }
        }
        @catch (NSException* e)
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::DestroyInstance(DCTLInstanceID instanceId)
{
    DCResult result = DCResult::DCERR_Failed;

    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pInstanceData.get())
    {
        // Free all resources that this structure contains.
        result = DisconnectInstance(pInstanceData, instanceId);

        // And remove the instance from map
        std::lock_guard<std::recursive_mutex> lock(m_instancesLock);
        SharedInstanceDataMap::iterator iter = m_instances.find(instanceId);
        NBAssert(iter != m_instances.end());
        if (iter != m_instances.end())
        {
            m_instances.erase(iter);
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::InitializeDirectConnection(DCTLInstanceID instanceId, int dcChannel,
                                    int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio, bool bInterlaced,
                                    int nSampleRate, int nNumChannels, double* pdStartTime)
{
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            @try
            {
                result = static_cast<DCResult>([pInstanceData->instance initializeDirectConnectionForClientId:m_clientID
                                                                                                   instanceId:instanceId
                                                                                                      channel:dcChannel
                                                                                                        width:nWidth
                                                                                                       height:nHeight
                                                                                                    frameRate:dFrameRate
                                                                                             pixelAspectRatio:dPixelAspectRatio
                                                                                                   interlaced:bInterlaced
                                                                                                   sampleRate:nSampleRate
                                                                                                  numChannels:nNumChannels
                                                                                                    startTime:pdStartTime]);
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::GetVideoSettings(DCTLInstanceID instanceId, int dcChannel, int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced)
{
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            @try
            {
                result = static_cast<DCResult>([pInstanceData->instance getVideoSettingsForClientId:m_clientID
                                                                                         instanceId:instanceId
                                                                                            channel:dcChannel
                                                                                              width:pnWidth
                                                                                             height:pnHeight
                                                                                          framerate:pdFrameRate
                                                                                   pixelAspectRatio:pdPixelAspectRatio
                                                                                         interlaced:pbInterlaced]);
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::GetAudioSettings(DCTLInstanceID instanceId, int dcChannel, int* pnSampleRate, int* pnNumChannels)
{
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            @try
            {
                result = static_cast<DCResult>([pInstanceData->instance getAudioSettingsForClientId:m_clientID
                                                                                         instanceId:instanceId
                                                                                            channel:dcChannel
                                                                                         sampleRate:pnSampleRate
                                                                                        numChannels:pnNumChannels]);
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::GetVideoFrame(DCTLInstanceID instanceId, int dcChannel, VideoFrameParams* pFrame, double dTime)
{
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pFrame && pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            // Do not send any pointer, IPC can change it. Also this is not used on another side. This can lead to a crash.
            unsigned char* pBuffer = pFrame->m_pBuffer;
            pFrame->m_pBuffer = nullptr;

            @try
            {
                result = static_cast<DCResult>([pInstanceData->instance getVideoFrameForClientId:m_clientID
                                                                                      instanceId:instanceId
                                                                                         channel:dcChannel
                                                                                           frame:pFrame
                                                                                            time:dTime]);
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }

            pFrame->m_pBuffer = pBuffer;
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::GetAudioFrame(DCTLInstanceID instanceId, int dcChannel, AudioFrameParams* pFrame, double dTime)
{
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            // Do not send any pointer, IPC can change it. Also this is not used on another side. This can lead to a crash.
            float* pBuffer = pFrame->m_pBuffer;
            pFrame->m_pBuffer = nullptr;

            @try
            {
                result = static_cast<DCResult>([pInstanceData->instance getAudioFrameForClientId:m_clientID
                                                                                      instanceId:instanceId
                                                                                         channel:dcChannel
                                                                                           frame:pFrame
                                                                                            time:dTime]);
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }

            pFrame->m_pBuffer = pBuffer;
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::GetMetadataFrame(DCTLInstanceID instanceId, int dcChannel, char** ppszBuffer)
{
    CAutoreleasePool pool;
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            @try
            {
                NSString * outData = nil;

                result = static_cast<DCResult>([pInstanceData->instance getMetadataFrameForClientId:m_clientID
                                                                                         instanceId:instanceId
                                                                                            channel:dcChannel
                                                                                           metadata:&outData]);

                if (outData)
                {
                    AllocString([outData UTF8String], ppszBuffer);
                }
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::FreeMetadataFrame(char* pszData)
{
    FreeString(pszData);

    return (pszData == nullptr) ? DCResult::DCERR_Failed : DCResult::DCERR_OK;
}

DCResult DirectConnectionIPCClientOSX::SendMetadataFrame(DCTLInstanceID instanceId, int dcChannel, const char* pszMetadata)
{
    CAutoreleasePool pool;
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pszMetadata && pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            NSString * metaDataString = [[NSString alloc] initWithUTF8String:pszMetadata];

            @try
            {
                result = static_cast<DCResult>([pInstanceData->instance sendMetadataFrameForClientId:m_clientID
                                                                                          instanceId:instanceId
                                                                                             channel:dcChannel
                                                                                            metadata:metaDataString]);
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }
#if !__has_feature(objc_arc)
            [metaDataString release];
#endif
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::FreeSharedMemory(DCTLInstanceID instanceId, DCTLSharedMemoryType type)
{
    DCResult result = DCResult::DCERR_Failed;
    SharedInstanceData pInstanceData = GetInstanceData(instanceId);
    NBAssert(pInstanceData.get());
    if (pInstanceData.get())
    {
        if (isProxyValid(pInstanceData->instance))
        {
            @try
            {
                result = static_cast<DCResult>([pInstanceData->instance freeSharedMemoryForClientId:m_clientID
                                                                                         instanceId:instanceId
                                                                                               type:type]);
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }
        }
        else
        {
            result = DCERR_ServerShutdown;
        }
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface)
{
    DCResult result = DCERR_OK;

    if (m_opaqueData->clientObject)
    {
        [m_opaqueData->clientObject setHostServerInterface:pHostServerInterface];
    }
    else
    {
        result = DCERR_ServerShutdown;
    }

    return result;
}

DCResult DirectConnectionIPCClientOSX::SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata)
{
    CAutoreleasePool pool;
    DCResult result = DCResult::DCERR_Failed;

    if (isProxyValid(m_opaqueData->serverProxy))
    {
        NSString * metaDataString = [[NSString alloc] initWithUTF8String:pszMetadata];

        @try
        {
            result = static_cast<DCResult>([m_opaqueData->serverProxy sendGlobalMetadataFrameChannel:dcChannel
                                                                                            metadata:metaDataString]);
        }
        @catch (NSException* e)
        {
            result = DCERR_ServerShutdown;
        }
#if !__has_feature(objc_arc)
        [metaDataString release];
#endif
    }

    return result;
}

DirectConnectionIPCClientOSX::SharedInstanceData DirectConnectionIPCClientOSX::GetInstanceData(DCTLInstanceID instanceId) const
{
	std::lock_guard<std::recursive_mutex> lock(m_instancesLock);

    auto iterator = m_instances.find(instanceId);

    if (iterator != m_instances.end())
    {
        return iterator->second;
    }

    return SharedInstanceData();
}

DCResult DirectConnectionIPCClientOSX::DisconnectInstance(const SharedInstanceData& pInstanceData, DCTLInstanceID instanceId) const
{
    DCResult result = DCResult::DCERR_OK;

    if (pInstanceData->instance)
    {
        if (isProxyValid(m_opaqueData->serverProxy))
        {
            @try
            {
                // Close connection on server connection;
                [m_opaqueData->serverProxy destroyInstanceByClientId:m_clientID
                                                          instanceId:instanceId];
            }
            @catch (NSException* e)
            {
                result = DCERR_ServerShutdown;
            }
        }

#if !__has_feature(objc_arc)
        [pInstanceData->connection release];
        [pInstanceData->instance release];
        [pInstanceData->connectionId release];
#endif

        pInstanceData->connection = nil;
        pInstanceData->instance = nil;
        pInstanceData->connectionId = nil;
    }

    return result;
}

}   // end namespace DCTL
