//	DirectConnectionGeneric.h
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

#include <string>
#include "DirectConnectionNTXInterfaces.h"

namespace DCTL
{

namespace Utility
{

// Use this function to get name for shared resource, this function
// will be used on both sides (client and server), to be sure that resource name is the same.
// 'pszResourceName' is VIDEO_RESOURCE_NAME or AUDIO_RESOURCE_NAME.
std::string MakeSharedResourceName(const char* pszResourceName, DCTLInstanceID instanceId);
long long   GetCurrentProcessId();
std::string ProcessIdToString(long long llPID);

#ifdef _WIN32
// Only for Win RPC
DCTLVideoRenderInfoIDL ConvertDCRenderInfoToIDL(const DCTL::VideoRenderInfo& renderInfo);
DCTL::VideoRenderInfo ConvertIDLRenderInfoToDC(const DCTLVideoRenderInfoIDL& renderInfo);

// NOTE: DCTLVideoFrameParamsIDL does not contain 'm_pBuffer' variable, be careful.
// Because of that:
// DCTL::VideoFrameParams frame(0x1, 0, 0, 0, 0, VPF_BGRA8);
// DCTLVideoFrameParamsIDL frameIDL = ConvertDCFrameToIDL(frame);
// assert(frame == ConvertIDLFrameToDC(frameIDL));	// <-- this assert will show error!!!
DCTLVideoFrameParamsIDL ConvertDCFrameToIDL(const DCTL::VideoFrameParams& frame);
DCTL::VideoFrameParams ConvertIDLFrameToDC(const DCTLVideoFrameParamsIDL& frame);

DCTLVideoPixelFormatIDL ConvertDCPixelFormatToIDL(DCTL::VideoPixelFormat format);
DCTL::VideoPixelFormat ConvertIDLPixelFormatToDC(DCTLVideoPixelFormatIDL format);

#endif

}	// end namespace Utility

}	// end namespace DCTL
