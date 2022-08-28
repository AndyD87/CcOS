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
 * @brief     Implementation of Class CcWindowsFilesystem
 */
#include "CcWindowsFilesystem.h"
#include "CcWindowsGlobals.h"
#include "CcKernel.h"
#include "CcWindowsFile.h"
CCEXTERNC_BEGIN
#include <shellapi.h>
CCEXTERNC_END

CcWindowsFilesystem::CcWindowsFilesystem()
{
}

CcWindowsFilesystem::~CcWindowsFilesystem()
{
}

bool CcWindowsFilesystem::isIn(const CcString& sMountPath, const CcString& sTargetPath, CcString& sInnerPath) const
{
  CCUNUSED(sMountPath);
  bool bIsIn = sTargetPath.length() > 1 && sTargetPath[1] == ':';
  if (bIsIn)
  {
    sInnerPath = sTargetPath;
  }
  else if (sTargetPath.length() > 1 && sTargetPath[0] == '\\' && sTargetPath[1] == '\\')
  {
    sInnerPath = sTargetPath;
    bIsIn = true;
  }
  return bIsIn;
}


CcFilePointer CcWindowsFilesystem::getFile(const CcString& path)
{
  CCNEWTYPE(file, CcWindowsFile, path);
  return file;
}

CcStatus CcWindowsFilesystem::mkdir(const CcString& Path) const
{
  CcStatus oStatus = false;
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
      // Check twice C: would return Access Denied
      if (GetFileAttributesW(sUnicode.getWcharString()) & FILE_ATTRIBUTE_DIRECTORY)
      {
        oStatus = EStatus::FSFileAlreadyExisting;
      }
      else
      {
        CCDEBUG_LASTERROR("CcWindowsFilesystem::mkdir failed with:");        
        return false;
      }
    }
    else
    {
      oStatus = EStatus::FSFileAlreadyExisting;
    }
  }
  return oStatus;
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
      CCDEBUG_LASTERROR("CcWindowsFilesystem::remove failed with:");        
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
      CCDEBUG_LASTERROR("CcWindowsFilesystem::remove failed with:");        
    }
  }
  return bRet;
}

CcFileInfo CcWindowsFilesystem::getInfo() const
{
  CcFileInfo oFileInfo;
  return oFileInfo;
}

size_t CcWindowsFilesystem::read(void*, size_t)
{
  return SIZE_MAX;
}

size_t CcWindowsFilesystem::size()
{
  return SIZE_MAX;
}

uint64 CcWindowsFilesystem::size64()
{
  return SIZE_MAX;
}

size_t CcWindowsFilesystem::write(const void*, size_t)
{
  return SIZE_MAX;
}

CcStatus CcWindowsFilesystem::open(EOpenFlags)
{
  CcStatus bRet(true);
  return bRet;
}

CcStatus CcWindowsFilesystem::close()
{
  bool bRet = true;
  return bRet;
}

bool CcWindowsFilesystem::isFile() const
{
  return false;
}

CcStatus CcWindowsFilesystem::setFilePointer(uint64)
{
  return false;
}

bool CcWindowsFilesystem::isDir() const
{
  return true;
}

CcFileInfoList CcWindowsFilesystem::getFileList() const
{
  return CcFileInfoList();
}

CcStatus CcWindowsFilesystem::move(const CcString&)
{
  return false;
}

CcStatus CcWindowsFilesystem::copy(const CcString&)
{
  return false;
}

CcDateTime CcWindowsFilesystem::getModified() const
{
  return CcDateTime();
}

CcDateTime CcWindowsFilesystem::getCreated() const
{
  return CcDateTime();
}

CcStatus CcWindowsFilesystem::setCreated(const CcDateTime& CCUNUSED_PARAM(oDateTime))
{
  return false;
}

CcStatus CcWindowsFilesystem::setModified(const CcDateTime& CCUNUSED_PARAM(oDateTime))
{
  return false;
}

CcStatus CcWindowsFilesystem::setUserId(uint32 CCUNUSED_PARAM(uiUserId))
{
  return false;
}

CcStatus CcWindowsFilesystem::setGroupId(uint32 CCUNUSED_PARAM(uiGroupId))
{
  return false;
}

CcStatus CcWindowsFilesystem::ioControl(uint32 cmd, const void *pInArg, size_t uiInSize, void *pOutArg, size_t uiOutSize, size_t* puiWritten)
{
  CCUNUSED(cmd);
  CCUNUSED(pInArg);
  CCUNUSED(uiInSize);
  CCUNUSED(pOutArg);
  CCUNUSED(uiOutSize);
  CCUNUSED(puiWritten);
  return false;
}

CcStatus CcWindowsFilesystem::setAttributes(EFileAttributes CCUNUSED_PARAM(uiAttributes))
{
  return false;
}
