/*
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

#include "RpcBaseServer.h"
#include "RpcCommon.h"


RpcBaseServer::RpcBaseServer() : m_dwPort(0)
{
}

bool RpcBaseServer::initServer(const char* protocol, DWORD dwStartPort, RPC_IF_HANDLE handle_s_ifspec, bool bWait)
{
	RPC_STATUS status = 1;

	m_dwPort = dwStartPort - 1;

	// We try 20 times to connect with differents ports.
	int nCountOfTry = 20;

	while (status != 0 && nCountOfTry > 0)
	{
		m_dwPort++;
		char szPort[MAX_PATH] = { 0 };

		itoa((int)m_dwPort, szPort, 10);

		// remote procedure calls.
		status = ::RpcServerUseProtseqEpA(
			reinterpret_cast<unsigned char*>((char*)protocol),
			RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
			reinterpret_cast<unsigned char*>((char*)szPort),
			NULL); // No security.

		nCountOfTry--;
	}

	if (status)
	{
		m_dwPort = 0;
		return false;
	}

	// Registers the Example1 interface.
	status = ::RpcServerRegisterIf2(
		handle_s_ifspec, // Interface to register.
		NULL, // Use the MIDL generated entry-point vector.
		NULL, // Use the MIDL generated entry-point vector.
		RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, // Forces use of security callback.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Use default number of concurrent calls.
		(unsigned)-1, // Infinite max size of incoming data blocks.
		SecurityCallback); // Naive security callback.

	{
// 		RPC_SERVER_INTERFACE* interfaceDesc = (RPC_SERVER_INTERFACE*)handle_s_ifspec;
// 		const GUID& guid = interfaceDesc->InterfaceId.SyntaxGUID;
	}

	if (status && status != RPC_S_ALREADY_REGISTERED)
	{
		return false;
	}

	// Start to listen for remote procedure calls for all registered interfaces.
	// This call will not return until RpcMgmtStopServerListening is called.
	status = ::RpcServerListen(1, // Recommended minimum number of threads.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Recommended maximum number of calls.
		bWait ? FALSE : TRUE); // Start listening now.

	if (status && status != RPC_S_ALREADY_LISTENING)
	{
		return false;
	}

	return true;
}

bool RpcBaseServer::initServerOnPort(const char* protocol, DWORD dwPort, RPC_IF_HANDLE handle_s_ifspec, bool bWait)
{
	RPC_STATUS status = 1;

	char szPort[MAX_PATH] = {0};

	itoa((int)dwPort, szPort, 10);

	// remote procedure calls.
	status = ::RpcServerUseProtseqEpA(
		reinterpret_cast<unsigned char*>((char*)protocol),
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
		reinterpret_cast<unsigned char*>((char*)szPort),
		NULL); // No security.

	if (status)
	{
		return false;
	}

	// Registers the Example1 interface.
	status = ::RpcServerRegisterIf2(
		handle_s_ifspec, // Interface to register.
		NULL, // Use the MIDL generated entry-point vector.
		NULL, // Use the MIDL generated entry-point vector.
		RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, // Forces use of security callback.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Use default number of concurrent calls.
		(unsigned)-1, // Infinite max size of incoming data blocks.
		SecurityCallback); // Naive security callback.

	if (status && status != RPC_S_ALREADY_REGISTERED)
	{
		return false;
	}

	// Start to listen for remote procedure calls for all registered interfaces.
	// This call will not return until RpcMgmtStopServerListening is called.
	status = ::RpcServerListen(1, // Recommended minimum number of threads.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Recommended maximum number of calls.
		bWait ? FALSE : TRUE); // Start listening now.

	if (status && status != RPC_S_ALREADY_LISTENING)
	{
		return false;
	}

	m_dwPort = dwPort;

	return true;
}

bool RpcBaseServer::deinitServer(RPC_IF_HANDLE handle_s_ifspec)
{
	RPC_STATUS status = 1;

	// Only deinitialize the server if we really mean to do this. Without this check,
	// temporary unanitialized servers created by LibLink in CheckServerConnection
	// will deinitialize true server which is handling calls back to NLE from Titler Live
	if (m_dwPort != 0)
	{
		status = ::RpcServerUnregisterIf(
			handle_s_ifspec,
			NULL,
			NULL
		);

		m_dwPort = 0;
	}

	return status == RPC_S_OK;
}

bool RpcBaseServer::serverListen(bool bWait)
{
	RPC_STATUS status = ::RpcServerListen(1, // Recommended minimum number of threads.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Recommended maximum number of threads.
		bWait ? FALSE : TRUE); // Start listening now.

	return !status;
}
