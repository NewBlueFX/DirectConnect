/*	DirectConnectionIPCSettings.h
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

namespace DCTL
{


#ifdef _WIN32

// Platform dependent values:

// DirectConnection TitlerLive RPC protocol.
static const char* RPC_DC_PROTOCOL              = "ncalrpc";

// DirectConnection TitlerLive process RPC port.
static const int RPC_DC_PORT                    = 19175;

// Host process RPC port.
static const int RPC_DC_START_HOST_PORT         = 19127;

// Common values for each platform:

// Name of shared memory resource for video frame.
static const char* VIDEO_RESOURCE_NAME          = "Local\\NewBlue.DirectConnection.TitlerLive.Frame";
// Name of shared memory resource for audio frame.
static const char* AUDIO_RESOURCE_NAME          = "Local\\NewBlue.DirectConnection.TitlerLive.AudioFrame";
// Name of shared memory resource for host frame #1.
static const char* HOST_RESOURCE1_NAME			= "Local\\NewBlue.DirectConnection.TitlerLive.HostFrame1";
// Name of shared memory resource for host frame #2.
static const char* HOST_RESOURCE2_NAME			= "Local\\NewBlue.DirectConnection.TitlerLive.HostFrame2";

#elif __APPLE__

// Platform dependent values:
static const char* SERVER_NAME                  = "com.newblue.directconnection.server.";

// Common values for each platform:

// Name of shared memory resource for video frame.
static const char* VIDEO_RESOURCE_NAME          = "NewBlue.DirectConnection.TitlerLive.Frame";
// Name of shared memory resource for audio frame.
static const char* AUDIO_RESOURCE_NAME          = "NewBlue.DirectConnection.TitlerLive.AudioFrame";
// Name of shared memory resource for host frame #1.
static const char* HOST_RESOURCE1_NAME			= "NewBlue.DirectConnection.TitlerLive.HostFrame1";
// Name of shared memory resource for host frame #2.
static const char* HOST_RESOURCE2_NAME			= "NewBlue.DirectConnection.TitlerLive.HostFrame2";

#else
#   error Unsupported platform
#endif

// Size for share memory for video frame. Use FULL HD byte.
static const int VIDEO_FRAME_RESOURCE_SIZE      = 1920 * 1080 * 4;
// Size for share memory for audio frame. Reserve size for 1 second (2 channels, 48000Hz).
static const int AUDIO_FRAME_RESOURCE_SIZE      = 48000 * 2 * sizeof(float);
// Size for share memory for host frame. Use FULL HD byte.
static const int HOST_FRAME_RESOURCE_SIZE       = 1920 * 1080 * 4;

}	// end namespace DCTL
