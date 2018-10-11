/*	SharedMemory.cpp
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


#include "SharedMemory.h"
#include <iostream>
#include <iosfwd>
#include <sstream>

#ifdef _WIN32
// While nothing.
#elif __APPLE__
#   include <fcntl.h>      // Defines O_* constants
#   include <sys/stat.h>   // Defines mode constants
#   include <sys/mman.h>
#else
#   error Unsupported platform
#endif

// EINTR error means "This call did not succeed because it was interrupted. However, if you try again, it will probably work."
// And EINTR_LOOP help us to avoid this error.

#define EINTR_LOOP(res, failres, func)          \
    do                                          \
    {                                           \
        res = func;                             \
    } while (res == failres && errno == EINTR)

// Anonymous namespace for local support functions
namespace
{

// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
// 64 bit hash function.
// 'key' is the data to hash, 'len' is the size of the data (or how much of it to hash against)
inline uint64_t FNV1Hash64bit(const unsigned char* data, const size_t len)
{
    uint64_t hash = 0xcbf29ce484222325;
    const uint64_t prime = 0x100000001b3;

    for(size_t i = 0; i < len; ++i)
    {
        unsigned char value = data[i];
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;
}

}


CSharedMemory::CSharedMemory()
{
}

CSharedMemory::~CSharedMemory()
{
    FreeSharedMemory();
}

bool CSharedMemory::InitSharedMemory(const std::string& resourceName, bool bCreate, unsigned int bufferSize)
{
    // Before any initialization, release all previous resources.
    FreeSharedMemory();

#ifdef _WIN32
    if (bCreate)
    {
        m_hObject = ::CreateFileMappingA(
			   INVALID_HANDLE_VALUE,    // use paging file
			   nullptr,                 // default security
			   PAGE_READWRITE,          // read/write access
			   0,                       // maximum object size (high-order DWORD)
			   bufferSize,              // maximum object size (low-order DWORD)
			   resourceName.c_str());   // name of mapping object

		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			bCreate = false;
		}
    }
    else
    {
        m_hObject = ::OpenFileMappingA(
			   FILE_MAP_ALL_ACCESS,     // read/write access
			   FALSE,                   // do not inherit the name
			   resourceName.c_str());   // name of mapping object
    }

    if (m_hObject)
    {
        m_pBuffer = (void*) ::MapViewOfFile(m_hObject, // handle to map object
			   FILE_MAP_ALL_ACCESS,     // read/write permission
			   0,
			   0,
			   bufferSize);

        if (!m_pBuffer)
        {
            ::CloseHandle(m_hObject);
            m_hObject = nullptr;
        }
    }

    if (IsInited())
    {
        // For Win m_actualResourceName is the same as m_userResourceName.
        m_actualResourceName = resourceName;
    }
#elif __APPLE__
    int iFlags = 0;
    mode_t mode = 0;

    if (bCreate)
    {
        // SHM object created with RW permission for user
        iFlags = O_CREAT | O_RDWR | O_EXCL;
        // and opened with read-write access mode
        mode = S_IRUSR | S_IWUSR;
    }
    else
    {
        // Open existing SHM object
        iFlags = O_RDWR;
        mode = 0;
    }

    // shm_open has a limit of 30 characters for name length.
    // Because of that we make a hash from input name (resourceName).
    const uint64_t nameHash = FNV1Hash64bit(reinterpret_cast<const unsigned char*>(resourceName.c_str()), resourceName.size());

    // And put this hash to string to make string with appropriate length.
    std::stringstream stringFormer;

    stringFormer << nameHash;

    const std::string& actualResourceName = stringFormer.str();
    NBAssert(actualResourceName.size() <= 30);  // self-check
    // Open existing shared memory (SHM) object, or create and open new SHM object.
    int iDescriptor;
    EINTR_LOOP(iDescriptor, -1, ::shm_open(actualResourceName.c_str(), iFlags, mode));
    // If iDescriptor == -1, you could print an error in readable format using strerror(errno).

    if (iDescriptor == -1 && errno == EEXIST)
    {
        // This SHM object exists. We will try to open existing SHM object
        iFlags &= ~O_EXCL;

        EINTR_LOOP(iDescriptor, -1, ::shm_open(actualResourceName.c_str(), iFlags, mode));

#ifdef DEVELOPER_TESTING
        // TODO: this way does not work, because 'st.st_nlink' is not updated on OSX system.
        if (iDescriptor != -1)
        {
            // Nice, we opened SHM object
            int iTempResult;    // Use it for NBAssert.

            // Here the fix of case when app was crashed and did not close SHM object correctly.
            // https://stackoverflow.com/questions/42549904/qsharedmemory-is-not-getting-deleted-on-application-crash
            // We try to check the number of current attachments and if it is zero, we will close and unlink SHM object.
            struct stat st = {};
            // Get the number of current attachments and if it is 0, we will reopen SHM object
            if (::fstat(iDescriptor, &st) == 0 && st.st_nlink == 0)
            {
                // Firstly close SHM, because we opened it right now.
                EINTR_LOOP(iTempResult, -1, ::close(iDescriptor));
                // And here unlink SHM to remove it fully.
                iTempResult = ::shm_unlink(actualResourceName.c_str());
                NBAssert(iTempResult == 0);

                // Try to open SHM again
                iFlags |= O_EXCL;

                EINTR_LOOP(iDescriptor, -1, ::shm_open(actualResourceName.c_str(), iFlags, mode));
            }
        }
#endif
    }

    if (iDescriptor != -1)
    {
        /*int iTruncateRes = ::ftruncate(iDescriptor, bufferSize);

        if (iTruncateRes == -1)
        {
            struct stat sharedMemoryInfo = {};

            if (fstat(iDescriptor, &sharedMemoryInfo) != -1)
            {
                if (sharedMemoryInfo.st_size >= bufferSize)
                {
                    iTruncateRes = 0;
                }
            }
        }*/

        int iSkippedError;
        // Just call ftruncate() without error checking, if the memory is not enough, mmap() will fail.
        EINTR_LOOP(iSkippedError, -1, ::ftruncate(iDescriptor, bufferSize));

        void* pBuffer = ::mmap(NULL, bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, iDescriptor, 0);

        if (pBuffer && pBuffer != MAP_FAILED)
        {
            m_hObject = iDescriptor;
            m_pBuffer = pBuffer;
            m_actualResourceName = actualResourceName;
            bCreate = (iFlags & O_EXCL);
        }

        if (!m_pBuffer)
        {
            int iResult;

            if (iFlags & O_EXCL)
            {
                iResult = ::shm_unlink(actualResourceName.c_str());   // remove SHM object name, mark for deletion once all processes have closed
                NBAssert(iResult == 0);
            }

            EINTR_LOOP(iResult, -1, ::close(iDescriptor));
            NBAssert(iResult == 0);
        }
    }
#else
#   error Unsupported platform
#endif

    if (IsInited())
    {
        m_bCreated = bCreate;
        m_userResourceName = resourceName;
        m_nMappedSize = bufferSize;
    }
    else
    {
        FreeSharedMemory();
    }

    return IsInited();
}

void CSharedMemory::FreeSharedMemory()
{
#ifdef _WIN32
    if (m_pBuffer)
    {
        ::UnmapViewOfFile(m_pBuffer);
    }
    
    if (m_hObject)
    {
        ::CloseHandle(m_hObject);
    }
#elif __APPLE__
    int iResult;
    if (m_pBuffer && m_pBuffer != MAP_FAILED)
    {
        iResult = ::munmap(m_pBuffer, m_nMappedSize);
        NBAssert(iResult == 0);
    }

    if (m_hObject)
    {
        bool bUnlinkShm = m_bCreated;
#ifdef DEVELOPER_TESTING
        // Get the number of current attachments
        struct stat st = {};

        if (::fstat(m_hObject, &st) == 0)
        {
            bUnlinkShm = st.st_nlink == 0;
        }
#endif

        EINTR_LOOP(iResult, -1, ::close(m_hObject));
        NBAssert(iResult == 0);

        if (bUnlinkShm)
        {
            // The shm_unlink() function disassociates the shared memory object specified by name from that
            // name. The resources associated with the shared memory object remain intact until the last
            // file descriptor reference is removed, e.g., by close or munmap, at which point the
            // resources are reclaimed (if no references exist at the time of the call to shm_unlink(), the
            // resources are reclaimed immediately).  The name can only be reused when it is bound to a new
            // shared memory object with a call to shm_open with the O_CREAT flag.
            iResult = ::shm_unlink(m_actualResourceName.c_str());   // remove SHM object name, mark for deletion once all processes have closed
            NBAssert(iResult == 0);
        }
    }
#else
#   error Unsupported platform
#endif

    m_pBuffer = nullptr;
    m_hObject = 0;
    m_userResourceName.clear();
    m_actualResourceName.clear();
    m_nMappedSize = 0;
    m_bCreated = false;
}

bool CSharedMemory::IsInited() const
{
    return m_hObject && m_pBuffer;
}

void* CSharedMemory::GetBuffer() const
{
    /*
    // This code will be failed in any case, because shared memory object was allocated with m_nMappedSize.
    if (IsInited() && nMappedSize > 0 && nMappedSize > m_nMappedSize)
    {
#ifdef _WIN32
        ::UnmapViewOfFile(m_pBuffer);

        m_pBuffer = (void *) ::MapViewOfFile(m_hObject, // handle to map object
			    FILE_MAP_ALL_ACCESS,  // read/write permission
			    0,
			    0,
                nMappedSize);
#elif __APPLE__
        int iResult = ::munmap(m_pBuffer, m_nMappedSize);
        NBAssert(iResult == 0);
        m_pBuffer = nullptr;
        void* pBuffer = ::mmap(NULL, nMappedSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_hObject, 0);

        if (pBuffer && pBuffer != MAP_FAILED)
        {
            m_pBuffer = pBuffer;
        }
#else
#   error Unsupported platform
#endif

        NBAssert(m_pBuffer);
        m_nMappedSize = m_pBuffer ? nMappedSize : 0;
    }
    */

    return m_pBuffer;
}

bool CSharedMemory::ResizeBuffer(unsigned int bufferSize)
{
    bool bResult = false;

    if (IsInited())
    {
        if (bufferSize > m_nMappedSize)
        {
            const std::string resourceName = m_userResourceName;
            const bool bCreated = m_bCreated;
            FreeSharedMemory();
            bResult = InitSharedMemory(resourceName, bCreated, bufferSize);
        }
        else
        {
            bResult = true;
        }
    }

    return bResult;
}

unsigned int CSharedMemory::GetCurrentSize() const
{
    return m_nMappedSize;
}

std::string CSharedMemory::GetName() const
{
    return m_userResourceName;
}

bool CSharedMemory::IsOwner() const
{
    return m_bCreated;
}

/*
// Place here unit tests for CSharedMemory class.
// Process 1:
int main(int argc, const char * argv[])
{
    CSharedMemory mem;
    
    NBAssert(false == mem.IsInited());
    NBAssert(nullptr == mem.GetBuffer());
    NBAssert(0 == mem.GetCurrentSize());
    NBAssert(mem.GetName().empty());
    NBAssert(false == mem.IsOwner());
    
    const char* pszName = "sdjkslaj8989_909_dsd123ds";
    
    bool res1 = mem.InitSharedMemory(pszName, true, 1024);
    NBAssert(res1);
    
    bool bInitialized1 = mem.IsInited();
    void* pPtr1 = mem.GetBuffer();
    unsigned int uiSize1 = mem.GetCurrentSize();
    std::string sName1 = mem.GetName();
    bool bOwner1 = mem.IsOwner();
    
    NBAssert(bInitialized1);
    NBAssert(pPtr1);
    NBAssert(uiSize1 == 1024);
    NBAssert(pszName == mem.GetName());
    NBAssert(bOwner1);
    
    bool bResized1 = mem.ResizeBuffer(100);
    NBAssert(bResized1);
    
    NBAssert(bInitialized1 == mem.IsInited());
    NBAssert(pPtr1 == mem.GetBuffer());
    NBAssert(uiSize1 == mem.GetCurrentSize() && mem.GetCurrentSize() == 1024);
    NBAssert(sName1 == mem.GetName());
    NBAssert(bOwner1 == mem.IsOwner());
    
    bool bResized2 = mem.ResizeBuffer(10000);
    NBAssert(bResized2);
    
    NBAssert(bInitialized1 == mem.IsInited());
    NBAssert(uiSize1 != mem.GetCurrentSize() && mem.GetCurrentSize() == 10000);
    NBAssert(sName1 == mem.GetName());
    NBAssert(bOwner1 == mem.IsOwner());
    
    memset(mem.GetBuffer(), 13, mem.GetCurrentSize());
    
    CSharedMemory mem2;
    
    bool res2 = mem2.InitSharedMemory(pszName, false, mem.GetCurrentSize());
    NBAssert(res2);
    
    NBAssert(mem2.IsInited());
    NBAssert(mem2.GetBuffer());
    NBAssert(mem2.GetCurrentSize() == mem.GetCurrentSize());
    NBAssert(pszName == mem2.GetName());
    NBAssert(mem2.IsOwner() == false);
    
    int cmpRes = memcmp(mem.GetBuffer(), mem2.GetBuffer(), mem2.GetCurrentSize());
    NBAssert(cmpRes == 0);
    pause();
    return 0;
}

// Process 2
int main(int argc, const char * argv[])
{
    const char* pszName = "sdjkslaj8989_909_dsd123ds";
    
    CSharedMemory mem2;
    unsigned int size = 10000;
    
    bool res2 = mem2.InitSharedMemory(pszName, false, size);
    NBAssert(res2);
    
    NBAssert(mem2.IsInited());
    NBAssert(mem2.GetBuffer());
    NBAssert(mem2.GetCurrentSize() == size);
    NBAssert(pszName == mem2.GetName());
    NBAssert(mem2.IsOwner() == false);
    
    unsigned char* pbBuffer = (unsigned char*)mem2.GetBuffer();
    NBAssert(pbBuffer[0] == 13);
    NBAssert(pbBuffer[100] == 13);
    NBAssert(pbBuffer[1000] == 13);
    return 0;
}
*/
