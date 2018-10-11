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

// RPC - Remote Procedure Call, more information read here:
// https://en.wikipedia.org/wiki/Microsoft_RPC
class RpcBaseServer
{
public:

	RpcBaseServer();

	/**
	 * Init Rpc Server.
	 *
	 * @param protocol - protocol for PRC. Ex: "ncalrpc".
	 * @param port     - port number. Ex: "21445"
	 * @param handle_s_ifspec - handle from generated file.
	 * @param bWait    - wait user connect.
	 *
	 */ 
	bool initServer(const char* protocol, DWORD dwStartPort, RPC_IF_HANDLE handle_s_ifspec, bool bWait);
	
	/**
	 * Init Rpc Server. This function will try to initialize server only on given port.
	 *
	 * @param protocol - protocol for PRC. Ex: "ncalrpc".
	 * @param port     - port number. Ex: "21445"
	 * @param handle_s_ifspec - handle from generated file.
	 * @param bWait    - wait user connect.
	 *
	 */ 
	bool initServerOnPort(const char* protocol, DWORD dwPort, RPC_IF_HANDLE handle_s_ifspec, bool bWait);

	/**
	 * Deinit Rpc Server.
	 *
	 * @param handle_s_ifspec - handle from generated file.
	 *
	 */ 
	bool deinitServer(RPC_IF_HANDLE handle_s_ifspec);

	/**
	 * Rpc Server wait user connect.
	 *
	 * @param bWait - wait user connect.
	 */ 
	bool serverListen(bool bWait);

	/**
	 * @return RPC server port.
	 *
	 */ 
	DWORD getPort() const
	{
		return m_dwPort;
	}

protected:

	// Port of server.
	DWORD m_dwPort;
};
