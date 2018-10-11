//	DirectConnectionInstanceCreator.h
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


#include "Platform.h"
#include "DirectConnectionCrossProcessObjects.h"
#include "DirectConnectionInterface.h"
#include "DirectConnectionInstance.h"

namespace DCTL
{

// Forward declaration
class DirectConnectionIPCClient;

// Class manages IDirectConnection instances and DirectConnectionIPCClient object,
// which is responsible for the connection between client and server.
class DirectConnectionInstanceCreator
{
	DirectConnectionInstanceCreator(const DirectConnectionInstanceCreator &str);	// not implemented
	DirectConnectionInstanceCreator& operator=(const DirectConnectionInstanceCreator &str);	// not implemented

public:

	// C-r will try to initialize connection, create server and client instances for this using DirectConnectionIPCClient object.
	DirectConnectionInstanceCreator(Port clientPort, bool* pbInitialized);
	~DirectConnectionInstanceCreator();

	// Create IDirectConnection instance for given port and channel.
	DirectConnectionInstance* CreateInstance(Channel channel);

	// Get the port on which standalone IPC server was created
	Port GetStandaloneClientPort() const;

	// Process id with which we connected
	DCTLProcessID GetProcessId() const;

	// Check if process is running right now.
	bool IsProcessRunning() const;

	// For example, you could use the SendGlobalMetadataFrame() function to check connection or close NTX standalone.
	// Use -1 for dcChannel, if you send the channel independent command.
	DCResult SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata);

protected:

	DirectConnectionIPCClient* m_pClient = nullptr;
};

}	// end namespace DCTL
