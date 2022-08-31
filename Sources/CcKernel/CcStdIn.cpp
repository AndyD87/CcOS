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
 * @brief     Implementation of Class CcStdIn
 */

#include "CcStdIn.h"
#include "CcWString.h"
#include "CcStringUtil.h"
#include "CcStatic.h"
#include "CcKernel.h"
#include <stdio.h>

#ifdef WINDOWS
#include <windows.h>
#include <iostream>
#include <istream>
#elif defined(LINUX)
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#endif

size_t CcStdIn::read(void* pBuffer, size_t uiSize)
{
  size_t iRet = SIZE_MAX;
#ifdef WINDOWS
  CCNEWARRAYTYPE(pwcBuffer, wchar_t, uiSize);
  size_t uiRead;
  if ((uiRead=readAsync(pwcBuffer, uiSize)) <= uiSize)
  {
    m_sTemporaryBackup.appendWchar(pwcBuffer, uiRead);
    if (m_sTemporaryBackup.length() > uiSize)
    {
      memcpy(pBuffer, m_sTemporaryBackup.getCharString(), uiSize);
      iRet = uiSize;
      m_sTemporaryBackup.remove(0, uiSize);
    }
    else
    {
      memcpy(pBuffer, m_sTemporaryBackup.getCharString(), m_sTemporaryBackup.length());
      iRet = m_sTemporaryBackup.length();
      m_sTemporaryBackup.clear();
    }
  }
  else
  {
    m_sTemporaryBackup.clear();
  }
  CCDELETEARR(pwcBuffer);
#elif defined(GENERIC)
  CCUNUSED(pBuffer);
  CCUNUSED(uiSize);
#else
  m_bCancleCalled = false;
  // For debugging save to pointer
  while(iRet > uiSize && m_bCancleCalled == false)
  {
    fd_set fds;
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    if (FD_ISSET(STDIN_FILENO, &fds))
    {
      char* pTarget = fgets(static_cast<char*>(pBuffer), static_cast<int>(uiSize), stdin);
      if (pTarget != nullptr)
      {
        iRet = CcStringUtil::strlen(static_cast<char*>(pBuffer));
      }
    }
    else
    {
      CcKernel::sleep(10);
    }
  }
#endif
  return iRet;
}

size_t CcStdIn::write(const void* pBuffer, size_t uiSize)
{
  CCUNUSED(pBuffer);
  CCUNUSED(uiSize);
  return SIZE_MAX;
}

size_t CcStdIn::readHidden(void* pBuffer, size_t uiSize)
{
  size_t uiRetValue = SIZE_MAX;
#ifdef WINDOWS
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(hStdin, &mode);
  mode &= ~ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);

  uiRetValue = read(pBuffer, uiSize);

  mode |= ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);
#elif defined(GENERIC)
  // Generic does not have std output ye
  CCUNUSED(pBuffer);
  CCUNUSED(uiSize);
#elif defined(LINUX)
  //struct termios tty;
  //tcgetattr(STDIN_FILENO, &tty);
  //tty.c_lflag &= ~ECHO;
  //tcsetattr(STDIN_FILENO, TCSANOW, &tty);

  uiRetValue = read(pBuffer, uiSize);

  //tty.c_lflag |= ECHO;
  //tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#else
  CCUNUSED(pBuffer);
  CCUNUSED(uiSize);
#endif
  return uiRetValue;
}


CcStatus CcStdIn::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  return false;
}

CcStatus CcStdIn::close()
{
  return false;
}

CcStatus CcStdIn::cancel()
{
#ifdef WINDOWS
  m_bCancleCalled = true;
  INPUT_RECORD ir[2];
  for (int i = 0; i < 2; i++)
  {
    KEY_EVENT_RECORD *kev = &ir[i].Event.KeyEvent;
    ir[i].EventType = KEY_EVENT;
    kev->bKeyDown = i == 0;    //<-true, than false
    kev->dwControlKeyState = 0;
    kev->wRepeatCount = 1;
    kev->uChar.UnicodeChar = VK_RETURN;
    kev->wVirtualKeyCode = VK_RETURN;
    kev->wVirtualScanCode = static_cast<WORD>(MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC));
  }
  DWORD dw; 
  WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ir, 2, &dw);
#endif
  m_bCancleCalled = true;
  return true;
}

void CcStdIn::disableBuffer()
{
#if !defined(GENERIC)
  setvbuf(stdin, NULL, _IONBF, 0);
#endif
}

#ifdef WINDOWS

size_t CcStdIn::readAsync(wchar_t* pBuffer, size_t uiSize)
{
  size_t uiReadSize = SIZE_MAX;
  m_bCancleCalled = false;
  if (nullptr != fgetws(pBuffer, static_cast<int>(uiSize), stdin))
  {
    if (!m_bCancleCalled)
    {
      uiReadSize = CcStringUtil::strlen(pBuffer, uiSize);
    }
  }
  return uiReadSize;
}

#endif
