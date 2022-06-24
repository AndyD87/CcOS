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
 * @brief     Implementation of Class CcFile
 */
#include "CcFile.h"
#include "CcBaseSettings.h"
#include "CcKernel.h"
#include "string.h"
#include "Hash/CcCrc32.h"
#include "CcByteArray.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcFileInfo.h"
#include "CcFileInfoList.h"
#include "CcFileSystem.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"

CcFile::CcFile(const CcString& sPath)
{
  setFilePath(sPath);
}

size_t CcFile::size()
{
  if(m_SystemFile.isValid())
    return m_SystemFile->size();
  else
    return 0;
}

uint64 CcFile::size64()
{
  if(m_SystemFile.isValid())
    return m_SystemFile->size64();
  else
    return 0;
}

size_t CcFile::read(void* pBuffer, size_t uSize)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->read(pBuffer, uSize);
  else
    return SIZE_MAX;
}

size_t CcFile::write(const void* pBuffer, size_t uSize)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->write(pBuffer, uSize);
  else
    return SIZE_MAX;
}

CcStatus CcFile::flush()
{
  if(m_SystemFile.isValid())
    return m_SystemFile->flush();
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::open(EOpenFlags flags)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->open(flags);
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::close()
{
  if(m_SystemFile.isValid())
    return m_SystemFile->close();
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::setFilePointer(uint64 pos)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->setFilePointer(pos);
  else
    return EStatus::FSFileNotFound;
}

void CcFile::setFilePath(const CcString& sPath)
{
  if(m_SystemFile != nullptr)
  {
    close();
  }
  m_SystemFile = CcFileSystem::getFile(getAbsolutePath(sPath));
}

bool CcFile::isFile() const
{
  if(m_SystemFile.isValid())
    return m_SystemFile->isFile();
  else
    return false;
}

bool CcFile::isFile(const CcString& sPath)
{
  CcFile oFile(sPath);
  return oFile.isFile();
}

bool CcFile::isDir() const
{
  if(m_SystemFile.isValid())
    return m_SystemFile->isDir();
  else
    return false;
}

bool CcFile::isDir(const CcString& sPath)
{
  CcFile oFile(sPath);
  return oFile.isDir();
}

CcStatus CcFile::move(const CcString& sPath)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->move(getAbsolutePath(sPath));
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::move(const CcString& sFrom, const CcString& sTo)
{
  CcFile oFrom(sFrom);
  return oFrom.move(sTo);
}

CcStatus CcFile::copy(const CcString& sPath)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->copy(getAbsolutePath(sPath));
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::copy(const CcString& sFrom, const CcString& sTo)
{
  CcFile oFrom(sFrom);
  return oFrom.copy(sTo);
}

bool CcFile::compare(const CcString& sFile1, const CcString& sFile2, bool bDoCrc)
{
  CcStatus bRet = false;
  if (bDoCrc)
  {
    CcCrc32 oCrcF1 = CcFile::getCrc32(sFile1);
    CcCrc32 oCrcF2 = CcFile::getCrc32(sFile2);
    if (oCrcF1 == oCrcF2)
    {
      bRet = true;
    }
  }
  else
  {
    CcByteArray oArrayF1(CCOS_DEFAULT_FILE_RW_BUFFER_SIZE);
    CcByteArray oArrayF2(CCOS_DEFAULT_FILE_RW_BUFFER_SIZE);
    CcFile oFile1(sFile1);
    CcFile oFile2(sFile2);
    if (oFile1.open(EOpenFlags::Read) &&
        oFile2.open(EOpenFlags::Read))
    {
      bRet = true;
      size_t uiReadFile1;
      size_t uiReadFile2;
      do
      {
        uiReadFile1 = oFile1.read(oArrayF1.getArray(), CCOS_DEFAULT_FILE_RW_BUFFER_SIZE);
        uiReadFile2 = oFile2.read(oArrayF2.getArray(), CCOS_DEFAULT_FILE_RW_BUFFER_SIZE);
        if (uiReadFile1 != uiReadFile2)
        {
          bRet = false;
        }
        else if ( oArrayF1 != oArrayF2)
        {
          bRet = false;
        }
      } while (uiReadFile1 != 0 && bRet != false);
      oFile1.close();
      oFile2.close();
    }
  }
  return bRet;
}

CcFileInfo CcFile::getInfo() const
{
  if(m_SystemFile.isValid())
    return m_SystemFile->getInfo();
  else
    return CcFileInfo();
}

CcDateTime CcFile::getModified() const
{
  if(m_SystemFile.isValid())
    return m_SystemFile->getModified();
  else
    return CcDateTime();
}

CcFileInfo CcFile::getInfo(const CcString& sFilePath)
{
  CcFileInfo oFileInfo;
  CcFile oFile(sFilePath);
  if (oFile.exists())
  {
    oFileInfo = oFile.getInfo();
  }
  else
  {
    CCERROR("CcFile::getInfo called on not existing file");
  }
  return oFileInfo;
}

CcDateTime CcFile::getModified(const CcString& sFilePath)
{
  CcDateTime oModified;
  CcFile oFile(sFilePath);
  if (oFile.exists())
  {
    oModified = oFile.getModified();
  }
  else
  {
    CCERROR("CcFile::getModified called on not existing file");
  }
  return oModified;
}

CcFileInfoList CcFile::getFileList() const
{
  if(m_SystemFile.isValid())
    return m_SystemFile->getFileList();
  else
    return CcFileInfoList();
}

uint64 CcFile::getFilePointer() const
{
  if(m_SystemFile.isValid())
    return m_SystemFile->getFilePointer();
  else
    return UINT64_MAX;
}

CcDateTime CcFile::getCreated() const
{
  if(m_SystemFile.isValid())
    return m_SystemFile->getCreated();
  else
    return CcDateTime();
}

CcStatus CcFile::setCreated(const CcDateTime& oDateTime)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->setCreated(oDateTime);
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::setModified(const CcDateTime& oDateTime)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->setModified(oDateTime);
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::setUserId(uint32 uiUserId)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->setUserId(uiUserId);
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::setGroupId(uint32 uiGroupId)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->setGroupId(uiGroupId);
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::setAttributes(EFileAttributes uiAttributes)
{
  if(m_SystemFile.isValid())
    return m_SystemFile->setAttributes(uiAttributes);
  else
    return EStatus::FSFileNotFound;
}

CcStatus CcFile::setCreated(const CcString& sFilePath, const CcDateTime& oDateTime)
{
  CcStatus bRet = false;
  CcFile oFile(sFilePath);
  if (oFile.open(EOpenFlags::Attributes))
  {
    bRet = oFile.setCreated(oDateTime);
    oFile.close();
  }
  return bRet;
}

CcStatus CcFile::setModified(const CcString& sFilePath, const CcDateTime& oDateTime)
{
  CcStatus bRet = false;
  CcFile oFile(sFilePath);
  if (oFile.open(EOpenFlags::Attributes))
  {
    bRet = oFile.setModified(oDateTime);
    oFile.close();
  }
  return bRet;
}

CcStatus CcFile::setUserId(const CcString& sFilePath, uint32 uiUserId)
{
  CcStatus bRet = false;
  CcFile oFile(sFilePath);
  if (oFile.open(EOpenFlags::Attributes))
  {
    bRet = oFile.setUserId(uiUserId);
    oFile.close();
  }
  return bRet;
}

CcStatus CcFile::setGroupId(const CcString& sFilePath, uint32 uiGroupId)
{
  CcStatus bRet = false;
  CcFile oFile(sFilePath);
  if (oFile.open(EOpenFlags::Attributes))
  {
    bRet = oFile.setGroupId(uiGroupId);
    oFile.close();
  }
  return bRet;
}

CcCrc32 CcFile::getCrc32()
{
  setFilePointer(0);
  CcCrc32 oCrc;
  CcByteArray oArray(CCOS_DEFAULT_FILE_RW_BUFFER_SIZE);
  size_t readSize;
  do
  {
    readSize = readArray(oArray, false);
    if(readSize != 0 && readSize <= oArray.size())
    {
      oCrc.append(oArray.getArray(), readSize);
    }
  } while (readSize == CCOS_DEFAULT_FILE_RW_BUFFER_SIZE);
  return oCrc;
}

CcCrc32 CcFile::getCrc32(const CcString& sPathToFile)
{
  CcCrc32 oRet;
  CcFile oFile(sPathToFile);
  if (oFile.open(EOpenFlags::Read))
  {
    oRet = oFile.getCrc32();
    oFile.close();
  }
  return oRet;
}

void* CcFile::getStdFile()
{
  return m_SystemFile->getStdFile();
}

bool CcFile::exists(const CcString& sPathToFile)
{
  CcFile cFile(sPathToFile);
  return cFile.isFile();
}

CcStatus CcFile::remove(const CcString& sPathToFile)
{
  return CcFileSystem::remove(getAbsolutePath(sPathToFile));
}

CcString CcFile::getAbsolutePath(const CcString& sPathToFile)
{
  CcString sPath;
  if (CcStringUtil::isPathAbsolute(sPathToFile))
  {
    sPath = sPathToFile;
  }
  else
  {
    // append relative path to working dir
    CcString sAbsolutePath = CcKernel::getWorkingDir();
    sAbsolutePath.appendPath(sPathToFile);
    sPath = sAbsolutePath;
  }
  return sPath.normalizePath();
}
