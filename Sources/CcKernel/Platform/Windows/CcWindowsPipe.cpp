/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcWindowsPipe
 */
#include "CcWindowsPipe.h"
#include "CcKernel.h"
#include "CcWString.h"

//! Default buffer size for common buffer on windows
#define CcWindowsPipe_CommonBufferSize 1024 * 10

CcWindowsPipe::CcWindowsPipe()
{
  open(EOpenFlags::ReadWrite);
}

CcWindowsPipe::~CcWindowsPipe()
{
  close();
}

size_t CcWindowsPipe::read(void* buffer, size_t size)
{
  lock();
  DWORD uiReadAll = 0;
  DWORD uiSizePeeked = 0;
  if (m_oReadBuffer.size() > 0)
  {
    uiReadAll = static_cast<DWORD>(m_oReadBuffer.read(buffer, size));
    m_oReadBuffer.remove(0, uiReadAll);
  }
  while ( PeekNamedPipe(m_hRead, NULL, 0, NULL, &uiSizePeeked, NULL) &&
          uiSizePeeked > 0 &&
          uiReadAll < size )
  {
    if (uiSizePeeked > size - uiReadAll)
      uiSizePeeked = static_cast<DWORD>(size - uiReadAll);
    DWORD uiSizeRead = 0;
    if ( ReadFile(m_hRead, (unsigned char*) buffer + uiReadAll, uiSizePeeked, &uiSizeRead, nullptr) )
    {
      uiReadAll += uiSizeRead;
    }
  }
  unlock();
  return uiReadAll;
}

size_t CcWindowsPipe::write(const void* pBuffer, size_t uSize)
{
  size_t uiReturn = SIZE_MAX;;
  if (uSize > 0)
  {
    DWORD readSize = 0;
    if (WriteFile(m_hWrite,        // open file handle
		pBuffer,                   // start of data to write
		static_cast<DWORD>(uSize), // number of bytes to write
		&readSize,                 // number of bytes written
		nullptr)	               // no overlapped structure
    )
    {
      if (readSize) FlushFileBuffers(m_hWrite);
      uiReturn = readSize;
    }
  }
  return uiReturn;
}

CcStatus CcWindowsPipe::open(EOpenFlags)
{
  SECURITY_ATTRIBUTES saAttr1;
  saAttr1.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr1.bInheritHandle = TRUE;
  saAttr1.lpSecurityDescriptor = nullptr;
  if (!CreatePipe(&m_HandleIn, &m_hWrite, &saAttr1, 0))
    CCDEBUG("StdoutRd CreatePipe");

  // Ensure the read handle to the pipe for STDOUT is not inherited.

  if (!SetHandleInformation(m_hWrite, HANDLE_FLAG_INHERIT, 0))
    CCDEBUG("Stdout SetHandleInformation");

  SECURITY_ATTRIBUTES saAttr2;
  saAttr2.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr2.bInheritHandle = TRUE;
  saAttr2.lpSecurityDescriptor = nullptr;
  if (!CreatePipe(&m_hRead, &m_HandleOut, &saAttr2, 0))
    CCDEBUG("StdoutRd CreatePipe");

  // Ensure the read handle to the pipe for STDOUT is not inherited.

  if (!SetHandleInformation(m_hRead, HANDLE_FLAG_INHERIT, 0))
    CCDEBUG("Stdout SetHandleInformation");
  return true;
}

CcStatus CcWindowsPipe::close()
{
  CancelIo(m_hWrite);
  CancelIo(m_HandleIn);
  if (m_HandleIn != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_HandleIn);
    m_HandleIn = INVALID_HANDLE_VALUE;
  }
  if (m_hWrite != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_hWrite);
    m_hWrite = INVALID_HANDLE_VALUE;
  }

  CancelIo(m_hRead);
  CancelIo(m_HandleOut);
  if (m_HandleOut != INVALID_HANDLE_VALUE)
  {
    //CloseHandle(m_HandleOut);
    m_HandleOut = INVALID_HANDLE_VALUE;
  }

  if (m_hRead != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_hRead);
    m_hRead = INVALID_HANDLE_VALUE;
  }
  return true;
}

CcStatus CcWindowsPipe::cancel()
{
  return true;
}

CcStatus CcWindowsPipe::flush()
{
  CcStatus oStatus(false);
  //if(FlushFileBuffers(m_hWrite))
  {
    oStatus = true;
  }
  return oStatus;
}

void CcWindowsPipe::readCache()
{
  lock();
  DWORD uiSizePeeked = 0;
  if (PeekNamedPipe(m_hRead, NULL, 0, NULL, &uiSizePeeked, NULL) &&
                        uiSizePeeked > 0)
  {
    void* pBuffer = m_oReadBuffer.getArray();
    if (uiSizePeeked > CcWindowsPipe_CommonBufferSize)
      uiSizePeeked = static_cast<DWORD>(CcWindowsPipe_CommonBufferSize);
    size_t uiNewSize = m_oReadBuffer.size() + uiSizePeeked;
    if (uiNewSize > CcWindowsPipe_CommonBufferSize)
    {
      m_oReadBuffer.remove(0, uiNewSize - CcWindowsPipe_CommonBufferSize);
      uiNewSize = CcWindowsPipe_CommonBufferSize;
      pBuffer   = m_oReadBuffer.getArray();
    }
    else
    {
      m_oReadBuffer.resize(uiNewSize);
      pBuffer = m_oReadBuffer.getArray(m_oReadBuffer.size() - uiSizePeeked);
    }
    DWORD uiSizeRead = 0;
    if (ReadFile(m_hRead, (unsigned char*)pBuffer, uiSizePeeked, &uiSizeRead, nullptr))
    {

    }
  }
  unlock();
}
