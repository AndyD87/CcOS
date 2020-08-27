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
 * @brief     Implementation of Class CcFile
 */
#include "CcFile.h"
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

CcFile::CcFile(const CcString& sPath)
{
  setFilePath(sPath);
}

size_t CcFile::size()
{
  return m_SystemFile->size();
}

uint64 CcFile::size64()
{
  return m_SystemFile->size64();
}

size_t CcFile::read(void* pBuffer, size_t uSize)
{
  return m_SystemFile->read(pBuffer, uSize);
}

size_t CcFile::write(const void* pBuffer, size_t uSize)
{
  return m_SystemFile->write(pBuffer, uSize);
}

CcStatus CcFile::flush()
{
  return m_SystemFile->flush();
}

CcStatus CcFile::open(EOpenFlags flags)
{
  return m_SystemFile->open(flags);
}

CcStatus CcFile::close()
{
  return m_SystemFile->close();
}

CcStatus CcFile::setFilePointer(uint64 pos)
{
  return m_SystemFile->setFilePointer(pos);
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
  return m_SystemFile->isFile();
}

bool CcFile::isFile(const CcString& sPath)
{
  CcFile oFile(sPath);
  return oFile.isFile();
}

bool CcFile::isDir() const
{
  return m_SystemFile->isDir();
}

bool CcFile::isDir(const CcString& sPath)
{
  CcFile oFile(sPath);
  return oFile.isDir();
}

CcStatus CcFile::move(const CcString& sPath)
{
  return m_SystemFile->move(getAbsolutePath(sPath));
}

CcStatus CcFile::move(const CcString& sFrom, const CcString& sTo)
{
  CcFile oFrom(sFrom);
  return oFrom.move(sTo);
}

CcStatus CcFile::copy(const CcString& sPath)
{
  return m_SystemFile->copy(getAbsolutePath(sPath));
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
    CcByteArray oArrayF1(1024);
    CcByteArray oArrayF2(1024);
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
        uiReadFile1 = oFile1.read(oArrayF1.getArray(), 1024);
        uiReadFile2 = oFile2.read(oArrayF2.getArray(), 1024);
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
  return m_SystemFile->getInfo();
}

CcDateTime CcFile::getModified() const
{
  return m_SystemFile->getModified();
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
  return m_SystemFile->getFileList();
}

uint64 CcFile::getFilePointer() const
{
  return m_SystemFile->getFilePointer();
}

CcDateTime CcFile::getCreated() const
{
  return m_SystemFile->getCreated();
}

CcStatus CcFile::setCreated(const CcDateTime& oDateTime)
{
  return m_SystemFile->setCreated(oDateTime);
}

CcStatus CcFile::setModified(const CcDateTime& oDateTime)
{
  return m_SystemFile->setModified(oDateTime);
}

CcStatus CcFile::setUserId(uint32 uiUserId)
{
  return m_SystemFile->setUserId(uiUserId);
}

CcStatus CcFile::setGroupId(uint32 uiGroupId)
{
  return m_SystemFile->setGroupId(uiGroupId);
}

CcStatus CcFile::setAttributes(EFileAttributes uiAttributes)
{
  return m_SystemFile->setAttributes(uiAttributes);
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
  CcByteArray oArray(10240); // @todo: Magic number
  size_t readSize;
  do
  {
    readSize = readArray(oArray, false);
    if(readSize != 0 && readSize <= oArray.size())
    {
      oCrc.append(oArray.getArray(), readSize);
    }
  } while (readSize == 10240); // @todo: Magic number
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
  if (
    sPathToFile.startsWith(CcGlobalStrings::Seperators::Slash)
#ifdef WINDOWS
     || sPathToFile.startsWith(CcGlobalStrings::Seperators::BackSlash)
     || (sPathToFile.length() > 1 && sPathToFile.at(1) == CcGlobalStrings::Seperators::Colon[0])
#endif
  )
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
