/*	DirectConnectionIPCInterface.h
 
	NewBlue Confidential & Proprietary.
	This work contains valuable confidential and proprietary
	information. Disclosure, use or reproduction outside of
	NewBlue, Inc. is prohibited except as
	authorized in writing. This work is protected by
	the laws of the United States and other countries. In the
	event of publication, the following notice shall apply:
	(c) Copyright 2007 NewBlue, Inc. All Rights Reserved.
 */

#pragma once

#include "DirectConnectionInterface.h"
#include "DirectConnectionNTXInterfaces.h"



/**
 * Obj-C equivalent of IDirectConnection. */
@protocol NewBlueDirectConnectionInstanceProtocolV2 /*<NSObject>*/

/**
 * This method will initialize instance with requested parameters. Also it will open Main UI.
 * dcChannel is number of channel which will be used to receive a video stream. */
- (int) initializeDirectConnectionForClientId:(in DCTLClientID)clientId
                                   instanceId:(in DCTLInstanceID)instanceId
                                      channel:(in int)dcChannel
                                        width:(in int)videoWidth
                                       height:(in int)videoHeight
                                    frameRate:(in double)videoFrameRate
                             pixelAspectRatio:(in double)videoPixelAspectRatio
                                   interlaced:(in bool)videoIterlaced
                                   sampleRate:(in int)audioSampleRate
                                  numChannels:(in int)audioNumChannels
                                    startTime:(out double*)outStartTime;

/**
 * Render method.
 * Returns 'true' when new frame is ready, else the shared memory will contain previous frame.
 * Set '0' for reserved. It is not used for now. */
- (int) getVideoFrameForClientId:(in DCTLClientID)clientId
                      instanceId:(in DCTLInstanceID)instanceId
                         channel:(in int)dcChannel
                           frame:(inout DCTL::VideoFrameParams*)inoutVideoFrame
                            time:(in double)timestamp;

/**
 * Get metadata, if it exists. */
- (int) getMetadataFrameForClientId:(in DCTLClientID)clientId
                         instanceId:(in DCTLInstanceID)instanceId
                            channel:(in int)dcChannel
                           metadata:(bycopy out NSString **)outData;

/**
 * To send metadata to channel */
- (int) sendMetadataFrameForClientId:(in DCTLClientID)clientId
                          instanceId:(in DCTLInstanceID)instanceId
                             channel:(in int)dcChannel
                            metadata:(in NSString*)data;

/**
 * Render audio frame.
 * pBuffer - buffer to return audio data
 * AudioFrameParams.sampleCount - number of samples to return. Buffer size must be at least sampleCount * numChannels long.
 * (numChannels should be set in InitializeDirectConnection call) */
- (int) getAudioFrameForClientId:(in DCTLClientID)clientId
                      instanceId:(in DCTLInstanceID)instanceId
                         channel:(in int)dcChannel
                           frame:(inout DCTL::AudioFrameParams*)inoutAudioFrame
                            time:(in double)timestamp;

/**
 * Returns actual video settings. */
- (int) getVideoSettingsForClientId:(in DCTLClientID)clientId
                         instanceId:(in DCTLInstanceID)instanceId
                            channel:(in int)dcChannel
                              width:(out int*)outWidth
                             height:(out int*)outHeight
                          framerate:(out double*)outFrameRate
                   pixelAspectRatio:(out double*)outPAR
                         interlaced:(out bool*)outIterlaced;

/**
 * Returns actual audio settings. */
- (int) getAudioSettingsForClientId:(in DCTLClientID)clientId
                         instanceId:(in DCTLInstanceID)instanceId
                            channel:(in int)dcChannel
                         sampleRate:(out int*)outSampleRate
                        numChannels:(out int*)outNumChannels;


/**
 * Free shared memory object. */
- (int) freeSharedMemoryForClientId:(in DCTLClientID)clientId
                         instanceId:(in DCTLInstanceID)instanceId
                               type:(in DCTLSharedMemoryType)type;

@end


/**
 * Protocol for messages from server to client. */
@protocol NewBlueDirectConnectionClientProtocolV3

/**
 * Reserved. */
- (oneway void)serverEvent:(in int)reserved;

/**
 * Get process id where client is launched. */

- (pid_t) getProcessId;

/**
 * Retrieve frame from NLE. */
- (bool) getFrameFromHostForClientId:(in DCTLClientID)clientId
						  instanceId:(in DCTLInstanceID)instanceId
                      videoFrameType:(in int)frameType
						  renderInfo:(inout DCTL::VideoRenderInfo*)pRenderInfo
						       frame:(inout DCTL::VideoFrameParams*)pFrame;

@end


/**
 * Protocol for messages from client to server. */
@protocol NewBlueDirectConnectionServerProtocolV4 /*<NSObject>*/

/**
 * Notify server that this client is connected. */
- (void) clientConnected:(byref in id<NewBlueDirectConnectionClientProtocolV3>)client
				clientId:(in DCTLClientID)clientId;

/**
 * Notify server that this client is disconnected. */
- (void) clientDisconnected:(in DCTLClientID)clientId;

/**
 * Create new NSConnection object and run in new thread. Use the returned string for createInstanceForConnectionId function. */
- (NSString*) createConnectionIdForInstance;

/**
 * Create new instance for this client. */
- (byref id<NewBlueDirectConnectionInstanceProtocolV2>)createInstanceForConnectionId:(in NSString*)connectionId
                                                                            clientId:(in DCTLClientID)clientId
                                                                          instanceId:(in DCTLInstanceID)instanceId;

/**
 * Destroy an instance for given connection and client. */
- (void) destroyInstanceByClientId:(in DCTLClientID)clientId
                        instanceId:(in DCTLInstanceID)instanceId;

/**
 * Get process id where server is launched. */
- (pid_t) getProcessId;

/**
 * To send metadata to any channel */
- (int) sendGlobalMetadataFrameChannel:(in int)dcChannel
                              metadata:(in NSString*)data;

@end






