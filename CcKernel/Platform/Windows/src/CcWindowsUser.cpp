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
 * @brief     Implementation of Class CcWindowsUser
 * @todo      Currently HomeDir is always current homedir
 */
#include "CcWindowsUser.h"
#include <Userenv.h>
#pragma comment(lib, "userenv.lib")

CcWindowsUser::CcWindowsUser(const CcString& Username):
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

CcWindowsUser::~CcWindowsUser( void )
{
}

bool CcWindowsUser::login(const CcPassword& Password) const
{
  // @todo check for windows user password
  CCUNUSED(Password);
  return false;
}

void CcWindowsUser::setWindowsHomeDir(const CcString& sHomeDir)
{
  m_sHomeDir = sHomeDir;
}

void CcWindowsUser::setWindowsPassword(const CcString& sPassword)
{
  m_sPassword = sPassword;
}