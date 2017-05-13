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
 * @brief     Implementation of Class WindowsFilesystem
 */
#include "WindowsFilesystem.h"
#include "WindowsGlobals.h"
#include "CcKernel.h"
#include "WindowsFile.h"
#include "Shellapi.h"

WindowsFilesystem::WindowsFilesystem( void )
{
}

WindowsFilesystem::~WindowsFilesystem( void )
{
}

CcFilePointer WindowsFilesystem::getFile(const CcString& path) const
{
  CcFilePointer file = new WindowsFile(path);
  CCMONITORNEW(file.ptr());
  return file;
}

bool WindowsFilesystem::mkdir(const CcString& Path) const
{
  CcUCString sUnicode = Path.getOsPath().getUnicode();
  if (CreateDirectoryW((wchar_t*)sUnicode.getWcharString(), nullptr))
  {
    return true;
  }
  else
  {
    CCDEBUG("CreateDirectory Failed with: " + CcString::fromNumber(GetLastError()));
    return false;
  }
}

bool WindowsFilesystem::remove(const CcString& Path) const
{
  CcUCString sUnicode = Path.getOsPath().getUnicode();
  if (WindowsFile(Path).isFile())
  {
    if (DeleteFileW((wchar_t*) sUnicode.getWcharString()))
      return true;
  }
  else
  {
    SHFILEOPSTRUCTW fileop;
    fileop.hwnd = nullptr;      // no status display
    fileop.wFunc = FO_DELETE;  // delete operation
    fileop.pFrom = sUnicode.getWcharString();    // source file name as double nullptr terminated string
    fileop.pTo = nullptr;      // no destination needed
    fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;  // do not prompt the user

    //if (!noRecycleBin)
    //  fileop.fFlags |= FOF_ALLOWUNDO;

    fileop.fAnyOperationsAborted = FALSE;
    fileop.lpszProgressTitle = nullptr;
    fileop.hNameMappings = nullptr;

    int ret = SHFileOperationW(&fileop);

    return (ret == 0);
  }
  return false;
}
