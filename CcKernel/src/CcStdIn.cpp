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
 * @brief     Implementation of Class CcStdIn
 */

#include "CcStdIn.h"
#include "CcWString.h"
#include "CcStringUtil.h"
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#elif defined LINUX
#include <termios.h>
#include <unistd.h>
#endif

CcStdIn::CcStdIn(void)
{
}

CcStdIn::~CcStdIn()
{
}

size_t CcStdIn::read(void* pBuffer, size_t uSize)
{
  size_t iRet = 0;
#ifdef WIN32
  CcWString ucString(uSize);
  if (fgetws(ucString.getWcharString(), (int) ucString.length(), stdin) != nullptr)
  {
    ucString.resize(wcslen(ucString.getWcharString()));
    m_sTemporaryBackup.append( ucString.getString());
    if (m_sTemporaryBackup.length() > uSize)
    {
      memcpy(pBuffer, m_sTemporaryBackup.getCharString(), uSize);
      iRet = uSize;
      m_sTemporaryBackup.remove(0, uSize);
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
#elif defined LINUX
  if (fgets(static_cast<char*>(pBuffer), static_cast<int>(uSize), stdin) != nullptr)
  {
    iRet = CcStringUtil::strlen(static_cast<char*>(pBuffer));
  }
#else
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
#endif
  return iRet;
}

size_t CcStdIn::write(const void* pBuffer, size_t uSize)
{
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
  return 0;
}

size_t CcStdIn::readHidden(void* pBuffer, size_t uSize)
{
  size_t uiRetValue = SIZE_MAX;
#ifdef WIN32
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(hStdin, &mode);
  mode &= ~ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);

  uiRetValue = read(pBuffer, uSize);

  mode |= ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);

#elif defined LINUX
  struct termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  tty.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);

  uiRetValue = read(pBuffer, uSize);

  tty.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#else
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
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
