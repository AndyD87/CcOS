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
 * @brief     Implementation of Class WindowsPipeOut
 */
#include "WindowsPipeOut.h"
#include "CcKernel.h"
#include "stdio.h"

WindowsPipeOut::WindowsPipeOut( CcIODevice* Out ):
  m_Handle(nullptr),
  m_hRead(nullptr),
  m_IODev(Out)
{
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = nullptr;
  if (!CreatePipe(&m_hRead, &m_Handle, &saAttr, 0))
    CCDEBUG("StdoutRd CreatePipe");

  // Ensure the read handle to the pipe for STDOUT is not inherited.

  if (!SetHandleInformation(m_hRead, HANDLE_FLAG_INHERIT, 0))
    CCDEBUG("Stdout SetHandleInformation");
}

WindowsPipeOut::~WindowsPipeOut( void )
{
  enterState(EThreadState::Stopping);
  CancelIo(m_hRead);
  CancelIo(m_Handle);
  if (m_Handle != 0)
    CloseHandle( m_Handle);
  if (m_hRead != 0)
    CloseHandle(m_hRead);
}

void WindowsPipeOut::run(void)
{
  char buf[1024];
  DWORD readSize;
  while (getThreadState() == EThreadState::Running)
  {
    memset(buf, 0, 1024);
    if (ReadFile(m_hRead, buf, 1024, &readSize, nullptr))
    {
      m_IODev->write(buf, readSize);
    }
  }
  enterState(EThreadState::Stopped);
}