//	main.cpp
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

#include <initguid.h>
#include <iostream>
#include <vector>

// Used for Sleep
#include <chrono>
#include <thread>

#include "DirectConnectionInterface.h"
#include "DirectConnectionNTXInterfaces.h"  // to define GUIDs, otherwise we will have linker errors. 
#include "DirectConnectionLibLink.h"

using namespace std::chrono;

bool SaveImage(char* szPathName, void* lpBits, int nWidth, int nHeight)

{
	//Create a new file for writing
	FILE *pFile = fopen(szPathName, "wb");
	if (pFile)
	{
		BITMAPINFOHEADER BMIH;
		BMIH.biSize = sizeof(BITMAPINFOHEADER);
		BMIH.biWidth = nWidth;
		BMIH.biHeight = nHeight;
		BMIH.biPlanes = 1;
		BMIH.biBitCount = 32;
		BMIH.biCompression = BI_RGB;
		BMIH.biSizeImage = nWidth * nHeight * 4;
		BITMAPFILEHEADER bmfh;
		int nBitsOffset = sizeof(BITMAPFILEHEADER) + BMIH.biSize;
		LONG lImageSize = BMIH.biSizeImage;
		LONG lFileSize = nBitsOffset + lImageSize;
		bmfh.bfType = 'B' + ('M' << 8);
		bmfh.bfOffBits = nBitsOffset;
		bmfh.bfSize = lFileSize;
		bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
		//Write the bitmap file header
		UINT nWrittenFileHeaderSize = fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pFile);
		//And then the bitmap info header
		UINT nWrittenInfoHeaderSize = fwrite(&BMIH, 1, sizeof(BITMAPINFOHEADER), pFile);
		//Finally, write the image data itself 
		UINT nWrittenDIBDataSize = fwrite(lpBits, 1, lImageSize, pFile);
		fclose(pFile);
		return true;
	}
	return false;
}

int main()
{
	// Frame values we will be using for testing.
	const int frameWidth = 1280;
	const int frameHeight = 720;
	const int bytesPerPixel = 4;
	const double frameRate = 30;
	const double pixelAspect = 1.0;
	const int audioSampleRate = 48000;
	const int audioChannels = 2;

	// Create the libLink that manages the connection with OvrStream or TitlerLive.
	DCTL::DirectConnectionLibLink libLink(L"Sample Client", L"OvrStream");
	//DCTL::DirectConnectionLibLink libLink(L"Sample Client", L"TitlerLive");

	std::cout << "Loading Application." << std::endl;

	// get available sources
	std::list<DCTL::PortAndChannels> sources;

	const std::chrono::milliseconds sleepForSourcesTime(500);	// 500 msec

	do
	{
		sources	= libLink.GetAvailableSources();

		if (sources.empty())
		{
			std::this_thread::sleep_for(sleepForSourcesTime);
		}
	} while (sources.empty());

	// connect to first source in this sample.
	const DCTL::PortAndChannels& sourceData = sources.front();

    std::cout << "Titler Live started with port = " << sourceData.first << '.' << std::endl;

	// Source port and channels
	const DCTL::Port port = sourceData.first;
	const DCTL::ChannelList channels = sourceData.second;

	// connect to first channel
	if (!channels.empty())
	{
		const DCTL::Channel dcChannel = channels.front();

		std::cout << "Create IDirectConnection instance" << std::endl;
		DCTL::IDirectConnection* pInstance = libLink.CreateInstance(port, dcChannel);
		if (pInstance)
		{
			// The time in seconds, you should use it to request video/audio frames.
			double startTime = 0.0;
			// Initialize the instance with width, height, frame rate, and pixel aspect ratio.
			std::cout << "Initialize IDirectConnection instance." << std::endl;
			if (pInstance->InitializeDirectConnection(frameWidth, frameHeight, frameRate, pixelAspect, false, audioSampleRate, audioChannels, &startTime) == DCTL::DCERR_OK)
			{
				// Buffer to store frame data from the instance.
				std::vector<unsigned char> videoBuffer(frameWidth * frameHeight * bytesPerPixel);

				DCTL::VideoFrameParams videoFrame(
					videoBuffer.data(),			// buffer for video frame data
					frameWidth,					// frame width
					frameHeight,				// frame height
					frameWidth * bytesPerPixel,	// frame row span in bytes
					bytesPerPixel,				// color depth
					DCTL::VPF_BGRA8				// color format
				);

				int audioSampleCount = audioSampleRate;
				std::vector<float> audioBuffer(audioChannels * audioSampleCount);
				DCTL::AudioFrameParams audioFrame(
					audioBuffer.data(),
					audioSampleRate,
					audioChannels,
					audioSampleCount
				);

				// Take current time
				const high_resolution_clock::time_point start = high_resolution_clock::now();
				// The time step between two consequent calls of GetVideoFrame-s.
				const high_resolution_clock::duration timeStep(nanoseconds(static_cast<long long>(0.5 + 1000000000.0 / frameRate)));
				// It is the time when we should start the next iteration of loop.
				high_resolution_clock::time_point nextStart = start + timeStep;

				// We will using this time to sleep while waiting for new iteration of loop,
				// to achieve perfectly aligned with the times of the time stamps.
				const milliseconds smallSleepTime(1);

				// Grab frames for 20 seconds
				for (int i = 0; i < frameRate * 20; ++i)
				{
					// Get frames. Do nothing with it.

					std::cout << "Getting video frame " << i << '.' << std::endl;
					if (pInstance->GetVideoFrame(&videoFrame, startTime) == DCTL::DCERR_OK)
					{
						// use video frame
						static int nFileCount = 0;
						char szFilePath[200];
						sprintf(szFilePath, "C:/Users/todor/Desktop/Test/Frame%d.bmp", nFileCount++);
						SaveImage(szFilePath, videoFrame.m_pBuffer, videoFrame.m_nWidth, videoFrame.m_nHeight);
					}

					std::cout << "Getting metadata frame " << i << '.' << std::endl;
					char* pszMetadata = nullptr;
					if (pInstance->GetMetadataFrame(&pszMetadata) == DCTL::DCERR_OK)
					{
						// parse metadata
                        std::cout << "Metadata - " << pszMetadata << std::endl;
						pInstance->FreeMetadataFrame(pszMetadata);
					}

					std::cout << "Sending metadata frame " << i << '.' << std::endl;
					std::string metadataToSend = "Metadata should be in XML format!";
					pInstance->SendMetadataFrame(metadataToSend.c_str());

					std::cout << "Getting audio frame " << i << '.' << std::endl;
					DCTL::AudioFrameParams localAudioFrame = audioFrame;

					// GetAudioFrame function will correct localAudioFrame.m_nSampleCount if you have big memory chunk for audio samples.
					// Or GetAudioFrame will return DCTL::DCERR_BufferIsNotEnough, if the memory is not enough.
					if (pInstance->GetAudioFrame(&localAudioFrame, startTime) == DCTL::DCERR_OK)
					{
						NBAssert(localAudioFrame.m_nSampleCount <= audioFrame.m_nSampleCount);
						// use audio frame
					}

					// Sleep until next frame.
					while (high_resolution_clock::now() < nextStart)
					{
						std::this_thread::sleep_for(smallSleepTime);
					}

					nextStart += timeStep;

					// Increment time to get next frame
					startTime += 1.0 / frameRate;
				}

                // Take and print actual video settings
                int currentVideoWidth = 0;
                int currentVideoHeight = 0;
                double currentVideoFrameRate = 0;
                double currentVideoPAR = 0;
                bool currentVideoInterlacedFlag = 0;

                pInstance->GetVideoSettings(&currentVideoWidth, &currentVideoHeight,
                    &currentVideoFrameRate, &currentVideoPAR, &currentVideoInterlacedFlag);

                std::cout << "Actual video settings:" << std::endl;
                std::cout << "\twidth = " << currentVideoWidth << std::endl;
                std::cout << "\theight = " << currentVideoHeight << std::endl;
                std::cout << "\tframerate = " << currentVideoFrameRate << std::endl;
                std::cout << "\tpixel aspect ratio = " << currentVideoPAR << std::endl;
                std::cout << "\tinterlaced flag = " << currentVideoInterlacedFlag << std::endl;

                // Take and print actual audio settings
                int currentAudioSampleRate = 0;
                int currentAudioNumChannels = 0;

                pInstance->GetAudioSettings(&currentAudioSampleRate, &currentAudioNumChannels);

                std::cout << "Actual audio settings:" << std::endl;
                std::cout << "\tsample rate = " << currentAudioSampleRate << std::endl;
                std::cout << "\tnumber of channels = " << currentAudioNumChannels << std::endl;
			}
			else
			{
				std::cout << "IDirectConnection instance failed to initialize." << std::endl;
			}

			pInstance->Release();
		}
		else
		{
			std::cout << "IDirectConnection instance failed to create." << std::endl;
		}
	}

	return 0;
}
