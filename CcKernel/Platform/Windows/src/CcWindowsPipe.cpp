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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcWindowsPipe
 */
#include "CcWindowsPipe.h"
#include "CcWindowsSocket.h"
#include "CcKernel.h"

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

CcWindowsPipe::~CcWindowsPipe(void)
{
  CancelIo(m_hWrite);
  CancelIo(m_HandleIn);
  if (m_HandleIn != INVALID_HANDLE_VALUE)
    CloseHandle( m_HandleIn);
  if (m_hWrite != INVALID_HANDLE_VALUE)
    CloseHandle( m_hWrite);

  CancelIo(m_hRead);
  CancelIo(m_HandleOut);
  if (m_HandleOut != INVALID_HANDLE_VALUE)
    CloseHandle(m_HandleOut);
  if (m_hRead != INVALID_HANDLE_VALUE)
    CloseHandle(m_hRead);
}

size_t CcWindowsPipe::read(void* buffer, size_t size)
{
  DWORD uiWriteSize;
  if (ReadFile(m_hRead, buffer, static_cast<DWORD>(size), &uiWriteSize, nullptr))
  {
  }
  else
  {
  }
  return uiWriteSize;
}

size_t CcWindowsPipe::write(const void* buffer, size_t size)
{
  DWORD readSize=0;
  if (readSize > 0)
  {
    if (WriteFile(m_hWrite,   // open file handle
      buffer,        // start of data to write
      static_cast<DWORD>(size),      // number of bytes to write
      &readSize,     // number of bytes written
      nullptr))      // no overlapped structure
    {
      return readSize;
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