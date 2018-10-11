/*	RpcBaseClient.cpp

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

#include "RpcBaseClient.h"

RpcBaseClient::RpcBaseClient()
{
	m_dwPort = 0;
	m_hExplicitBinding = NULL;
}

RpcBaseClient::~RpcBaseClient()
{
	deinitClient();
}

bool RpcBaseClient::initClient(const char * protocol, DWORD dwPort)
{
	deinitClient();

	RPC_STATUS status;
	unsigned char* szStringBinding = NULL;

	m_dwPort = dwPort;
	char szPort[MAX_PATH] = { 0 };
	itoa((int)m_dwPort, szPort, 10);

	// Creates a string binding handle.
	// This function is nothing more than a printf.
	// Connection is not done here.
	status = ::RpcStringBindingComposeA(
		NULL, // UUID to bind to.
		reinterpret_cast<unsigned char*>((char *)protocol),
		NULL,
		reinterpret_cast<unsigned char*>((char *)szPort),
		NULL, // Protocol dependent network options to use.
		&szStringBinding); // String binding output.

	if (status)
	{
		return false;
	}

	// Validates the format of the string binding handle and converts
	// it to a binding handle.
	// Connection is not done here either.
	status = ::RpcBindingFromStringBindingA(
		szStringBinding, // The string binding to validate.
		&m_hExplicitBinding); // Put the result in the implicit binding

	::RpcStringFreeA(&szStringBinding);

	if (status)
	{
		return false;
	}

	return true;
}

bool RpcBaseClient::deinitClient()
{
	const bool result = !!m_hExplicitBinding;

	if (m_hExplicitBinding)
	{
		::RpcBindingFree(&m_hExplicitBinding);
		m_hExplicitBinding = NULL;
	}

	return result;
}

handle_t RpcBaseClient::getExplicitHandle() const
{
	return m_hExplicitBinding;
}

DWORD RpcBaseClient::getPort() const
{
	return m_dwPort;
}
