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
 * @brief     Implemtation of class CcSyncClient
 */
#include "CcSyncClient.h"
#include "CcSyncGlobals.h"
#include "CcSyncRequest.h"
#include "CcSyncResponse.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcDirectory.h"
#include "CcSyncDirectory.h"
#include "Hash/CcCrc32.h"
#include "CcSslSocket.h"
#include "Communication/DCommonPorts.h"
#include "CcJson/CcJsonDocument.h"
#include "CcUserList.h"
#include "CcKernel.h"
#include "CcSyncDbClient.h"

CcSyncClient::CcSyncClient(const CcString& sAccountName)
{
  CcString sConfigFile = CcKernel::getConfigDir();
  sConfigFile.appendPath(CcSyncGlobals::ConfigDirName);
  sConfigFile.appendPath(CcSyncGlobals::Client::ConfigFileName);
  if (CcFile::exists(sConfigFile))
  {
    init(sConfigFile, sAccountName);
  }
  else
  {
    CCINFO("Configuration not found!");
  }
}

CcSyncClient::CcSyncClient(const CcString& sConfigFilePath, const CcString& sAccountName)
{
  if (CcFile::exists(sConfigFilePath))
  {
    init(sConfigFilePath, sAccountName);
  }
  else
  {
    CCINFO("Configuration not found!");
  }
}

CcSyncClient::CcSyncClient(const CcSyncClient& oToCopy)
{
  operator=(oToCopy);
}

CcSyncClient::CcSyncClient(CcSyncClient&& oToMove)
{
  operator=(std::move(oToMove));
}

CcSyncClient::~CcSyncClient(void)
{
  close();
}

CcSyncClient& CcSyncClient::operator=(const CcSyncClient& oToCopy)
{
  CCUNUSED(oToCopy);
  CCERROR("Do not copy CcSyncClient!!!");
  return *this;
}

CcSyncClient& CcSyncClient::operator=(CcSyncClient&& oToMove)
{
  if (this != &oToMove)
  {
    CCERROR("Do not move CcSyncClient!!!");
  }
  return *this;
}

bool CcSyncClient::login()
{
  m_bLogin = false;
  if (m_pAccount != nullptr)
  {
    m_oRequest.setAccountLogin(m_pAccount->getName(), m_pAccount->getName(), m_pAccount->getPassword().getString());
    if (sendRequestGetResponse())
    {
      if (!m_oResponse.hasError())
      {
        m_sSession = m_oResponse.getSession();
        m_bLogin = true;
      }
    }
  }
  return m_bLogin;
}

void CcSyncClient::logout()
{
  if (m_pAccount != nullptr)
  {
    m_oRequest.init(ESyncCommandType::Close);
    sendRequestGetResponse();
    m_pAccount = nullptr;
    m_oBackupDirectories.clear();
  }
  close();
}

CcString CcSyncClient::getAccountData()
{
  CcString sRet;
  if (m_pAccount != nullptr)
  {
    m_oRequest.init(ESyncCommandType::AccountGetData);
    m_oRequest.setSession(m_sSession);
    if (sendRequestGetResponse())
    {
      CcSyncAccountConfig oAccountConfig = m_oResponse.getAccountInfo();
      sRet = oAccountConfig.getName();
    }
  }
  return sRet;
}

void CcSyncClient::checkForServerUpdates()
{
  if (m_bLogin)
  {
    for (CcSyncDirectory& oDirectory : m_oBackupDirectories)
    {
      if (serverDirectoryEqual(oDirectory, CcSyncGlobals::Database::RootDirId))
      {
        CCDEBUG("Client is up to date");
      }
      else
      {
        m_oDatabase.beginTransaction();
        CCDEBUG("Client is not up to date with Server, start equalizing");
        doServerUpdate(oDirectory, CcSyncGlobals::Database::RootDirId);
        m_oDatabase.endTransaction();
      }
    }
  }
  else
  {
    CCERROR("Not yet logged in.");
  }
}

bool CcSyncClient::setupLocations()
{
  bool bRet = true;
  for (CcSyncDirectory& oDirectory : m_oBackupDirectories)
  {
    if (!CcDirectory::exists(oDirectory.getLocation()))
      if (!CcDirectory::create(oDirectory.getLocation()))
        bRet = false;
  }
  return bRet;
}

void CcSyncClient::scanDirectory(const CcString& sDirectoryName, bool bDeepScan)
{
  for (CcSyncDirectory& oDirectory : m_oBackupDirectories)
  {
    if (oDirectory.getName() == sDirectoryName)
    {
      oDirectory.scan(bDeepScan);
      break;
    }
  }
}

void CcSyncClient::scanDirectories( bool bDeepScan)
{
  for (CcSyncDirectory& oDirectory : m_oBackupDirectories)
  {
    oDirectory.scan(bDeepScan);
  }
}

void CcSyncClient::resetQueues()
{
  for (CcSyncDirectory& oDirectory : m_oBackupDirectories)
  {
    oDirectory.queueReset();
  }
}

void CcSyncClient::doQueue()
{
  for (CcSyncDirectory& oDirectory : m_oBackupDirectories)
  {
    oDirectory.queueResetAttempts();
    while (oDirectory.queueHasItems())
    {
      if (connect() == false)
      {
        CCDEBUG("Connection Lost, stop process");
        break;
      }
      else if (m_bLogin == false)
      {
        CCDEBUG("Login not yet done, stop process");
        break;
      }
      else
      {
        CcSyncFileInfo oFileInfo;
        uint64 uiQueueIndex = 0;
        m_oDatabase.beginTransaction();
        EBackupQueueType eQueueType = oDirectory.queueGetNext(oFileInfo, uiQueueIndex);
        switch (eQueueType)
        {
          case EBackupQueueType::AddDir:
            doAddDir(oDirectory, oFileInfo, uiQueueIndex);
            break;
          case EBackupQueueType::RemoveDir:
            doRemoveDir(oDirectory, oFileInfo, uiQueueIndex);
            break;
          case EBackupQueueType::UpdateDir:
            doUpdateDir(oDirectory, oFileInfo, uiQueueIndex);
            break;
          case EBackupQueueType::DownloadDir:
            doDownloadDir(oDirectory, oFileInfo, uiQueueIndex);
            break;
          case EBackupQueueType::AddFile:
            doAddFile(oDirectory, oFileInfo, uiQueueIndex);
            break;
          case EBackupQueueType::RemoveFile:
            doRemoveFile(oDirectory, oFileInfo, uiQueueIndex);
            break;
          case EBackupQueueType::UpdateFile:
            doUpdateFile(oDirectory, oFileInfo, uiQueueIndex);
            break;
          case EBackupQueueType::DownloadFile:
            doDownloadFile(oDirectory, oFileInfo, uiQueueIndex);
            break;
          default:
            oDirectory.queueIncrementItem(uiQueueIndex);
        }
        m_oDatabase.endTransaction();
      }
    }
  }
}

void CcSyncClient::close()
{
  if (m_pSocket != nullptr)
  {
    m_oRequest.init(ESyncCommandType::Close);
    sendRequestGetResponse();
    CCMONITORDELETE(m_pSocket);
    delete m_pSocket;
  }
  m_bLogin = false;
}

bool CcSyncClient::selectAccount(const CcString& sNewAccount)
{
  bool bRet = false;
  if (m_pAccount != nullptr)
  {
    m_pAccount = nullptr;
    m_oBackupDirectories.clear();
  }
  if (sNewAccount.length() == 0)
    m_pAccount = m_oConfig.getFirstAccountConfig();
  else
    m_pAccount = m_oConfig.getAccountConfig(sNewAccount);
  if (m_pAccount != nullptr)
  {
    if (setupDatabase())
    {
      bRet = m_pAccount->isValid();
    }
  }
  return bRet;
}

void CcSyncClient::init(const CcString& sConfigFile, const CcString& sAccountName)
{
  if (!m_oConfig.readConfig(sConfigFile))
  {
    CCDEBUG("No configuration file in config-directory");
  }
  selectAccount(sAccountName);
}


bool CcSyncClient::setupDatabase()
{
  bool bRet = false;
  m_sDatabaseFile = CcKernel::getConfigDir();
  m_sDatabaseFile.appendPath(CcSyncGlobals::ConfigDirName);
  m_sDatabaseFile.appendPath(m_pAccount->getDatabaseFilePath());
  if (m_oDatabase.openDatabase(m_sDatabaseFile))
  {
    if (!checkSqlTables())
    {
      setupSqlTables();
    }
    CCDEBUG("User Database connected: " + m_pAccount->getDatabaseFilePath());
    bRet = true;
  }
  else
    CCERROR("Unable to create Database for " + m_pAccount->getDatabaseFilePath());
  return bRet;
}

bool CcSyncClient::checkSqlTables()
{
  bool bRet = true;
  CcSqlResult oResult;
  for (CcSyncDirectoryConfig& oConfigDirectory : m_pAccount->getDirectoryList())
  {
    CcSyncDirectory oDirectory(&m_oDatabase, &oConfigDirectory);
    bRet &= oDirectory.validate();
    m_oBackupDirectories.append(oDirectory);
  }
  return bRet;
}

bool CcSyncClient::setupSqlTables()
{
  bool bRet = true;
  return bRet;
}

bool CcSyncClient::connect()
{
  bool bRet = false;
  if (m_pSocket == nullptr)
  {
    m_pSocket = new CcSslSocket(); 
    CCMONITORNEW(m_pSocket);
    if (m_pSocket->initClient())
    {
      if (m_pSocket->connect(m_pAccount->getServer().getHostname(), m_pAccount->getServer().getPortString()))
      {
        m_pSocket->setTimeout(CcDateTimeFromSeconds(30));
        bRet = true;
      }
      else
      {
        CCMONITORDELETE(m_pSocket);
        delete m_pSocket;
        m_pSocket = nullptr;
      }
    }
    else
    {
      CCMONITORDELETE(m_pSocket);
      delete m_pSocket;
      m_pSocket = nullptr;
    }
  }
  else
  {
    bRet = true;
  }
  return bRet;
}

bool CcSyncClient::sendRequestGetResponse()
{
  bool bRet = false;
  CcJsonDocument oJsonDoc(m_oRequest.getData());
  if (connect())
  {
    m_oResponse.clear();
    if (m_pSocket->writeArray(oJsonDoc.getJsonDocument()))
    {
      if (m_oRequest.getCommandType() != ESyncCommandType::Close)
      {
        CcByteArray oRead;
        size_t uiReadSize = 0;
        do
        {
          CcByteArray oLastRead(CcSyncGlobals::MaxResponseSize);
          uiReadSize = m_pSocket->readArray(oLastRead);
          oRead.append(oLastRead);
        } while ( oRead.size()  <  CcSyncGlobals::MaxResponseSize  &&
                  oRead.size()  >  0                               &&
                  oRead.last()  != '\0');

        if (uiReadSize > CcSyncGlobals::MaxResponseSize)
        {
          oRead.clear();
          CCERROR("Read from socket failed, close connection");
          close();
        }

        m_oResponse.parseData(oRead);
        if (m_oResponse.getCommandType() == m_oRequest.getCommandType())
        {
          bRet = true;
        }
        else
        {
          CCERROR("Wrong server response");
        }
      }
    }
    else
    {
      CCERROR("Get response failed");
      close();
    }
  }
  return bRet;
}

void CcSyncClient::recursiveRemoveDirectory(CcSyncDirectory& oDirectory, const CcSyncFileInfo& oFileInfo)
{
  CcSyncFileInfoList oClientDirectories = oDirectory.getDirectoryInfoListById(oFileInfo.getId());
  CcSyncFileInfoList oClientFiles = oDirectory.getFileInfoListById(oFileInfo.getId());

  for (CcSyncFileInfo& oClientFileInfo : oClientFiles)
  {
    CcString sPath = oDirectory.getFullFilePathById(oClientFileInfo.getId());
    oDirectory.fileListRemove(oClientFileInfo, false);
  }
  for (CcSyncFileInfo& oClientDirInfo : oClientDirectories)
  {
    recursiveRemoveDirectory(oDirectory, oClientDirInfo);
  }

  CcString sPath = oDirectory.getFullDirPathById(oFileInfo.getId());
  oDirectory.directoryListRemove(oFileInfo, false);
  if (CcDirectory::exists(sPath))
  {
    CcDirectory::remove(sPath);
  }
}

bool CcSyncClient::sendFile(const CcString& sPath)
{
  bool bRet = false;
  CcFile oFile(sPath);
  CcCrc32 oCrc;
  if(oFile.open(EOpenFlags::Read | EOpenFlags::ShareRead))
  {
    bool bTransfer = true;
    while (bTransfer)
    {
      CcByteArray oBuffer(CcSyncGlobals::TransferSize);
      oFile.readArray(oBuffer);
      if (oBuffer.size() > 0)
      {
        oCrc.append(oBuffer);
        if (m_pSocket->writeArray(oBuffer) == false)
        {
          bTransfer = false;
          CCERROR("Write failed during File transfer, stop connection");
          close();
        }
      }
      else
      {
        bRet = true;
        bTransfer = false;
      }
    }
    oFile.close();
  }
  if (bRet == true)
  {
    m_oRequest.setCrc(oCrc);
    if (sendRequestGetResponse())
    {
      if (m_oResponse.hasError() == false)
        bRet = true;
      else
        bRet = false;
    }
    else
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcSyncClient::receiveFile(CcFile* pFile, CcSyncFileInfo& oFileInfo)
{
  bool bRet = false;
  bool bTransfer = true;
  CcCrc32 oCrc;
  size_t uiReceived = 0;
  while (bTransfer)
  {
    if (uiReceived < oFileInfo.getSize())
    {
      size_t uiBufferSize = CcSyncGlobals::TransferSize;
      if (oFileInfo.getSize() - uiReceived < CcSyncGlobals::TransferSize)
      {
        uiBufferSize = oFileInfo.getSize() - uiReceived;
      }
      CcByteArray oByteArray(uiBufferSize);
      size_t uiReadSize = m_pSocket->readArray(oByteArray);
      if (uiReadSize <= uiBufferSize)
      {
        oCrc.append(oByteArray);
        uiReceived += uiReadSize;
        if (!pFile->writeArray(oByteArray))
        {
          bRet = false;
          bTransfer = false;
        }
      }
      else
      {
        bTransfer = false;
        CCERROR("Error during socket read, close connection");
        close();
      }
    }
    else
    {
      bTransfer = false;
      m_oRequest.setCrc(oCrc);
      if (sendRequestGetResponse())
      {
        if (m_oResponse.hasError() == false)
          bRet = true;
        else
          bRet = false;
      }
      else
      {
        bRet = false;
      }
    }
  }
  return bRet;
}

bool CcSyncClient::doServerUpdate(CcSyncDirectory& oDirectory, uint64 uiDirId)
{
  bool bRet = false;
  m_oRequest.setDirectoryGetFileList(oDirectory.getName(), uiDirId);
  if (sendRequestGetResponse() &&
      m_oResponse.hasError() == false)
  {
    bRet = true;
    CcSyncFileInfoList oServerDirectories;
    CcSyncFileInfoList oServerFiles;
    m_oResponse.getDirectoryDirectoryInfoList(oServerDirectories, oServerFiles);
    CcSyncFileInfoList oClientDirectories = oDirectory.getDirectoryInfoListById(uiDirId);
    CcSyncFileInfoList oClientFiles = oDirectory.getFileInfoListById(uiDirId);
    for (CcSyncDirInfo& oServerDirInfo : oServerDirectories)
    {
      if (oClientDirectories.containsFile(oServerDirInfo.getId()))
      {
        CcSyncDirInfo oClientDirInfo = oClientDirectories.getFile(oServerDirInfo.getId());
        // Compare Server Directory with Client Directory
        if (oClientDirInfo != oServerDirInfo)
        {
          if (oClientDirInfo.getName() != oServerDirInfo.getName())
          {
            CcString sPathFrom = oDirectory.getFullDirPathById(oClientDirInfo.getId());
            CcString sPathTo = oDirectory.getFullDirPathById(oServerDirInfo.getDirId()).appendPath(oServerDirInfo.getName());
            if (!CcDirectory::move(sPathFrom, sPathFrom))
            {
              CCERROR("Failed to move directory: " + sPathFrom + " > " + sPathTo);
            }
          }
          oDirectory.directoryUpdate(oServerDirInfo);
          CCDEBUG("Directory not up to date: " + oServerDirInfo.getName());
          doServerUpdate(oDirectory, oServerDirInfo.getId());
        }
        // Remove Directory from current list
        oClientDirectories.removeFile(oServerDirInfo.getId());
      }
      else
      {
        CcString sPathToDir = oDirectory.getFullDirPathById(oServerDirInfo.getDirId());
        sPathToDir.appendPath(oServerDirInfo.getName());
        if (CcDirectory::exists(sPathToDir))
        {
          if (oDirectory.directoryInsert(oServerDirInfo))
          {
            doServerUpdate(oDirectory, oServerDirInfo.getId());
          }
          else
          {
            oDirectory.queueDownloadDirectory(oServerDirInfo);
          }
        }
        else
        {
          oDirectory.queueDownloadDirectory(oServerDirInfo);
        }
      }
    }

    // Search Filelist
    for (CcSyncFileInfo& oServerFileInfo : oServerFiles)
    {
      if (oClientFiles.containsFile(oServerFileInfo.getId()))
      {
        const CcSyncFileInfo& oFileInfo = oClientFiles.getFile(oServerFileInfo.getId());
        if (oFileInfo != oServerFileInfo)
        {
          oDirectory.queueUpdateFile(oServerFileInfo);
        }
        oClientFiles.removeFile(oServerFileInfo.getId());
      }
      else if (oClientFiles.containsFile(oServerFileInfo.getName()))
      {
        const CcSyncFileInfo& oClientFileInfo = oClientFiles.getFile(oServerFileInfo.getName());
        if (oDirectory.fileListRemove(oClientFileInfo))
        {
          oDirectory.queueDownloadFile(oServerFileInfo);
        }
        // Remove Directory from current list
        oClientFiles.removeFile(oServerFileInfo.getName());
      }
      else
      {
        CcString sPathToFile = oDirectory.getFullDirPathById(oServerFileInfo.getDirId());
        sPathToFile.appendPath(oServerFileInfo.getName());
        if (CcFile::exists(sPathToFile))
        {
          CcCrc32 oFileCrc = CcFile::getCrc32(sPathToFile);
          if (oFileCrc.getValue() == oServerFileInfo.getCrc() &&
              oDirectory.fileListInsert(oServerFileInfo))
          {
            CCDEBUG("Existing file added to list: " + oServerFileInfo.getName());
          }
          else
          {
              oDirectory.queueUpdateFile(oServerFileInfo);
          }
        }
        else
        {
          oDirectory.queueDownloadFile(oServerFileInfo);
        }
      }
    }
    // remove all not listed directories on server
    for (CcSyncFileInfo& oClientFileInfo : oClientFiles)
    {
      oDirectory.fileListRemove(oClientFileInfo, false);
    }
    // remove all not listed files on server
    for (CcSyncFileInfo& oClientDirInfo : oClientDirectories)
    {
      recursiveRemoveDirectory(oDirectory, oClientDirInfo);
    }
    oDirectory.directoryUpdateChanged(uiDirId);
  }
  return bRet;
}

bool CcSyncClient::serverDirectoryEqual(CcSyncDirectory& oDirectory, uint64 uiDirId)
{
  bool bRet = false;
  m_oRequest.setDirectoryGetDirectoryInfo(oDirectory.getName(), uiDirId);
  if (sendRequestGetResponse() &&
      m_oResponse.hasError() == false)
  {
    CcSyncDirInfo oDirInfoServer = m_oResponse.getFileInfo();
    CcSyncDirInfo oDirInfoClient = oDirectory.getDirectoryInfoById(uiDirId);
    if (oDirInfoServer == oDirInfoClient)
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcSyncClient::doAddDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oDirInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  CcString sFilePath = oDirectory.getFullDirPathById(oDirInfo.getDirId());
  sFilePath.appendPath(oDirInfo.getName());
  if (oDirInfo.fromSystemFile(sFilePath))
  {
    m_oRequest.setDirectoryCreateDirectory(oDirectory.getName(), oDirInfo);
    if (sendRequestGetResponse() &&
      m_oResponse.hasError() == false)
    {
      bRet = true;
      CcSyncFileInfo oResponseFileInfo = m_oResponse.getFileInfo();
      oDirectory.directoryInsert(oResponseFileInfo);
      oDirectory.queueFinalizeDirectory(oResponseFileInfo, uiQueueIndex);
      CCDEBUG("Directory successfully added: " + oDirInfo.getName());
    }
    else
    {
      oDirectory.queueIncrementItem(uiQueueIndex);
      CCERROR("Directory failed to add:  " + oDirInfo.getName());
      CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
    }
  }
  else
  {
    oDirectory.queueIncrementItem(uiQueueIndex);
    CCERROR("Queued Directory not found:  " + sFilePath);
    CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
  }
  return bRet;
}

bool CcSyncClient::doRemoveDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oDirInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  m_oRequest.setDirectoryRemoveDirectory(oDirectory.getName(), oDirInfo);
  if (sendRequestGetResponse() )
  {
    if (m_oResponse.hasError() == false)
    {
      oDirectory.directoryListRemove(oDirInfo);
      oDirectory.queueFinalizeDirectory(oDirInfo, uiQueueIndex);
      bRet = true;
    }
    else
    {
      switch (m_oResponse.getError().getError())
      {
        case EError::FSDirNotFound:
          oDirectory.directoryListRemove(oDirInfo);
          oDirectory.queueFinalizeDirectory(oDirInfo, uiQueueIndex);
          bRet = true;
          break;
        default:
          CCERROR("Error on deleting Directory: " + oDirInfo.getName());
          CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
          oDirectory.queueIncrementItem(uiQueueIndex);
      }
    }
  }
  else
  {
    CCERROR("RemoveDirectory request failed: " + oDirInfo.getName());
    CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
    oDirectory.queueIncrementItem(uiQueueIndex);
  }
  return bRet;
}

bool CcSyncClient::doUpdateDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  CCUNUSED(oDirectory);
  CCUNUSED(oFileInfo);
  CCUNUSED(uiQueueIndex);
  CCDEBUG("@TODO Implementation");
  oDirectory.queueIncrementItem(uiQueueIndex);
  return bRet;
}

bool CcSyncClient::doDownloadDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  m_oRequest.setDirectoryGetDirectoryInfo(oDirectory.getName(), oFileInfo.getId());
  if (sendRequestGetResponse())
  {
    CcSyncDirInfo oDirInfo = m_oResponse.getFileInfo();
    CcString sFullDirPath = oDirectory.getFullDirPathById(oDirInfo.getDirId());
    sFullDirPath.appendPath(oDirInfo.getName());
    CcDirectory oDirectoryHandl(sFullDirPath);
    if (oDirectoryHandl.exists() ||
        oDirectoryHandl.create())
    {
      oDirectoryHandl.setUserId(oFileInfo.getUserId());
      oDirectoryHandl.setGroupId(oFileInfo.getGroupId());
      if (oDirectory.directoryInsert(oDirInfo))
      {
        CCDEBUG("Directory Added: " + oDirInfo.getName());
        bRet = true;
        oDirectory.queueFinalizeDirectory(oDirInfo, uiQueueIndex);
        doServerUpdate(oDirectory, oDirInfo.id());
      }
      else
      {
        CCERROR("Directory not inserted in list found");
        CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
        oDirectory.queueIncrementItem(uiQueueIndex);
      }
    }
    else
    {
      CCERROR("Unable to create directory");
      oDirectory.queueIncrementItem(uiQueueIndex);
    }
  }
  else
  {
    CCERROR("Directory not found");
    CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
    oDirectory.queueIncrementItem(uiQueueIndex);
  }
  return bRet;
}

bool CcSyncClient::doAddFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  CcString sFilePath = oDirectory.getFullDirPathById(oFileInfo.getDirId());
  sFilePath.appendPath(oFileInfo.getName());
  if (oFileInfo.fromSystemFile(sFilePath))
  {
    m_oRequest.setDirectoryUploadFile(oDirectory.getName(), oFileInfo);
    if (sendRequestGetResponse())
    {
      if (m_oResponse.hasError() == false)
      {
        if (sendFile(sFilePath))
        {
          CcSyncFileInfo oResponseFileInfo = m_oResponse.getFileInfo();
          if (oDirectory.fileListInsert(oResponseFileInfo))
          {
            oDirectory.queueFinalizeFile(uiQueueIndex);
            CCDEBUG("File Successfully added: " + oFileInfo.getName());
          }
          else
          {
            oDirectory.queueIncrementItem(uiQueueIndex);
            CCERROR("Inserting to filelist failed: " + sFilePath);
          }
          bRet = true;
        }
        else
        {
          oDirectory.queueIncrementItem(uiQueueIndex);
          CCERROR("Sending file failed: " + sFilePath);
          CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
        }
      }
      else
      {
        switch (m_oResponse.getError().getError())
        {
          case EError::FSFileAlreadyExisting:
            if (m_oResponse.data().contains(CcSyncGlobals::FileInfo::Id))
            {
              CcSyncFileInfo oResponseFileInfo = m_oResponse.getFileInfo();
              oDirectory.fileListInsert(oResponseFileInfo);
              oDirectory.queueFinalizeFile(uiQueueIndex);
              CCDEBUG("File Successfully added: " + oFileInfo.getName());
              bRet = true;
              break;
            }
          default:
            oDirectory.queueIncrementItem(uiQueueIndex);
            CCERROR("Sending file failed: " + sFilePath);
            CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
        }
      }
    }
    else
    {
      CCERROR("File Upload Request failed: " + sFilePath);
      CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
      oDirectory.queueIncrementItem(uiQueueIndex);
    }
  }
  else
  {
    oDirectory.queueIncrementItem(uiQueueIndex);
    CCERROR("Queued Directory not found: " + sFilePath);
    CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
  }
  return bRet;
}

bool CcSyncClient::doRemoveFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  m_oRequest.setDirectoryRemoveFile(oDirectory.getName(), oFileInfo);
  if (sendRequestGetResponse())
  {
    if (m_oResponse.hasError() == false)
    {
      if(oDirectory.fileListRemove(oFileInfo))
      {
        oDirectory.queueFinalizeFile(uiQueueIndex);
        CCDEBUG("File successfully removed: " + oFileInfo.getName());
        bRet = true;
      }
      else
      {
        CCERROR("Error on deleting File in Directory: " + oFileInfo.getName());
        CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
        oDirectory.queueIncrementItem(uiQueueIndex);
      }
    }
    else
    {
      switch (m_oResponse.getError().getError())
      {
        case EError::FSFileNotFound:
          oDirectory.fileListRemove(oFileInfo);
          oDirectory.queueFinalizeFile(uiQueueIndex);
          CCDEBUG("File successfully removed: " + oFileInfo.getName());
          bRet = true;
          break;
        default:
          CCERROR("Error on deleting File: " + oFileInfo.getName());
          CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
          oDirectory.queueIncrementItem(uiQueueIndex);
      }
    }
  }
  else
  {
    CCERROR("RemoveFile request failed: " + oFileInfo.getName());
    CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
    oDirectory.queueIncrementItem(uiQueueIndex);
  }
  return bRet;
}

bool CcSyncClient::doUpdateFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  bool bDoRemoveUpload = false;
  m_oRequest.setDirectoryGetFileInfo(oDirectory.getName(), oFileInfo.getId());
  if (sendRequestGetResponse() &&
      m_oResponse.hasError() == false)
  {
    CcSyncFileInfo oClientFileInfo = oDirectory.getFileInfoById(oFileInfo.getId());
    CcString sCurrentFilePath = oDirectory.getFullFilePathById(oClientFileInfo.id());
    // update ClientFileInfo with current stored file
    oClientFileInfo.fromSystemFile(sCurrentFilePath);

    CcSyncFileInfo oServerFileInfo = m_oResponse.getFileInfo();
    if (oClientFileInfo.modified() > oServerFileInfo.modified())
    {
      if (oClientFileInfo.getSize() == oServerFileInfo.getSize() &&
          oClientFileInfo.getCrc()  == oServerFileInfo.getCrc())
      {
        if (oDirectory.fileListUpdate(oServerFileInfo))
        {
          CcFile::setUserId(sCurrentFilePath, oServerFileInfo.getUserId());
          CcFile::setGroupId(sCurrentFilePath, oServerFileInfo.getGroupId());
          CcFile::setModified(sCurrentFilePath, CcDateTimeFromSeconds(oServerFileInfo.getModified()));
          CCDEBUG("Update file, update in database succeeded: " + oServerFileInfo.getName());
          oDirectory.queueFinalizeFile(uiQueueIndex);
        }
        else
        {
          CCDEBUG("Update file, update in database failed: " + oServerFileInfo.getName());
          oDirectory.queueIncrementItem(uiQueueIndex);
        }
      }
      else
      {
        bDoRemoveUpload = true;
      }
    }
    else if (oClientFileInfo.modified() < oServerFileInfo.modified())
    {
      if (oClientFileInfo.getCrc() == oServerFileInfo.getCrc())
      {
        if(oDirectory.fileListUpdate(oServerFileInfo))
        {
          CcFile::setUserId(sCurrentFilePath, oServerFileInfo.getUserId());
          CcFile::setGroupId(sCurrentFilePath, oServerFileInfo.getGroupId());
          CcFile::setModified(sCurrentFilePath, CcDateTimeFromSeconds(oServerFileInfo.getModified()));
          CCDEBUG("Update file, update in database succeeded: " + oServerFileInfo.getName());
          oDirectory.queueFinalizeFile(uiQueueIndex);
        }
        else
        {
          CCDEBUG("Update file, update in database failed: " + oServerFileInfo.getName());
          oDirectory.queueIncrementItem(uiQueueIndex);
        }
      }
      else
      {
        if(oDirectory.fileListRemove(oFileInfo, false))
        {
          bRet = doAddFile(oDirectory, oFileInfo, 0);
          if (bRet == true)
          {
            oDirectory.queueFinalizeFile(uiQueueIndex);
          }
          else
          {
            oDirectory.queueIncrementItem(uiQueueIndex);
          }
        }
        else
        {
          CCDEBUG("Update file, remove database file failed: " + oFileInfo.getName());
          oDirectory.queueIncrementItem(uiQueueIndex);
        }
      }
    }
    else
    {
      if(oClientFileInfo == oServerFileInfo)
      {
        CCDEBUG("Files are the same");
        oDirectory.queueFinalizeFile(uiQueueIndex);
      }
      else
      {
        if (oDirectory.fileListRemove(oFileInfo, false))
        {
          bRet = doAddFile(oDirectory, oFileInfo, 0);
          if (bRet == true)
          {
            CCDEBUG("Files info updated: " + oServerFileInfo.getName());
            oDirectory.queueFinalizeFile(uiQueueIndex);
          }
          else
          {
            CCDEBUG("Files update failed: " + oServerFileInfo.getName());
            oDirectory.queueIncrementItem(uiQueueIndex);
          }
        }
      }
    }
  }
  else
  {
    bDoRemoveUpload = true;
  }
  if(bDoRemoveUpload)
  {
    bRet = doRemoveFile(oDirectory, oFileInfo, 0);
    if (bRet == true)
    {
      bRet = doAddFile(oDirectory, oFileInfo, 0);
      if(bRet == true)
      {
        oDirectory.queueFinalizeFile(uiQueueIndex);
      }
    }
  }
  return bRet;
}

bool CcSyncClient::doDownloadFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex)
{
  bool bRet = false;
  m_oRequest.setDirectoryDownloadFile(oDirectory.getName(), oFileInfo.getId());
  if (sendRequestGetResponse())
  {
    oFileInfo = m_oResponse.getFileInfo();
    CcString sFullDirPath = oDirectory.getFullDirPathById(oFileInfo.getDirId());
    if (CcDirectory::exists(sFullDirPath))
    {
      sFullDirPath.appendPath(oFileInfo.getName());
      CcFile oFile(sFullDirPath);
      if (oFile.open(EOpenFlags::Overwrite))
      {
        if (receiveFile(&oFile, oFileInfo))
        {
          oFile.setUserId(oFileInfo.getUserId());
          oFile.setGroupId(oFileInfo.getGroupId());
          oFile.setModified(CcDateTimeFromSeconds( oFileInfo.getModified()));
          if (oDirectory.fileListInsert(oFileInfo))
          {
            CCDEBUG("File downloaded: " + oFileInfo.getName());
            bRet = true;
            oDirectory.queueFinalizeFile(uiQueueIndex);
          }
          else
          {
            CCERROR("Insert to Filelist failed");
            CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
            oDirectory.queueIncrementItem(uiQueueIndex);
          }
        }
        else
        {
          CCERROR("File download failed");
          CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
          oDirectory.queueIncrementItem(uiQueueIndex);
        }
        oFile.close();
      }
      else
      {
        CCERROR("Unable to create file");
        CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
        oDirectory.queueIncrementItem(uiQueueIndex);
      }
    }
    else
    {
      CCERROR("Directory for download not found");
      CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
      oDirectory.queueIncrementItem(uiQueueIndex);
    }
  }
  else
  {
    CCERROR("DownloadFile request failed");
    CCERROR("    ErrorMsg: " + m_oResponse.getErrorMsg());
    oDirectory.queueIncrementItem(uiQueueIndex);
  }
  return bRet;
}
