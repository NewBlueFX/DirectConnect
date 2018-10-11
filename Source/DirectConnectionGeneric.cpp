// DirectConnectionGeneric.cpp
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

#include "DirectConnectionGeneric.h"
#include <iosfwd>
#include <sstream>


std::string DCTL::Utility::MakeSharedResourceName(const char* pszResourceName, DCTLInstanceID instanceId)
{
	std::stringstream ssResult;

	ssResult << pszResourceName;
	ssResult << instanceId;

	return ssResult.str();
}

long long DCTL::Utility::GetCurrentProcessId()
{
	long long nPid = -1;
#ifdef _WIN32
	nPid = ::GetCurrentProcessId();
#else
	nPid = getpid();
#endif
	return nPid;
}

std::string DCTL::Utility::ProcessIdToString(long long llPID)
{
    if(llPID >= 0)
        return std::to_string(llPID);
    else
        return std::string();
}

#ifdef _WIN32
// Only for Win RPC
DCTLVideoRenderInfoIDL DCTL::Utility::ConvertDCRenderInfoToIDL(const DCTL::VideoRenderInfo& srcInfo)
{
	DCTLVideoRenderInfoIDL resultInfo = {};

	resultInfo.m_lValidFlags	= srcInfo.m_nValidFlags;
	resultInfo.m_fProgress		= srcInfo.m_fProgress;
	resultInfo.m_fTime			= srcInfo.m_fTime;
	resultInfo.m_fDuration		= srcInfo.m_fDuration;
	resultInfo.m_fFramesPerSec	= srcInfo.m_fFramesPerSec;
	resultInfo.m_fAspectRatio	= srcInfo.m_fAspectRatio;
	resultInfo.m_lFrame			= srcInfo.m_nFrame;
	resultInfo.m_nField			= srcInfo.m_nField;
	resultInfo.m_fScale			= srcInfo.m_fScale;

	return resultInfo;
}

DCTL::VideoRenderInfo DCTL::Utility::ConvertIDLRenderInfoToDC(const DCTLVideoRenderInfoIDL& srcInfo)
{
	DCTL::VideoRenderInfo resultInfo;

	resultInfo.m_nValidFlags	= srcInfo.m_lValidFlags;
	resultInfo.m_fProgress		= srcInfo.m_fProgress;
	resultInfo.m_fTime			= srcInfo.m_fTime;
	resultInfo.m_fDuration		= srcInfo.m_fDuration;
	resultInfo.m_fFramesPerSec	= srcInfo.m_fFramesPerSec;
	resultInfo.m_fAspectRatio	= srcInfo.m_fAspectRatio;
	resultInfo.m_nFrame			= srcInfo.m_lFrame;
	resultInfo.m_nField			= srcInfo.m_nField;
	resultInfo.m_fScale			= srcInfo.m_fScale;

	return resultInfo;
}

DCTLVideoFrameParamsIDL DCTL::Utility::ConvertDCFrameToIDL(const DCTL::VideoFrameParams& frame)
{
	DCTLVideoFrameParamsIDL resultFrame = {};

	resultFrame.m_nWidth		= frame.m_nWidth;
	resultFrame.m_nHeight		= frame.m_nHeight;
	resultFrame.m_nSpan			= frame.m_nSpan;
	resultFrame.m_nDepth		= frame.m_nDepth;
	resultFrame.m_nX			= frame.m_nX;
	resultFrame.m_nY			= frame.m_nY;
	resultFrame.m_pfFormat		= ConvertDCPixelFormatToIDL(frame.m_pfFormat);
	resultFrame.m_bFlipped		= frame.m_bFlipped;
	resultFrame.m_bPremultAlpha	= frame.m_bPremultAlpha;

	return resultFrame;
}

DCTL::VideoFrameParams DCTL::Utility::ConvertIDLFrameToDC(const DCTLVideoFrameParamsIDL& frame)
{
	DCTL::VideoFrameParams resultFrame;

	resultFrame.m_nWidth		= frame.m_nWidth;
	resultFrame.m_nHeight		= frame.m_nHeight;
	resultFrame.m_nSpan			= frame.m_nSpan;
	resultFrame.m_nDepth		= frame.m_nDepth;
	resultFrame.m_nX			= frame.m_nX;
	resultFrame.m_nY			= frame.m_nY;
	resultFrame.m_pfFormat		= ConvertIDLPixelFormatToDC(frame.m_pfFormat);
	resultFrame.m_bFlipped		= frame.m_bFlipped ? true : false;
	resultFrame.m_bPremultAlpha	= frame.m_bPremultAlpha ? true : false;

	return resultFrame;
}

DCTLVideoPixelFormatIDL DCTL::Utility::ConvertDCPixelFormatToIDL(DCTL::VideoPixelFormat format)
{
	switch (format)
	{
	case DCTL::VPF_BGRA8:		return IDL_DCTLPF_BGRA8;
	case DCTL::VPF_ARGB8:		return IDL_DCTLPF_ARGB8;
	case DCTL::VPF_YUVA8:		return IDL_DCTLPF_YUVA8;
	case DCTL::VPF_BGRA32:		return IDL_DCTLPF_BGRA32;
	case DCTL::VPF_ARGB32:		return IDL_DCTLPF_ARGB32;
	case DCTL::VPF_YUVA32:		return IDL_DCTLPF_YUVA32;
	case DCTL::VPF_GRAY8:		return IDL_DCTLPF_GRAY8;
	case DCTL::VPF_GRAY32:		return IDL_DCTLPF_GRAY32;
	case DCTL::VPF_BGRA16:		return IDL_DCTLPF_BGRA16;
	case DCTL::VPF_ARGB16:		return IDL_DCTLPF_ARGB16;
	case DCTL::VPF_GRAY16:		return IDL_DCTLPF_GRAY16;
	case DCTL::VPF_YUY2:		return IDL_DCTLPF_YUY2;
	case DCTL::VPF_GRAY16U:		return IDL_DCTLPF_GRAY16U;
	case DCTL::VPF_YUVA_R408:	return IDL_DCTLPF_YUVA_R408;
	case DCTL::VPF_YUVA_R4FL:	return IDL_DCTLPF_YUVA_R4FL;
	case DCTL::VPF_RGBA8:		return IDL_DCTLPF_RGBA8;
	case DCTL::VPF_RGBA32:		return IDL_DCTLPF_RGBA32;
	}

	NBAssert(0);
	return static_cast<DCTLVideoPixelFormatIDL>(format);
}

DCTL::VideoPixelFormat DCTL::Utility::ConvertIDLPixelFormatToDC(DCTLVideoPixelFormatIDL format)
{
	switch (format)
	{
	case IDL_DCTLPF_BGRA8:		return DCTL::VPF_BGRA8;
	case IDL_DCTLPF_ARGB8:		return DCTL::VPF_ARGB8;
	case IDL_DCTLPF_YUVA8:		return DCTL::VPF_YUVA8;
	case IDL_DCTLPF_BGRA32:		return DCTL::VPF_BGRA32;
	case IDL_DCTLPF_ARGB32:		return DCTL::VPF_ARGB32;
	case IDL_DCTLPF_YUVA32:		return DCTL::VPF_YUVA32;
	case IDL_DCTLPF_GRAY8:		return DCTL::VPF_GRAY8;
	case IDL_DCTLPF_GRAY32:		return DCTL::VPF_GRAY32;
	case IDL_DCTLPF_BGRA16:		return DCTL::VPF_BGRA16;
	case IDL_DCTLPF_ARGB16:		return DCTL::VPF_ARGB16;
	case IDL_DCTLPF_GRAY16:		return DCTL::VPF_GRAY16;
	case IDL_DCTLPF_YUY2:		return DCTL::VPF_YUY2;
	case IDL_DCTLPF_GRAY16U:	return DCTL::VPF_GRAY16U;
	case IDL_DCTLPF_YUVA_R408:	return DCTL::VPF_YUVA_R408;
	case IDL_DCTLPF_YUVA_R4FL:	return DCTL::VPF_YUVA_R4FL;
	case IDL_DCTLPF_RGBA8:		return DCTL::VPF_RGBA8;
	case IDL_DCTLPF_RGBA32:		return DCTL::VPF_RGBA32;
	}

	NBAssert(0);
	return static_cast<DCTL::VideoPixelFormat>(format);
}

#endif