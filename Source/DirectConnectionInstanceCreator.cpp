// DirectConnectionInstanceCreator.cpp
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

#include "DirectConnectionInstanceCreator.h"
#include "DirectConnectionIPCClient.h"


namespace DCTL
{

DirectConnectionInstanceCreator::DirectConnectionInstanceCreator(Port clientPort, bool* pbInitialized)
{
	bool bInitialized = false;
	// pClient will be created with one reference
	DirectConnectionIPCClient* pClient = new DirectConnectionIPCClient(clientPort, true, &bInitialized);

	if (bInitialized)
	{
		m_pClient = pClient;
		m_pClient->AddRef();
	}

	pClient->Release();

	if (pbInitialized)
	{
		*pbInitialized = bInitialized;
	}
}

DirectConnectionInstanceCreator::~DirectConnectionInstanceCreator()
{
	if (m_pClient)
	{
		m_pClient->Release();
		m_pClient = nullptr;
	}
}

DirectConnectionInstance* DirectConnectionInstanceCreator::CreateInstance(Channel channel)
{
	NBAssert(m_pClient);
	CheckPointer(m_pClient, nullptr);

	bool bInitialized = false;
	// pInstance will be created with one reference
	DirectConnectionInstance* pInstance = new DirectConnectionInstance(m_pClient, channel, &bInitialized);

	if (!bInitialized)
	{
		pInstance->Release();
		pInstance = nullptr;
	}

	return pInstance;
}

DCTL::Port DirectConnectionInstanceCreator::GetStandaloneClientPort() const
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->GetClientPort() : 0;
}

DCTLProcessID DirectConnectionInstanceCreator::GetProcessId() const
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->GetProcessID() : 0;
}

bool DirectConnectionInstanceCreator::IsProcessRunning() const
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->IsProcessRunning() : false;
}

DCResult DirectConnectionInstanceCreator::SendGlobalMetadataFrame(int dcChannel, const char* pszMetadata)
{
	NBAssert(m_pClient);
	return m_pClient ? m_pClient->SendGlobalMetadataFrame(dcChannel, pszMetadata) : DCResult::DCERR_Failed;
}

}
