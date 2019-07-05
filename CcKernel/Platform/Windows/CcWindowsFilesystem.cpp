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
 * @brief     Implementation of Class CcWindowsFilesystem
 */
#include "CcWindowsFilesystem.h"
#include "CcWindowsGlobals.h"
#include "CcKernel.h"
#include "CcWindowsFile.h"
#include <shellapi.h>

CcWindowsFilesystem::CcWindowsFilesystem()
{
}

CcWindowsFilesystem::~CcWindowsFilesystem()
{
}

CcFilePointer CcWindowsFilesystem::getFile(const CcString& path) const
{
  CcFilePointer file = new CcWindowsFile(path);
  CCMONITORNEW(file.ptr());
  return file;
}

CcStatus CcWindowsFilesystem::mkdir(const CcString& Path) const
{
  CcWString sUnicode = Path.getOsPath().getWString();
  if (CreateDirectoryW((wchar_t*)sUnicode.getWcharString(), nullptr))
  {
    return true;
  }
  else
  {
    DWORD dwError = GetLastError();
    if (dwError != ERROR_ALREADY_EXISTS)
    {
      CCDEBUG("CreateDirectory Failed with: " + CcString::fromNumber(GetLastError()));
      return false;
    }
    else
    {
      return true;
    }
  }
}

CcStatus CcWindowsFilesystem::remove(const CcString& Path) const
{
  bool bRet = false;
  CcWString sUnicode = Path.getOsPath().getWString();
  if (CcWindowsFile(Path).isFile())
  {
    if (DeleteFileW((wchar_t*) sUnicode.getWcharString()))
    {
      bRet = true;
    }
    else
    {
      CCDEBUG("DeleteFile failed with: " + CcString::fromNumber(GetLastError()));
    }
  }
  else
  {
    if(RemoveDirectoryW(sUnicode.getWcharString()) != FALSE)
    {
      bRet = true;
    }
    else
    {
      CCDEBUG("DeleteFile failed with: " + CcString::fromNumber(GetLastError()));
    }
  }
  return bRet;
}
