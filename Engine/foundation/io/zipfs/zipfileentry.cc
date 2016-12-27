/****************************************************************************
Copyright (c) 2006, Radon Labs GmbH
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/


#include "stdneb.h"
#include "io/zipfs/zipfileentry.h"

namespace IO
{
using namespace Util;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
ZipFileEntry::ZipFileEntry() :
    archiveCritSect(0),
    zipFileHandle(0),
    uncompressedSize(0),
	mCurrentPos(0),
	m_localBuffer(0)
{
    Memory::Clear(&this->filePosInfo, sizeof(this->filePosInfo));
}

//------------------------------------------------------------------------------
/**
*/
ZipFileEntry::~ZipFileEntry()
{
    this->zipFileHandle = 0;
	mCurrentPos = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ZipFileEntry::Setup(const StringAtom& n, unzFile h, CriticalSection* critSect)
{
    n_assert(0 != h);
    n_assert(0 == this->zipFileHandle);
    n_assert(0 != critSect);

    this->name = n;
    this->zipFileHandle = h;

    // store pointer to archive's critical section
    this->archiveCritSect = critSect;

    // store file position
    int res = unzGetFilePos(this->zipFileHandle, &this->filePosInfo);
    n_assert(UNZ_OK == res);

    // get other data about the file
    unz_file_info fileInfo;
    res = unzGetCurrentFileInfo(this->zipFileHandle, &fileInfo, 0, 0, 0, 0, 0, 0);
    n_assert(UNZ_OK == res);
    this->uncompressedSize = fileInfo.uncompressed_size;
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileEntry::Open(const String& password)
{
	if(m_localBuffer!=NULL)
	{
		return true;
	}
    // critical section active until close is called or this function fails
    this->archiveCritSect->Enter();

    // set current file to this file
    int res = unzGoToFilePos(this->zipFileHandle, const_cast<unz_file_pos*>(&this->filePosInfo));
    if (UNZ_OK != res) 
    {
 //       this->archiveCritSect->Leave();
        return false;
    }

    // open the current file with optional password
    if (password.IsValid())
    {
        res = unzOpenCurrentFilePassword(this->zipFileHandle, password.AsCharPtr());
    }
    else
    {
        res = unzOpenCurrentFile(this->zipFileHandle);
    }
	
	m_localBuffer = (char *) Memory::Alloc(Memory::StreamDataHeap, uncompressedSize);
	unzReadCurrentFile(this->zipFileHandle, m_localBuffer, uncompressedSize);
	unzCloseCurrentFile(this->zipFileHandle);
	 this->archiveCritSect->Leave();
    if (UNZ_OK != res) 
    {
      
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
ZipFileEntry::Close()
{
	mCurrentPos = 0;
	if(m_localBuffer!=NULL)
	{
		Memory::Free(Memory::StreamDataHeap, m_localBuffer);
		m_localBuffer = NULL;

	}
    // close the file
   // int res = unzCloseCurrentFile(this->zipFileHandle);
   // n_assert(UNZ_OK == res);

    // release the critical section
 //   this->archiveCritSect->Leave();
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileEntry::Read(void* buf, Stream::Size numBytes)
{
    n_assert(0 != this->zipFileHandle);
    n_assert(0 != buf);
    n_assert(0 != this->archiveCritSect);

    // read uncompressed data 
	n_assert(m_localBuffer!=0);
	Memory::Copy(m_localBuffer+mCurrentPos,buf,numBytes);

	mCurrentPos+=numBytes;
    return true;
}

bool ZipFileEntry::SetOffset( IO::Stream::Size offset )
{
// 	int pos = unzSetOffset(zipFileHandle,offset);
// 	return pos == offset;

	mCurrentPos = offset;
	return true;
}

} // namespace ZipFileEntry



