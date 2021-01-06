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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcWindowsPipe
 */
#include "CcWindowsPipe.h"
#include "CcKernel.h"

#define CcWindowsPipe_CommonBufferSize 1024 * 10

CcWindowsPipe::CcWindowsPipe()
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
}

CcWindowsPipe::~CcWindowsPipe()
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
    CloseHandle(m_HandleOut);
    m_HandleOut = INVALID_HANDLE_VALUE;
  }

  if (m_hRead != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_hRead);
    m_hRead = INVALID_HANDLE_VALUE;
  }
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
    if (uiSizePeeked > size)
      uiSizePeeked = static_cast<DWORD>(size);
    DWORD uiSizeRead = 0;
    if ( ReadFile(m_hRead, (unsigned char*) buffer + uiReadAll, uiSizePeeked, &uiSizeRead, nullptr) )
    {
      uiReadAll += uiSizeRead;
    }
  }
  unlock();
  return uiReadAll;
}

size_t CcWindowsPipe::write(const void* buffer, size_t size)
{
  DWORD readSize=0;
  if (size > 0)
  {
    if (WriteFile(m_hWrite,   // open file handle
      buffer,        // start of data to write
      static_cast<DWORD>(size),      // number of bytes to write
      &readSize,     // number of bytes written
      nullptr))      // no overlapped structure
    {
      if (FlushFileBuffers(m_hWrite))
      {
        return readSize;
      }
    }
    else
    {
      return SIZE_MAX;
    }
  }
  return readSize;
}

CcStatus CcWindowsPipe::open(EOpenFlags)
{
  return true;
}

CcStatus CcWindowsPipe::close()
{
  return true;
}

CcStatus CcWindowsPipe::cancel()
{
  return true;
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