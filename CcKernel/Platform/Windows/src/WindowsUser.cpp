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
 * @brief     Implementation of Class WindowsUser
 * @todo      Currently HomeDir is always current homedir
 */
#include "WindowsUser.h"
#include <Userenv.h>
#pragma comment(lib, "userenv.lib")

WindowsUser::WindowsUser(const CcString& Username):
  CcUser(Username)
{
  TCHAR szHomeDirBuf[MAX_PATH] = { 0 };
  HANDLE hToken = 0;
  OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);

  // Returns a path like C:/Documents and Settings/nibu if my user name is nibu
  DWORD BufSize = MAX_PATH;
  GetUserProfileDirectory(hToken, szHomeDirBuf, &BufSize);
  // Close handle opened via OpenProcessToken
  CloseHandle(hToken);
  m_sHomeDir = szHomeDirBuf;
  m_sHomeDir.normalizePath();
}

WindowsUser::~WindowsUser( void )
{
}

bool WindowsUser::login(const CcPassword& Password) const
{
  // @todo check for windows user password
  CCUNUSED(Password);
  return false;
}

void WindowsUser::setWindowsHomeDir(const CcString& sHomeDir)
{
  m_sHomeDir = sHomeDir;
}

void WindowsUser::setWindowsPassword(const CcString& sPassword)
{
  m_sPassword = sPassword;
}