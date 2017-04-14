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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class WindowsPipeIn
 */
#include "WindowsPipeIn.h"
#include "WindowsSocket.h"
#include "CcKernel.h"

WindowsPipeIn::WindowsPipeIn(CcIODevice* Out) :
  m_Handle(nullptr),
  m_hWrite(nullptr),
  m_IODev(Out)
{
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = nullptr;
  if (!CreatePipe(&m_Handle, &m_hWrite, &saAttr, 0))
    CCDEBUG("StdoutRd CreatePipe");

  // Ensure the read handle to the pipe for STDOUT is not inherited.

  if (!SetHandleInformation(m_hWrite, HANDLE_FLAG_INHERIT, 0))
    CCDEBUG("Stdout SetHandleInformation");
}

WindowsPipeIn::~WindowsPipeIn(void)
{
  enterState(EThreadState::Stopping);
  CancelIo(m_hWrite);
  CancelIo(m_Handle);
  if (m_Handle != 0)
    CloseHandle( m_Handle);
  if (m_hWrite != 0)
    CloseHandle( m_hWrite);
}

void WindowsPipeIn::run(void)
{
  char buf[1024];
  DWORD readSize;
  while (getThreadState() == EThreadState::Running)
  {
    memset(buf, 0, 1024);
    readSize = (DWORD)((WindowsSocket*)m_IODev)->readTimeout(buf, 1024, 10);
    if (readSize > 0)
    {
      WriteFile(  m_hWrite,   // open file handle
                  buf,        // start of data to write
                  readSize,   // number of bytes to write
                  nullptr,       // @todo resend if outvar is lower than readSize, number of bytes that were written
                  nullptr);      // no overlapped structure
    }
  }
  enterState(EThreadState::Stopped);
}