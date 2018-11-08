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
 * @brief     Implementation of Class CcWindowsRegistryFile
 */
#include "CcWindowsRegistryFile.h"
#include "CcKernel.h"
#include "CcStringUtil.h"
#include "CcWString.h"
#include "CcDirectory.h"
#include "Shellapi.h"

class CcWindowsRegistryFilePrivate
{
public:
  HANDLE hFile    = INVALID_HANDLE_VALUE;
  CcWString sPath;
};

CcWindowsRegistryFile::CcWindowsRegistryFile(const CcString& path)
{
  m_pPrivate = new CcWindowsRegistryFilePrivate();
  CCMONITORNEW(m_pPrivate);
  if (path.startsWith("/reg/"))
    m_pPrivate->sPath = path.substr(5).getOsPath().getWString();
  else if (path.startsWith("/"))
    m_pPrivate->sPath = path.substr(1).getOsPath().getWString();
  else
    m_pPrivate->sPath = path.getOsPath().getWString();
}

CcWindowsRegistryFile::~CcWindowsRegistryFile( void )
{
}

size_t CcWindowsRegistryFile::read(void* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return SIZE_MAX;
}

size_t CcWindowsRegistryFile::write(const void* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return SIZE_MAX;
}

size_t CcWindowsRegistryFile::size(void)
{
  uint64 uiSize = size64();
  return static_cast<size_t>(uiSize);
}

uint64 CcWindowsRegistryFile::size64(void)
{
  uint64 uiSize = 0;
  return uiSize;
}

CcStatus CcWindowsRegistryFile::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  bool bRet(false);
  return bRet;
}

CcStatus CcWindowsRegistryFile::close(void)
{
  return false;
}

bool CcWindowsRegistryFile::isFile(void) const
{
  return false;
}

bool CcWindowsRegistryFile::isDir(void) const
{
  bool bRet(false);
  return bRet;
}

CcStatus CcWindowsRegistryFile::setFilePointer(uint64 pos)
{
  CCUNUSED(pos);
  bool bRet = false;
  return bRet;
}

// @TODO generate FileInfoList in FileSystem not here
CcFileInfoList CcWindowsRegistryFile::getFileList() const
{
  CcFileInfoList oRet;
  if (isDir())
  {
  }
  return oRet;
}

CcStatus CcWindowsRegistryFile::move(const CcString& sPath)
{
  CCUNUSED(sPath);
  return false;
}

CcStatus CcWindowsRegistryFile::copy(const CcString& sPath)
{
  CCUNUSED(sPath);
  if (isFile())
  {
    return false;
  }
  else
  {
    return CcDirectory::copy(m_pPrivate->sPath.getString(), sPath);
  }
}

CcFileInfo CcWindowsRegistryFile::getInfo(void) const
{
  CcFileInfo oFileInfo; 
  return oFileInfo;
}

CcDateTime CcWindowsRegistryFile::getModified(void) const
{
  CcDateTime tRet;
  return tRet;
}

CcDateTime CcWindowsRegistryFile::getCreated(void) const
{
  CcDateTime tRet;
  return tRet;
}

CcStatus CcWindowsRegistryFile::setCreated(const CcDateTime& oDateTime)
{
  CCUNUSED(oDateTime);
  bool bRet = false;
  return bRet;
}

CcStatus CcWindowsRegistryFile::setModified(const CcDateTime& oDateTime)
{
  CCUNUSED(oDateTime);
  bool bRet = false;
  return bRet;
}

CcStatus CcWindowsRegistryFile::setUserId(uint32 uiUserId)
{
  CCUNUSED(uiUserId);
  return false;
}

CcStatus CcWindowsRegistryFile::setGroupId(uint32 uiUserId)
{
  CCUNUSED(uiUserId);
  return false;
}

CcStatus CcWindowsRegistryFile::setAttributes(EFileAttributes uiAttributes)
{
  CCUNUSED(uiAttributes);
  return false;
}

