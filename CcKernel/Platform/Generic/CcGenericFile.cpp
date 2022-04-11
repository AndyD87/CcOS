/*
 * @copyright  Andreas Dirmeier (C) 2022
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
 * @brief     Implementation of Class CcGenericFile
 */

// set define to default use 64bit operations in Linux
#include "CcGenericFile.h"
#include "CcKernel.h"
#include "CcStringUtil.h"
#include "CcFileInfo.h"
#include "CcFileInfoList.h"

CcGenericFile::CcGenericFile()
{
}

CcGenericFile::~CcGenericFile()
{
  close();
}

bool CcGenericFile::createFile()
{
  return false;
}

CcFileInfo CcGenericFile::getInfo() const
{
  return m_oFileInfo;
}

size_t CcGenericFile::read(void* pBuffer, size_t uiSize)
{
  return m_oData.read(pBuffer, uiSize);
}

size_t CcGenericFile::size()
{
  return m_oFileInfo.getFileSize();
}

uint64 CcGenericFile::size64()
{
  return m_oFileInfo.getFileSize();
}

size_t CcGenericFile::write(const void* pBuffer, size_t uiSize)
{
  return m_oData.write(pBuffer, uiSize);
}

CcStatus CcGenericFile::open(EOpenFlags eOpenFlags)
{
  CcStatus bRet(true);
  m_eOpenFlags = eOpenFlags;
  return bRet;
}

CcStatus CcGenericFile::close()
{
  bool bRet = true;
  return bRet;
}

bool CcGenericFile::isFile() const
{
  return false;
}

CcStatus CcGenericFile::setFilePointer(uint64 pos)
{
  m_uiOffset = pos;
  return true;
}

bool CcGenericFile::isDir() const
{
  return false;
}

CcFileInfoList CcGenericFile::getFileList() const
{
  CcFileInfoList slRet;
  return slRet;
}

CcStatus CcGenericFile::move(const CcString&)
{
  return false;
}

CcStatus CcGenericFile::copy(const CcString&)
{
  return false;
}

CcDateTime CcGenericFile::getModified() const
{
  return m_oFileInfo.getModified();
}

CcDateTime CcGenericFile::getCreated() const
{
  m_oFileInfo.getCreated();
  return true;
}

CcStatus CcGenericFile::setCreated(const CcDateTime& oDateTime)
{
  m_oFileInfo.setCreated(oDateTime);
  return true;
}

CcStatus CcGenericFile::setModified(const CcDateTime& oDateTime)
{
  m_oFileInfo.setCreated(oDateTime);
  return true;
}

CcStatus CcGenericFile::setUserId(uint32 uiUserId)
{
  m_oFileInfo.setUserId(uiUserId);
  return true;
}

CcStatus CcGenericFile::setGroupId(uint32 uiGroupId)
{
  m_oFileInfo.setGroupId(uiGroupId);
  return true;
}

CcStatus CcGenericFile::ioControl(uint32 cmd, const void *pInArg, size_t uiInSize, void *pOutArg, size_t uiOutSize, size_t* puiWritten)
{
  CCUNUSED(cmd);
  CCUNUSED(pInArg);
  CCUNUSED(uiInSize);
  CCUNUSED(pOutArg);
  CCUNUSED(uiOutSize);
  CCUNUSED(puiWritten);
  return false;
}

CcStatus CcGenericFile::setAttributes(EFileAttributes uiAttributes)
{
  m_oFileInfo.setFlags(uiAttributes);
  return true;
}
