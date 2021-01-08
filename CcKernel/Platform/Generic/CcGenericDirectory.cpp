/*
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @brief     Implementation of Class CcGenericDirectory
 */

// set define to default use 64bit operations in Linux
#define _FILE_OFFSET_BITS 64
#include "CcGenericDirectory.h"
#include "CcKernel.h"
#include "CcStringUtil.h"
#include "CcFileInfo.h"
#include "CcFileInfoList.h"

CcGenericDirectory::CcGenericDirectory()
{
}

CcGenericDirectory::~CcGenericDirectory()
{
  close();
}

bool CcGenericDirectory::createFile()
{
  return false;
}

uint64 CcGenericDirectory::getFilePointer() const
{
  return 0;
}

CcFileInfo CcGenericDirectory::getInfo() const
{
  return m_oFileInfo;
}

size_t CcGenericDirectory::read(void*, size_t)
{
  return SIZE_MAX;
}

size_t CcGenericDirectory::size()
{
  return SIZE_MAX;
}

uint64 CcGenericDirectory::size64()
{
  return SIZE_MAX;
}

size_t CcGenericDirectory::write(const void*, size_t)
{
  return SIZE_MAX;
}

CcStatus CcGenericDirectory::open(EOpenFlags)
{
  CcStatus bRet(true);
  return bRet;
}

CcStatus CcGenericDirectory::close()
{
  bool bRet = true;
  return bRet;
}

bool CcGenericDirectory::isFile() const
{
  return false;
}

CcStatus CcGenericDirectory::setFilePointer(uint64)
{
  return false;
}

bool CcGenericDirectory::isDir() const
{
  return true;
}

CcFileInfoList CcGenericDirectory::getFileList() const
{
  CcFileInfoList slRet;
  for(IFile* pFile : m_oFileList)
  {
    slRet.append(pFile->getInfo());
  }
  return slRet;
}

CcStatus CcGenericDirectory::move(const CcString&)
{
  return false;
}

CcStatus CcGenericDirectory::copy(const CcString&)
{
  return false;
}

CcDateTime CcGenericDirectory::getModified() const
{
  return m_oFileInfo.getModified();
}

CcDateTime CcGenericDirectory::getCreated() const
{
  return m_oFileInfo.getCreated();
}

CcStatus CcGenericDirectory::setCreated(const CcDateTime& oDateTime)
{
  m_oFileInfo.setCreated(oDateTime);
  return true;
}

CcStatus CcGenericDirectory::setModified(const CcDateTime& oDateTime)
{
  m_oFileInfo.setCreated(oDateTime);
  return true;
}

CcStatus CcGenericDirectory::setUserId(uint32 uiUserId)
{
  m_oFileInfo.setUserId(uiUserId);
  return true;
}

CcStatus CcGenericDirectory::setGroupId(uint32 uiGroupId)
{
  m_oFileInfo.setGroupId(uiGroupId);
  return true;
}

CcStatus CcGenericDirectory::ioControl(uint32 cmd, const void *pInArg, size_t uiInSize, void *pOutArg, size_t uiOutSize, size_t* puiWritten)
{
  CCUNUSED(cmd);
  CCUNUSED(pInArg);
  CCUNUSED(uiInSize);
  CCUNUSED(pOutArg);
  CCUNUSED(uiOutSize);
  CCUNUSED(puiWritten);
  return false;
}

CcStatus CcGenericDirectory::setAttributes(EFileAttributes uiAttributes)
{
  m_oFileInfo.setFlags(uiAttributes);
  return true;
}
