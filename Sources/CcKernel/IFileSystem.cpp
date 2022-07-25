/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the free Software Foundation, either version 3 of the License, or
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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     IFileSystem dummy implementations.
 */
#include "CcKernel.h"
#include "IFileSystem.h"
#include "CcFileInfo.h"
#include "CcFileInfoList.h"

bool IFileSystem::isIn(const CcString& sMountPath, const CcString& sTargetPath, CcString& sInnerPath) const
{
  bool bIsIn = sTargetPath.startsWith(sMountPath);
  if (bIsIn)
  {
    sInnerPath = sTargetPath.substr(sMountPath.length());
  }
  return bIsIn;
}

uint64 IFileSystem::getFilePointer() const
{
  return 0;
}

CcFileInfo IFileSystem::getInfo() const
{
  CcFileInfo oFileInfo;
  return oFileInfo;
}

size_t IFileSystem::read(void*, size_t)
{
  return SIZE_MAX;
}

size_t IFileSystem::size()
{
  return SIZE_MAX;
}

uint64 IFileSystem::size64()
{
  return SIZE_MAX;
}

size_t IFileSystem::write(const void*, size_t)
{
  return SIZE_MAX;
}

CcStatus IFileSystem::open(EOpenFlags)
{
  CcStatus bRet(true);
  return bRet;
}

CcStatus IFileSystem::close()
{
  bool bRet = true;
  return bRet;
}

bool IFileSystem::isFile() const
{
  return false;
}

CcStatus IFileSystem::setFilePointer(uint64)
{
  return false;
}

bool IFileSystem::isDir() const
{
  return true;
}

CcFileInfoList IFileSystem::getFileList() const
{
  return CcFileInfoList();
}

CcStatus IFileSystem::move(const CcString&)
{
  return false;
}

CcStatus IFileSystem::copy(const CcString&)
{
  return false;
}

CcDateTime IFileSystem::getModified() const
{
  return CcDateTime();
}

CcDateTime IFileSystem::getCreated() const
{
  return CcDateTime();
}

CcStatus IFileSystem::setCreated(const CcDateTime& CCUNUSED_PARAM(oDateTime))
{
  return false;
}

CcStatus IFileSystem::setModified(const CcDateTime& CCUNUSED_PARAM(oDateTime))
{
  return false;
}

CcStatus IFileSystem::setUserId(uint32 CCUNUSED_PARAM(uiUserId))
{
  return false;
}

CcStatus IFileSystem::setGroupId(uint32 CCUNUSED_PARAM(uiGroupId))
{
  return false;
}

CcStatus IFileSystem::ioControl(uint32 cmd, const void *pInArg, size_t uiInSize, void *pOutArg, size_t uiOutSize, size_t* puiWritten)
{
  CCUNUSED(cmd);
  CCUNUSED(pInArg);
  CCUNUSED(uiInSize);
  CCUNUSED(pOutArg);
  CCUNUSED(uiOutSize);
  CCUNUSED(puiWritten);
  return false;
}

CcStatus IFileSystem::setAttributes(EFileAttributes CCUNUSED_PARAM(uiAttributes))
{
  return false;
}
