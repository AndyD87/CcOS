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
 * @brief     Implementation of Class CcDirectory
 */
#include "CcDirectory.h"
#include "string.h"
#include "CcKernel.h"

CcDirectory::CcDirectory(const CcString& path):
  m_Path(path)
{
}

CcDirectory::~CcDirectory( void )
{
}

bool CcDirectory::exists()
{
  CcFile cFile(m_Path);
  return cFile.isDir();
}

bool CcDirectory::exists(const CcString& sPathToFile)
{
  CcFile cFile(sPathToFile);
  return cFile.isDir();
}

bool CcDirectory::create(const CcString& sPathToFile)
{
  return CcKernel::getFileSystemManager().mkdir(sPathToFile);
}

bool CcDirectory::create(const CcString& sPathToFile, bool bRecursive)
{
  if (bRecursive)
  {
    CcStringList sList = sPathToFile.split('/');
    size_t uiCounter = 0;
    CcString sPath;
    if (sPathToFile.length() > 0 &&
        sPathToFile[0] == '/')
    {
      if (sPathToFile.length() > 1 &&
        sPathToFile[1] == '/')
      {
        sPath = "//";
      }
      else
      {
        sPath = "/";
      }
    }
    do
    {
      sPath.appendPath(sList[uiCounter]);
      if (!exists(sPath))
      {
        if (!create(sPath))
          return false;
      }
      uiCounter++;
    } while (uiCounter < sList.size());

  }
  else
    return CcKernel::getFileSystemManager().mkdir(sPathToFile);
  return true;
}

bool CcDirectory::create(bool bRecursive)
{
  return create(m_Path, bRecursive);
}

bool CcDirectory::move(const CcString& sPathToDirectoryFrom, const CcString& sPathToDirectoryTo)
{
  CcFile oFile(sPathToDirectoryFrom);
  return oFile.move(sPathToDirectoryTo);
}

bool CcDirectory::move(const CcString& sPathToDirectoryTo)
{
  CcFile oFile(m_Path);
  return oFile.move(sPathToDirectoryTo);
}

bool CcDirectory::remove(const CcString& sPathToFile)
{
  return CcKernel::getFileSystemManager().remove(sPathToFile);
}

bool CcDirectory::remove(const CcString& sPathToFile, bool bRecursive)
{
  if (bRecursive)
    return CcKernel::getFileSystemManager().remove(sPathToFile);
  else
    return CcKernel::getFileSystemManager().remove(sPathToFile);
}

bool CcDirectory::remove(bool bRecursive)
{
  return remove(m_Path, bRecursive);
}

bool CcDirectory::setCreated(const CcDateTime& oCreated)
{
  bool bRet = false;
  CcFile oFile(m_Path);
  if (oFile.open(EOpenFlags::Write | EOpenFlags::Attributes | EOpenFlags::ShareWrite))
  {
    if (oFile.setCreated(oCreated))
    {
      bRet = true;
    }
    oFile.close();
  }
  return bRet;
}

bool CcDirectory::setModified(const CcDateTime& oModified)
{
  bool bRet = false;
  CcFile oFile(m_Path);
  if (oFile.open(EOpenFlags::Write | EOpenFlags::Attributes | EOpenFlags::ShareWrite))
  {
    if (oFile.setModified(oModified))
    {
      bRet = true;
    }
    oFile.close();
  }
  return true;
}

bool CcDirectory::setUserId(uint16 uiUserId)
{
  CcFile oFile(m_Path);
  return oFile.setUserId(uiUserId);
}

bool CcDirectory::setGroupId(uint16 uiGroupId)
{
  CcFile oFile(m_Path);
  return oFile.setGroupId(uiGroupId);
}

CcFileInfoList CcDirectory::getFileList(const CcString& sPathToDirectory)
{
  CcFile cFile(sPathToDirectory);
  return cFile.getFileList();
}
