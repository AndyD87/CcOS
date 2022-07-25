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
 * @brief     Implementation of Class CcFileSystem
 */
#include "CcFileSystem.h"
#include "CcKernel.h"
#include "CcDirectory.h"
#include "CcGlobalStrings.h"
#include "IFileInterface.h"

// Will be initialized by CcKernel at startup
CcVector<CcFileSystemListItem>* CcFileSystem::s_pFSList;

void CcFileSystem::init(CcVector<CcFileSystemListItem>*& pItem)
{
  if (pItem == nullptr)
  {
    CCNEW(pItem, CcVector<CcFileSystemListItem>);
  }
  s_pFSList = pItem;
}

void CcFileSystem::deinit(CcVector<CcFileSystemListItem>*& pItem)
{
  if (pItem == s_pFSList)
  {
    CCDELETE(pItem);
    s_pFSList = nullptr;
  }
}

CcFilePointer CcFileSystem::getFile(const CcString& Path)
{
  CcString sInnerPath;
  CcFileSystemHandle oFs = getFileSystemByPath(Path, sInnerPath);
  if (oFs.isValid())
  {
    if(sInnerPath.length() == 0 ||
      #ifdef WINDOWS
        false
      #else
        ( sInnerPath.length() == 1 && sInnerPath[0] == '/')
      #endif
    )

    {
      return CCNEW_INLINE(IFileInterface, oFs.cast<IFile>());
    }
    else
    {
      return oFs->getFile(Path);
    }
  }
  else
    return CcFilePointer();
}

CcStatus CcFileSystem::mkdir(const CcString& Path)
{
  CcString sInnerPath;
  CcFileSystemHandle oFs = getFileSystemByPath(Path, sInnerPath);
  if (oFs.isValid())
    return oFs->mkdir(sInnerPath);
  else
    return EStatus::FileSystemNotFound;
}

CcStatus CcFileSystem::remove(const CcString& Path)
{
  CcString sInnerPath;
  CcFileSystemHandle oFs = getFileSystemByPath(Path, sInnerPath);
  if (oFs.isValid())
    return oFs->remove(sInnerPath);
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

CcString CcFileSystem::findExecutable(const CcString& sName, const CcString& sWorkingDir)
{
  CcStringList oList = findExecutables(sName, 1, sWorkingDir);
  return oList.size() > 0 ? oList[0] : CcGlobalStrings::Empty;
}

CcStringList CcFileSystem::findExecutables(const CcString& sName, size_t uiNr, const CcString& sWorkingDir)
{
  // First search in Working dir
  CcStringList oFileList;
  size_t uiCurrentFound = 0;
  CcString sCurrentWorkingDir = sWorkingDir;
  if (sWorkingDir == CcGlobalStrings::Empty)
    sCurrentWorkingDir = CcKernel::getWorkingDir();
  CcFileInfoList oCurrentDirList = CcDirectory::getFileList(sCurrentWorkingDir);
  for (CcFileInfo& oCurrentFile : oCurrentDirList)
  {
    if (oCurrentFile.getName() == sName)
    {
      CcString sPath = sCurrentWorkingDir;
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
    for (CcString& sPath : oPaths)
    {
      sPath.normalizePath();
    }
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

CcFileSystemHandle CcFileSystem::getFileSystemByPath(const CcString& sPath, CcString& sInnerPath)
{
  CcFileSystemHandle pFileSystem;
  // search in registered providers
  for (CcFileSystemListItem& pFileSysem : *s_pFSList)
  {
    if (pFileSysem.getFileSystem()->isIn(pFileSysem.getPath(), sPath, sInnerPath))
    {
      pFileSystem = pFileSysem.getFileSystem();
      break;
    }
  }

  if(!pFileSystem.isValid() && s_pFSList->size() > 0)
  {
    // Take root file system if nothing was found
    pFileSystem = s_pFSList->at(0).getFileSystem();
    sInnerPath = sPath;
  }
  return pFileSystem;
}

CcFileInfoList CcFileSystem::getFileSystemsByPath(const CcString& sPath)
{
  CcFileInfoList pFileSystem;
  // search in registered providers
  CcString sSearchPath = sPath;
  while(sSearchPath.endsWith(CcGlobalStrings::Seperators::Path))
    sSearchPath.remove(sSearchPath.length()-1);
  for (size_t i = 0; i < s_pFSList->size(); i++)
  {
    if (s_pFSList->at(i).getPath().startsWith(sSearchPath) &&
        ( s_pFSList->at(i).getPath().length() > sSearchPath.length() &&
          s_pFSList->at(i).getPath()[sSearchPath.length()] == '/')
    )
    {
      CcString sName = s_pFSList->at(i).getPath().getReplace(sSearchPath, "");
      if(sName.length())
      {
        if(sName.length() && sName[0] == '/') sName.remove(0);
        size_t uiOffset = sName.find('/');
        if(uiOffset < sName.length()) sName.remove(uiOffset, SIZE_MAX);
        CcFileInfo oFileInfo;
        oFileInfo.setFlags(EFileAttributes::Directory + static_cast<uint16>(0777));
        oFileInfo.setName(sName);
        pFileSystem.append(oFileInfo);
      }
    }
  }
  return pFileSystem;
}
