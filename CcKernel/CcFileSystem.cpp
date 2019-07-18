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
 * @brief     Implementation of Class CcFileSystem
 */
#include "CcFileSystem.h"
#include "CcKernel.h"
#include "CcDirectory.h"
#include "CcGlobalStrings.h"

CcVector<CcFileSystemListItem>* CcFileSystem::m_FSList = nullptr;

void CcFileSystem::init()
{
  CCNEW(m_FSList, CcVector<CcFileSystemListItem>);
}

CcFilePointer CcFileSystem::getFile(const CcString& Path)
{
  return getFileSystemByPath(Path)->getFile(Path);
}

bool CcFileSystem::mkdir(const CcString& Path)
{
  return getFileSystemByPath(Path)->mkdir(Path);
}

bool CcFileSystem::remove(const CcString& Path)
{
  return getFileSystemByPath(Path)->remove(Path);
}

CcString CcFileSystem::findExecutable(const CcString& sName)
{
  CcStringList oList = findExecutables(sName, 1);
  return oList.size() > 0 ? oList[0] : CcGlobalStrings::Empty;
}

CcStringList CcFileSystem::findExecutables(const CcString& sName, size_t uiNr)
{
  // First search in Working dir
  CcStringList oFileList;
  size_t uiCurrentFound = 0;
  CcFileInfoList oCurrentDirList = CcDirectory::getFileList("./");
  for (CcFileInfo& oCurrentFile : oCurrentDirList)
  {
    if (oCurrentFile.getName() == sName)
    {
      CcString sPath = CcKernel::getWorkingDir();
      sPath.appendPath(sName);
      oFileList.append(sPath);
      uiCurrentFound++;
      break;
    }
  }
  if (uiNr == 0 || uiNr > uiCurrentFound)
  {
    CcString sPaths = CcKernel::getEnvironmentVariable("PATH");
    CcStringList oPaths = sPaths.split(";", false);
    oFileList.append(findFileInDirectories(oPaths, sName, uiNr - uiCurrentFound));
  }
  return oFileList;
}

CcStringList CcFileSystem::findFileInDirectories(const CcStringList oDirs, const CcString& sName, size_t uiNr)
{
  CcStringList oFileList;
  size_t uiCurrentFound = 0;
  for (CcString& sPath : oDirs)
  {
    if (uiNr == 0 || uiNr > uiCurrentFound)
    {
      CcFileInfoList oPathDirList = CcDirectory::getFileList(sPath);
      for (CcFileInfo& oCurrentFile : oPathDirList)
      {
        ESensitivity eSensitivity = ESensitivity::CaseSensitiv;
#ifdef WINDOWS
        eSensitivity = ESensitivity::CaseInsensitiv;
#endif
        if (oCurrentFile.getName().compare(sName, eSensitivity))
        {
          sPath.appendPath(sName);
          oFileList.append(sPath);
          uiCurrentFound++;
          break;
        }
      }
    }
    else
    {
      break;
    }
  }
  return oFileList;
}

void CcFileSystem::addMountPoint(const CcString& sPath, CcFileSystemHandle hFilesystem)
{
  CcFileSystemListItem newItem(sPath, hFilesystem);
  bool bAdded = false;
  for (size_t i = 0; i < m_FSList->size() && bAdded == false; i++)
  {
    if (sPath.startsWith(m_FSList->at(i).getPath()))
    {
      m_FSList->insert(i, newItem);
      bAdded = true;
    }
  }
  if (bAdded == false)
  {
    m_FSList->append(newItem);
  }
}

CcFileSystemHandle CcFileSystem::getFileSystemByPath(const CcString& sPath)
{
  CcFileSystemHandle pFileSystem;
  // search in registered providers
  for (size_t i = 0; i < m_FSList->size(); i++)
  {
    if (sPath.startsWith(m_FSList->at(i).getPath()))
    {
      pFileSystem = m_FSList->at(i).getFileSystem();
      break;
    }
  }

  if(!pFileSystem.isValid() && m_FSList->size() > 0)
  {
    // Take root file system if nothing es found
    pFileSystem = m_FSList->at(0).getFileSystem();
  }
  return pFileSystem;
}
