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

#ifdef WINDOWS
#include <windows.h>
#elif defined(LINUX)
#include <termios.h>
#include <unistd.h>
#endif

size_t CcStdIn::read(void* pBuffer, size_t uSize)
{
  size_t iRet = SIZE_MAX;
#ifdef WINDOWS
  CCNEWARRAYTYPE(pwcBuffer, wchar_t, uSize);
  wchar_t* pOutput;
  if ((pOutput = fgetws(pwcBuffer, (int)uSize, stdin)) != nullptr)
  {
    size_t uiRead = CcStringUtil::findChar(pwcBuffer, uSize, '\0');
    m_sTemporaryBackup.appendWchar(pwcBuffer, uiRead);
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
  CCDELETEARR(pwcBuffer);
#elif defined(GENERIC)
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
#else
  // For debugging save to pointer
  char* pTarget = fgets(static_cast<char*>(pBuffer), static_cast<int>(uSize), stdin);
  if (pTarget != nullptr)
  {
    iRet = CcStringUtil::strlen(static_cast<char*>(pBuffer));
  }
#endif
  return iRet;
}

size_t CcStdIn::write(const void* pBuffer, size_t uSize)
{
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
  return SIZE_MAX;
}

size_t CcStdIn::readHidden(void* pBuffer, size_t uSize)
{
  size_t uiRetValue = SIZE_MAX;
#ifdef WINDOWS
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(hStdin, &mode);
  mode &= ~ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);

  uiRetValue = read(pBuffer, uSize);

  mode |= ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);
#elif defined(GENERIC)
  // Generic does not have std output ye
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
#elif defined(LINUX)
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

void CcStdIn::disableBuffer()
{
#if !defined(GENERIC)
  setvbuf(stdin, NULL, _IONBF, 0);
#endif
}
