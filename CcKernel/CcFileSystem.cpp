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

// Will be initialized by CcKernel at startup
CcVector<CcFileSystemListItem>* CcFileSystem::s_pFSList;

void CcFileSystem::init()
{
  CCNEW(s_pFSList, CcVector<CcFileSystemListItem>);
}

void CcFileSystem::deinit()
{
  CCDELETE(s_pFSList);
}

CcFilePointer CcFileSystem::getFile(const CcString& Path)
{
  CcFileSystemHandle oFs = getFileSystemByPath(Path);
  if (oFs.isValid())
    return oFs->getFile(Path);
  else
    return CcFilePointer();
}

CcStatus CcFileSystem::mkdir(const CcString& Path)
{
  CcFileSystemHandle oFs = getFileSystemByPath(Path);
  if (oFs.isValid())
    return oFs->mkdir(Path);
  else
    return EStatus::FileSystemNotFound;
}

CcStatus CcFileSystem::remove(const CcString& Path)
{
  CcFileSystemHandle oFs = getFileSystemByPath(Path);
  if (oFs.isValid())
    return oFs->remove(Path);
  else
    return EStatus::FileSystemNotFound;
}

CcString CcFileSystem::getNextFreeFilename(const CcString& sPath, const CcString& sName, const CcString& sAppend)
{
  CcString sTargetName = sName;
  CcString sNewBaseName = sPath;
  sNewBaseName.appendPath(sName);
  size_t uiIndex = 0;
  CcString sTempBaseName = sNewBaseName;
  while(CcFile::exists(sTempBaseName + sAppend))
  {
    sTempBaseName = sNewBaseName + "_" + CcString::fromSize(uiIndex);
    uiIndex++;
  }
  if(uiIndex > 0)
  {
    sTargetName.append("_" + CcString::fromSize(uiIndex - 1));
  }
  return sTargetName;
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
#ifdef WINDOWS
    CcStringList oPaths = sPaths.split(CcGlobalStrings::Seperators::Semicolon, false);
#else
    CcStringList oPaths = sPaths.split(CcGlobalStrings::Seperators::Colon, false);
#endif
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
  for (size_t i = 0; i < s_pFSList->size() && bAdded == false; i++)
  {
    if (sPath.startsWith(s_pFSList->at(i).getPath()))
    {
      s_pFSList->insert(i, newItem);
      bAdded = true;
    }
  }
  if (bAdded == false)
  {
    s_pFSList->append(newItem);
  }
}

void CcFileSystem::removeMountPoint(const CcString& sPath)
{
  size_t uiPos = 0;
  for (CcFileSystemListItem& oItem : *s_pFSList)
  {
    if (oItem.getPath() == sPath)
      break;
    else
      uiPos++;
  }
  if (uiPos < s_pFSList->size())
  {
    s_pFSList->remove(uiPos);
  }
}

CcFileSystemHandle CcFileSystem::getFileSystemByPath(const CcString& sPath)
{
  CcFileSystemHandle pFileSystem;
  // search in registered providers
  for (size_t i = 0; i < s_pFSList->size(); i++)
  {
    if (sPath.startsWith(s_pFSList->at(i).getPath()))
    {
      pFileSystem = s_pFSList->at(i).getFileSystem();
      break;
    }
  }

  if(!pFileSystem.isValid() && s_pFSList->size() > 0)
  {
    // Take root file system if nothing es found
    pFileSystem = s_pFSList->at(0).getFileSystem();
  }
  return pFileSystem;
}
