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
 * @brief     Implementation of Class CcDirectory
 */
#include "CcDirectory.h"
#include "string.h"
#include "CcKernel.h"
#include "CcFileSystem.h"
#include "CcStringList.h"
#include "CcGlobalStrings.h"

bool CcDirectory::exists()
{
  CcFile cFile(m_sPath);
  return cFile.isDir();
}

bool CcDirectory::exists(const CcString& sPathToFile)
{
  CcFile cFile(sPathToFile);
  return cFile.isDir();
}

bool CcDirectory::create(const CcString& sPathToFile, bool bRecursive, bool bFailOnExists)
{
  bool bSuccess = true;
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
        sPath = CcGlobalStrings::Seperators::DoubleSlashes;
      }
      else
      {
        sPath = CcGlobalStrings::Seperators::Slash;
      }
    }
    if (sList.size() > 0)
    {
      do
      {
        sPath.appendPath(sList[uiCounter]);
        if (!exists(sPath))
        {
          if (!create(sPath, false, false))
            return false;
        }
        uiCounter++;
      } while (uiCounter < sList.size());
    }
  }
  else
  {
    CcStatus oStatus = CcFileSystem::mkdir(sPathToFile);
    if(oStatus != EStatus::AllOk &&
       (bFailOnExists == true || oStatus != EStatus::FSDirAlreadyExists))
    {
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool CcDirectory::create(bool bRecursive, bool bFaileOnExists)
{
  return create(m_sPath, bRecursive, bFaileOnExists);
}

bool CcDirectory::move(const CcString& sPathToDirectoryFrom, const CcString& sPathToDirectoryTo)
{
  CcFile oFile(sPathToDirectoryFrom);
  return oFile.move(sPathToDirectoryTo);
}

bool CcDirectory::move(const CcString& sPathToDirectoryTo)
{
  CcFile oFile(m_sPath);
  return oFile.move(sPathToDirectoryTo);
}

bool CcDirectory::remove(const CcString& sPathToFile)
{
  return CcFileSystem::remove(sPathToFile);
}

bool CcDirectory::copy(const CcString& sPathToFile)
{
  bool bRet = false;
  if (CcFile::isFile(m_sPath))
  {
    bRet = CcFile::copy(m_sPath, sPathToFile);
  }
  else if (CcDirectory::exists(sPathToFile) || CcDirectory::create(sPathToFile, true))
  {
    bRet = true;
    CcFileInfoList oFileList = CcDirectory::getFileList(m_sPath);
    for (CcFileInfo& oFile : oFileList)
    {
      CcString sNextDirFrom = m_sPath;
      CcString sNextDirTo = sPathToFile;
      sNextDirFrom.appendPath(oFile.getName());
      sNextDirTo.appendPath(oFile.getName());
      if (oFile.isDir())
      {
        CcDirectory::create(sNextDirTo);
        if (!CcDirectory::copy(sNextDirFrom, sNextDirTo))
        {
          bRet = false;
          break;
        }
      }
      else
      {
        if (!CcFile::copy(sNextDirFrom, sNextDirTo))
        {
          bRet = false;
          break;
        }
      }
    }
  }
  return bRet;
}

bool CcDirectory::copy(const CcString& sPathToDirectoryFrom, const CcString& sPathToDirectoryTo)
{
  CcDirectory oFile(sPathToDirectoryFrom);
  return oFile.copy(sPathToDirectoryTo);
}

bool CcDirectory::remove(const CcString& sPathToFile, bool bRecursive)
{
  CcDirectory oDir(sPathToFile);
  return oDir.remove(bRecursive);
}

bool CcDirectory::remove(bool bRecursive)
{
  if (bRecursive)
  {
    CcFileInfoList oFileList = CcDirectory::getFileList(m_sPath);
    for (CcFileInfo& oFile : oFileList)
    {
      CcString sNextDirFrom = m_sPath;
      sNextDirFrom.appendPath(oFile.getName());
      if (oFile.isDir())
      {
        if (!CcDirectory::remove(sNextDirFrom, true))
        {
          return false;
        }
      }
      else
      {
        if (!CcFile::remove(sNextDirFrom))
        {
          return false;
        }
      }
    }
  }
  return CcFileSystem::remove(m_sPath);
}

bool CcDirectory::clear(bool bRecursive)
{
  CcFileInfoList oFileList = CcDirectory::getFileList(m_sPath);
  for (CcFileInfo& oFile : oFileList)
  {
    if (oFile.isFile())
    {
      CcString sFileToDelete = m_sPath;
      sFileToDelete.appendPath(oFile.getName());
      if (!CcFile::remove(sFileToDelete))
        return false;
    }
    else if(bRecursive)
    {
      CcString sFileToDelete = m_sPath;
      sFileToDelete.appendPath(oFile.getName());
      if (!CcDirectory::remove(sFileToDelete, true))
        return false;
    }
  }
  return true;
}

bool CcDirectory::clear(const CcString& sPath, bool bRecursive)
{
  CcDirectory oDir(sPath);
  return oDir.clear(bRecursive);
}

bool CcDirectory::setCreated(const CcDateTime& oCreated)
{
  bool bRet = false;
  CcFile oFile(m_sPath);
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
  CcFile oFile(m_sPath);
  if (oFile.open(EOpenFlags::Write | EOpenFlags::Attributes | EOpenFlags::ShareWrite))
  {
    if (oFile.setModified(oModified))
    {
      bRet = true;
    }
    oFile.close();
  }
  return bRet;
}

bool CcDirectory::setUserId(uint32 uiUserId)
{
  CcFile oFile(m_sPath);
  return oFile.setUserId(uiUserId);
}

bool CcDirectory::setGroupId(uint32 uiGroupId)
{
  CcFile oFile(m_sPath);
  return oFile.setGroupId(uiGroupId);
}

CcFileInfoList CcDirectory::getFileList()
{
  CcFile cFile(m_sPath);
  return cFile.getFileList();
}

CcFileInfoList CcDirectory::getFileList(const CcString& sPathToDirectory)
{
  CcFile cFile(sPathToDirectory);
  return cFile.getFileList();
}
