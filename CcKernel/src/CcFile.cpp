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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
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

CcFile::CcFile(void):
  m_filePointer(0),
  m_SystemFile(nullptr)
{
}

CcFile::CcFile(const CcString& path):
  m_filePointer(0),
  m_SystemFile(nullptr)
{
    //check if path is relative or absolute
    if ( path.startWith("/") || 
        (path.length() > 1 && path.at(1) == ':'))
    {
      m_SystemFile = CcKernel::getFileSystemManager().getFile(path);
    }
    else
    {
      // append relative path to working dir
      CcString sAbsolutePath = CcKernel::getFileSystemManager().getWorkingDir();
      sAbsolutePath.appendPath(path);
      m_SystemFile = CcKernel::getFileSystemManager().getFile(sAbsolutePath);
    }
}

CcFile::~CcFile( void )
{
}

size_t CcFile::size(void)
{
  return m_SystemFile->size();
}

size_t CcFile::read(char* buffer, size_t size)
{
  return m_SystemFile->read(buffer, size);
}

size_t CcFile::write(const char* buffer, size_t size)
{
  return m_SystemFile->write(buffer, size);
}

bool CcFile::open(EOpenFlags flags)
{
  return m_SystemFile->open(flags);
}

bool CcFile::close(void)
{
  return m_SystemFile->close();
}

bool CcFile::setFilePointer(size_t pos)
{
  return m_SystemFile->setFilePointer(pos);
}

bool CcFile::isFile(void) const
{
  return m_SystemFile->isFile();
}

bool CcFile::isDir(void) const
{
  return m_SystemFile->isDir();
}

bool CcFile::move(const CcString& sPath)
{
  return m_SystemFile->move(sPath);
}

bool CcFile::move(const CcString sFrom, const CcString& sTo)
{
  CcFile oFrom(sFrom);
  return oFrom.move(sTo);
}

CcFileInfo CcFile::getInfo() const
{
  return m_SystemFile->getInfo();
}

CcDateTime CcFile::getModified(void) const
{
  return m_SystemFile->getModified();
}

CcFileInfoList CcFile::getFileList() const
{
  return m_SystemFile->getFileList();
}

CcDateTime CcFile::getCreated(void) const
{
  return m_SystemFile->getCreated();
}

bool CcFile::setCreated(const CcDateTime& oDateTime)
{
  return m_SystemFile->setCreated(oDateTime);
}

bool CcFile::setModified(const CcDateTime& oDateTime)
{
  return m_SystemFile->setModified(oDateTime);
}

bool CcFile::setUserId(uint16 uiUserId)
{
  return m_SystemFile->setUserId(uiUserId);
}

bool CcFile::setGroupId(uint16 uiGroupId)
{
  return m_SystemFile->setGroupId(uiGroupId);
}

CcCrc32 CcFile::getCrc32()
{
  setFilePointer(0);
  CcCrc32 oCrc;
  size_t readSize;
  do
  {
    CcByteArray oArray(1024); // @todo: Magic number
    readSize = readArray(oArray);
    oCrc.append(oArray);
  } while (readSize == 1024); // @todo: Magic number
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

bool CcFile::exists(const CcString& sPathToFile)
{
  CcFile cFile(sPathToFile);
  return cFile.isFile();
}

bool CcFile::remove(const CcString& sPathToFile)
{
  return CcKernel::getFileSystemManager().remove(sPathToFile);
}
