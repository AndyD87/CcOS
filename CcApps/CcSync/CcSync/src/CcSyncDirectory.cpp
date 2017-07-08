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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcSyncDirectory
 */
#include "CcSyncDirectory.h"
#include "CcFile.h"
#include "CcDirectory.h"
#include "CcSyncGlobals.h"
#include "CcFile.h"
#include "CcDirectory.h"
#include "Hash/CcCrc32.h"
#include "Hash/CcMd5.h"
#include "CcSqlite.h"
#include "CcSyncFileInfo.h"
#include "CcSyncFileInfoList.h"
#include "CcSyncDbClient.h"
#include "CcString.h"
#include "CcKernel.h"
#include "CcDateTime.h"

CcSyncDirectory::CcSyncDirectory(CcSyncDbClient* pDatabase, CcSyncDirectoryConfig* pConfig) :
  m_pDatabase(pDatabase),
  m_pConfig(pConfig)
{
  if (pDatabase != nullptr)
    pDatabase->setupDirectory(getName());
}

CcSyncDirectory::CcSyncDirectory(const CcSyncDirectory& oToCopy) :
  m_pDatabase(oToCopy.m_pDatabase),
  m_pConfig(oToCopy.m_pConfig)
{
}

CcSyncDirectory::~CcSyncDirectory( void )
{
}

CcSyncDirectory& CcSyncDirectory::operator=(const CcSyncDirectory& oToCopy)
{
  m_pConfig = oToCopy.m_pConfig;
  m_pDatabase = oToCopy.m_pDatabase;
  m_uiRootId = oToCopy.m_uiRootId;
  return *this;
}

void CcSyncDirectory::scan( bool bDeepSearch)
{
  if (CcDirectory::exists(m_pConfig->getLocation()))
  {
    m_pDatabase->beginTransaction();
    scanSubDir(m_uiRootId, m_pConfig->getLocation(), bDeepSearch);
    m_pDatabase->endTransaction();
  }
}

bool CcSyncDirectory::validate()
{
  bool bRet = true;
  return bRet;
}

bool CcSyncDirectory::queueHasItems()
{
  return m_pDatabase->queueHasItem(m_pConfig->getName());
}

EBackupQueueType CcSyncDirectory::queueGetNext(CcSyncFileInfo& oFileInfo, uint64 &uiQueueIndex)
{
  return m_pDatabase->queueGetNext(getName(), oFileInfo, uiQueueIndex);
}

void CcSyncDirectory::queueFinalizeDirectory(CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex)
{
  m_pDatabase->queueFinalizeDirectory(getName(), oFileInfo, uiQueueIndex);
}

void CcSyncDirectory::queueFinalizeFile(uint64 uiQueueIndex)
{
  if(uiQueueIndex != 0)
    m_pDatabase->queueFinalizeFile(getName(), uiQueueIndex);
}

void CcSyncDirectory::queueIncrementItem(uint64 uiQueueIndex)
{
  if(uiQueueIndex != 0)
    m_pDatabase->queueIncrementItem(getName(), uiQueueIndex);
}

void CcSyncDirectory::queueReset()
{
  m_pDatabase->beginTransaction();
  m_pDatabase->queueReset(getName());
  m_pDatabase->endTransaction();
}

void CcSyncDirectory::queueResetAttempts()
{
  m_pDatabase->beginTransaction();
  m_pDatabase->queueResetAttempts(getName());
  m_pDatabase->endTransaction();
}

void CcSyncDirectory::queueDownloadDirectory(const CcSyncFileInfo& oFileInfo)
{
  m_pDatabase->queueDownloadDirectory(getName(), oFileInfo);
}

void CcSyncDirectory::queueDownloadFile(const CcSyncFileInfo& oFileInfo)
{
  m_pDatabase->queueDownloadFile(getName(), oFileInfo);
}

CcString CcSyncDirectory::getInnerPathById(uint64 uiDirId)
{
  return m_pDatabase->getInnerPathById(getName(), uiDirId);
}

CcString CcSyncDirectory::getFullDirPathById(uint64 uiDirId)
{
  CcString sPath(m_pConfig->getLocation());
  sPath.appendPath(getInnerPathById(uiDirId));
  return sPath;
}

CcString CcSyncDirectory::getFullFilePathById(uint64 uiFileId)
{
  CcString sPath;
  CcSyncFileInfo oFileInfo = getFileInfoById(uiFileId);
  if (oFileInfo.id() == uiFileId)
  {
    sPath = getFullDirPathById(oFileInfo.getDirId());
    sPath.appendPath(oFileInfo.getName());
  }
  return sPath;
}

bool CcSyncDirectory::fileListInsert(const CcSyncFileInfo& oFileInfo)
{
  return (m_pDatabase->fileListInsert(getName(), oFileInfo) != 0);
}

bool CcSyncDirectory::fileListUpdate(const CcSyncFileInfo& oFileInfo)
{
  return m_pDatabase->fileListUpdate(getName(), oFileInfo);
}

bool CcSyncDirectory::fileListRemove(const CcSyncFileInfo& oFileInfo, bool bDoUpdateParents)
{
  CcString sFilePath = getFullFilePathById(oFileInfo.getId());
  if(CcFile::exists(sFilePath))
  {
    CcFile::remove(sFilePath);
  }
  return  m_pDatabase->fileListRemove(getName(), oFileInfo, bDoUpdateParents);
}

bool CcSyncDirectory::fileListRemoveWithHistory(CcSyncFileInfo& oFileInfo)
{
  bool bRet = false;
  CcSyncFileInfo oSaveFileInfo = getFileInfoById(oFileInfo.getId());
  CcString sFullPath = getFullFilePathById(oFileInfo.getId());
  CcDateTime oCurrentTime = CcKernel::getDateTime();
  CcString sPathInHistory = getHistoryDir();
  sPathInHistory.appendPath(oCurrentTime.getString("yyyy/MM/dd"));
  if (CcDirectory::create(sPathInHistory, true))
  {
    sPathInHistory.appendPath(oCurrentTime.getString("hh-mm-ss-zzzuuu."));
    sPathInHistory.append(oSaveFileInfo.getName());
    if (CcFile::move(sFullPath, sPathInHistory))
    {
      // Securely update all values with database values
      oSaveFileInfo.changed() = oCurrentTime.getTimestampS();
      historyInsert(EBackupQueueType::RemoveFile, oSaveFileInfo);
      bRet = fileListRemove(oSaveFileInfo, true);
    }
  }
  return bRet;
}

CcSyncFileInfoList CcSyncDirectory::getDirectoryInfoListById(size_t uiDirId)
{
  return m_pDatabase->getDirectoryInfoListById(getName(), uiDirId);
}

CcSyncFileInfoList CcSyncDirectory::getFileInfoListById(size_t uiDirId)
{
  return m_pDatabase->getFileInfoListById(getName(), uiDirId);
}

CcSyncFileInfo CcSyncDirectory::getDirectoryInfoById(size_t uiDirId)
{
  return m_pDatabase->getDirectoryInfoById(getName(), uiDirId);
}

CcSyncFileInfo CcSyncDirectory::getDirectoryInfoFromSubdir(size_t uiDirId, const CcString& sSubDirName)
{
  return m_pDatabase->getDirectoryInfoFromSubdir(getName(), uiDirId, sSubDirName);
}

CcSyncFileInfo CcSyncDirectory::getFileInfoById(size_t uiFileId)
{
  return m_pDatabase->getFileInfoById(getName(), uiFileId);
}

CcSyncFileInfo CcSyncDirectory::getFileInfoByFilename(size_t uiDirId, const CcString& sFileName)
{
  return m_pDatabase->getFileInfoByFilename(getName(), uiDirId, sFileName);
}

bool CcSyncDirectory::fileListCreate(CcSyncFileInfo& oFileInfo)
{
  bool bRet = false;
  CcString sFullPath = getFullDirPathById(oFileInfo.getDirId());
  sFullPath.appendPath(oFileInfo.getName());
  CcFile oFile(sFullPath);
  if (oFile.open(EOpenFlags::Write | EOpenFlags::Attributes))
  {
    if (oFile.setModified(CcDateTimeFromSeconds( oFileInfo.getModified())))
    {
      oFileInfo.changed() = CcKernel::getDateTime().getTimestampS();
      // make sure that this file id is not already used!
      if (fileListExists(oFileInfo.getId()))
        fileListRemove(oFileInfo, false);

      uint64 uiId = m_pDatabase->fileListInsert(getName(), oFileInfo);
      if (uiId > 0)
      {
        oFileInfo.id() = uiId;
        if (historyInsert(EBackupQueueType::AddFile, oFileInfo))
        {
          directoryUpdateChanged(oFileInfo.dirId());
          bRet = true;
        }
        else
        {
          m_pDatabase->fileListRemove(getName(), oFileInfo);
        }
      }
    }
    oFile.close();
  }
  return bRet;
}

bool CcSyncDirectory::fileListExists(uint64 uiFileId)
{
  return m_pDatabase->fileListExists(getName(), uiFileId);
}


void CcSyncDirectory::scanSubDir(uint64 uiDbIndex, const CcString& sPath, bool bDeepSearch)
{
  CcSyncFileInfoList oDirectoryInfoList = getDirectoryInfoListById(uiDbIndex);
  CcSyncFileInfoList oFileInfoList = getFileInfoListById(uiDbIndex);
  CcFileInfoList oSystemFileList = CcDirectory::getFileList(sPath);
  for (size_t i=0; i < oSystemFileList.size(); i++)
  {
    const CcFileInfo& oSystemFileInfo = oSystemFileList[i];
    if (oSystemFileInfo.isDir())
    {
      if (oDirectoryInfoList.containsDirectory(oSystemFileInfo.getName()))
      {
        CcString sNextPath(sPath);
        sNextPath.appendPath(oSystemFileInfo.getName());
        const CcSyncFileInfo& oBackupDirectoryInfo = oDirectoryInfoList.getDirectory(oSystemFileInfo.getName());
        if (oBackupDirectoryInfo != oSystemFileInfo)
        {
          queueUpdateDir(oBackupDirectoryInfo);
        }
        scanSubDir(oBackupDirectoryInfo.getId(), sNextPath, bDeepSearch);
        oDirectoryInfoList.removeFile(oSystemFileInfo.getName());
      }
      else
      {
        queueAddDir(0, uiDbIndex, sPath, oSystemFileInfo);
      }
    }
    else
    {
      if (oFileInfoList.containsFile(oSystemFileInfo.getName()))
      {
        const CcSyncFileInfo& oBackupFileInfo = oFileInfoList.getFile(oSystemFileInfo.getName());
        if (oBackupFileInfo != oSystemFileInfo)
        {
          queueUpdateFile(oBackupFileInfo);
        }
        oFileInfoList.removeFile(oSystemFileInfo.getName());
      }
      else
      {
        queueAddFile(0, uiDbIndex, oSystemFileInfo);
      }
    }
  }
  // FileList is processed, start removing unfound files in database
  for (const CcSyncFileInfo& oBackupFileInfo : oFileInfoList)
  {
    queueRemoveFile(0, oBackupFileInfo);
  }
  for (const CcSyncFileInfo& oBackupFileInfo : oDirectoryInfoList)
  {
    queueRemoveDir(0, oBackupFileInfo);
  }
}

void CcSyncDirectory::queueAddDir(uint64 uiDependent, uint64 uiDirId, const CcString& sParentPath, const CcFileInfo& oDirectoryInfo)
{
  CcString sRecursivePath(sParentPath);
  sRecursivePath.appendPath(oDirectoryInfo.getName());
  uint64 uiNewQueueId = m_pDatabase->queueInsert(getName(), uiDependent, EBackupQueueType::AddDir, 0, uiDirId, oDirectoryInfo.getName(), oDirectoryInfo.getUserId(), oDirectoryInfo.getGroupId());
  if (uiNewQueueId > 0)
  {
    CcFileInfoList oFileInfoList = CcDirectory::getFileList(sRecursivePath);
    for (CcFileInfo& oFileInfo : oFileInfoList)
    {
      if (oFileInfo.isDir())
      {
        queueAddDir(uiNewQueueId, 0, sRecursivePath, oFileInfo);
      }
      else
      {
        queueAddFile(uiNewQueueId, 0, oFileInfo);
      }
    }
  }
  else
  {
    CCERROR("Adding Directory to database");
  }
}

void CcSyncDirectory::queueUpdateDir(const CcSyncFileInfo& oFileInfo)
{
  uint64 uiId = m_pDatabase->queueInsert(getName(), 0, EBackupQueueType::UpdateDir, oFileInfo.getId(), oFileInfo.getDirId(), oFileInfo.getName(), oFileInfo.getUserId(), oFileInfo.getGroupId());
  if (uiId == 0)
  {
    CCERROR("Adding Files to database");
  }
}

uint64 CcSyncDirectory::queueRemoveDir(uint64 uiDependent, const CcSyncFileInfo& oFileInfo)
{
  CcSyncFileInfoList oDirectoryInfoList = getDirectoryInfoListById(oFileInfo.getId());
  CcSyncFileInfoList oFileInfoList = getFileInfoListById(oFileInfo.getId());
  // FileList is processed, start removing unfound files in database
  for (const CcSyncFileInfo& oBackupFileInfo : oFileInfoList)
  {
    uiDependent = queueRemoveFile(uiDependent, oBackupFileInfo);
  }
  for (const CcSyncFileInfo& oBackupFileInfo : oDirectoryInfoList)
  {
    uiDependent = queueRemoveDir(uiDependent, oBackupFileInfo);
  }
  uiDependent = m_pDatabase->queueInsert(getName(), uiDependent, EBackupQueueType::RemoveDir, oFileInfo.getId(), oFileInfo.getDirId(), oFileInfo.getName(), oFileInfo.getUserId(), oFileInfo.getGroupId());
  if (uiDependent == 0)
  {
    CCERROR("Adding Files to database");
  }
  return uiDependent;
}

uint64 CcSyncDirectory::queueRemoveFile(uint64 uiDependent, const CcSyncFileInfo& oFileInfo)
{
  uint64 uiId = m_pDatabase->queueInsert(getName(), uiDependent, EBackupQueueType::RemoveFile, oFileInfo.getId(), oFileInfo.getDirId(), oFileInfo.getName(), oFileInfo.getUserId(), oFileInfo.getGroupId());
  if (uiId == 0)
  {
    CCERROR("Adding Files to database");
  }
  return uiId;
}

void CcSyncDirectory::queueAddFile(uint64 uiDependent, uint64 uiDirId, const CcFileInfo& oFileInfo)
{
  uint64 uiId = m_pDatabase->queueInsert(getName(), uiDependent, EBackupQueueType::AddFile, 0, uiDirId, oFileInfo.getName(), oFileInfo.getUserId(), oFileInfo.getGroupId());
  if (uiId == 0)
  {
    CCERROR("Adding file to queue");
  }
}

void CcSyncDirectory::queueUpdateFile(const CcSyncFileInfo& oFileInfo)
{
  uint64 bSuccess = m_pDatabase->queueInsert(getName(), 0, EBackupQueueType::UpdateFile, oFileInfo.getId(), oFileInfo.getDirId(), oFileInfo.getName(), oFileInfo.getUserId(), oFileInfo.getGroupId());
  if (bSuccess == 0)
  {
    CCERROR("Adding Files to database");
  }
}

bool CcSyncDirectory::directoryListRemove(const CcSyncFileInfo& oFileInfo, bool bDoUpdateParents)
{
  return m_pDatabase->directoryListRemove(getName(), oFileInfo, bDoUpdateParents);
}

bool CcSyncDirectory::directoryCreate(CcSyncFileInfo& oFileInfo)
{
  CcString sPath = m_pConfig->getLocation();
  sPath.appendPath(getInnerPathById(oFileInfo.getDirId()));
  sPath.appendPath(oFileInfo.getName());

  CcDirectory oNewDirectory(sPath);
  if (oNewDirectory.create(sPath, true))
  {
    if (m_pDatabase->directoryCreate(getName(), oFileInfo))
    {
      return true;
    }
    else
    {
      CCDEBUG("Adding new Directory to Database failed: " + sPath);
      oNewDirectory.remove();
    }
  }
  else
  {
    CCDEBUG("Creating new Directory failed: " + sPath);
  }
  return false;
}

bool CcSyncDirectory::directoryUpdate(const CcSyncFileInfo& oFileInfo)
{
  bool bRet = false;
  if (m_pDatabase->directoryUpdate(getName(), oFileInfo))
  {
    bRet = true;
  }
  else
  {
    CCDEBUG("Updating Directory in Database failed: " + oFileInfo.getName());
  }
  return bRet;
}

bool CcSyncDirectory::directoryListRemoveWithHistory(CcSyncFileInfo& oFileInfo)
{
  bool bRet = false;
  CcString sFullPath = getFullDirPathById(oFileInfo.getId());
  CcDateTime oCurrentTime = CcKernel::getDateTime();
  if (CcDirectory::remove(sFullPath))
  {
    oFileInfo.changed() = oCurrentTime.getTimestampS();
    historyInsert(EBackupQueueType::RemoveDir, oFileInfo);
    if (directoryListRemove(oFileInfo, true))
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcSyncDirectory::directoryExists(uint64 uiDirId)
{
  return m_pDatabase->directoryExists(getName(), uiDirId);
}

bool CcSyncDirectory::directorySubDirExists(uint64 uiParentDirId, const CcString& sName)
{
  return m_pDatabase->directorySubDirExists(getName(), uiParentDirId, sName);
}

bool CcSyncDirectory::directoryEmpty(uint64 uiDirId)
{
  return m_pDatabase->directoryEmpty(getName(), uiDirId);
}

bool CcSyncDirectory::directoryInsert( const CcSyncFileInfo& oFileInfo)
{
  return (m_pDatabase->directoryInsert(getName(), oFileInfo) > 0);
}

void CcSyncDirectory::directoryUpdateChanged(uint64 uiDirId)
{
  return m_pDatabase->directoryUpdateChanged(getName(), uiDirId);
}

bool CcSyncDirectory::historyInsert(EBackupQueueType eQueueType, const CcSyncFileInfo& oFileInfo)
{
  return m_pDatabase->historyInsert(getName(), eQueueType, oFileInfo);
}

bool CcSyncDirectory::fileInDirExists(uint64 uiDirId, const CcSyncFileInfo& oFileInfo)
{
  return m_pDatabase->fileListFileExists(getName(), uiDirId, oFileInfo);
}

CcString CcSyncDirectory::getTemporaryDir()
{
  CcString sTempDir(m_pConfig->getLocation());
  sTempDir.append("_temp");
  if (CcDirectory::exists(sTempDir) == false)
  {
    CcDirectory::create(sTempDir, true);
  }
  return sTempDir;
}

CcString CcSyncDirectory::getHistoryDir()
{
  CcString sTempDir(m_pConfig->getLocation());
  sTempDir.append("_History");
  if (CcDirectory::exists(sTempDir) == false)
  {
    CcDirectory::create(sTempDir, true);
  }
  return sTempDir;
}
