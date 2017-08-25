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
 * @brief     Implemtation of class CcSyncServerWorker
 */
#include "CcSyncDbClient.h"
#include "CcSyncServerWorker.h"
#include "CcSyncGlobals.h"
#include "CcKernel.h"
#include "CcDirectory.h"
#include "CcSslSocket.h"
#include "CcSyncServer.h"
#include "CcSyncClientConfig.h"
#include "CcSyncDirectory.h"
#include "CcSqlite.h"
#include "Hash/CcCrc32.h"

CcSyncServerWorker::CcSyncServerWorker(CcSyncServer* oServer, CcSocket* oSocket) :
  m_oServer(oServer),
  m_oSocket(oSocket)
{
}

CcSyncServerWorker::~CcSyncServerWorker(void)
{
  if (m_oSocket != nullptr)
  {
    CCMONITORDELETE(m_oSocket); 
    delete m_oSocket;
  }
  if (m_pDirectory != nullptr)
  {
    CCMONITORDELETE(m_pDirectory);
    delete m_pDirectory;
  }
}

void CcSyncServerWorker::run()
{
  while (getThreadState() == EThreadState::Running)
  {
    if (getRequest())
    {
      if (m_oUser.isValid())
        m_oUser.getDatabase()->beginTransaction();
      ESyncCommandType eCommandType = m_oRequest.getCommandType();
      switch (eCommandType)
      {
        case ESyncCommandType::Close:
          m_oSocket->close();
          stop();
          break;
        case ESyncCommandType::ServerGetInfo:
          m_oResponse.init(eCommandType);
          doServerGetInfo();
          break;
        case ESyncCommandType::ServerAccountCreate:
          m_oResponse.init(eCommandType);
          doServerGetInfo();
          break;
        case ESyncCommandType::AccountCreate:
          m_oResponse.init(eCommandType);
          doAccountCreate();
          break;
        case ESyncCommandType::AccountLogin:
          m_oResponse.init(eCommandType);
          doAccountLogin();
          break;
        case ESyncCommandType::AccountGetData:
          m_oResponse.init(eCommandType);
          doAccountGetData();
          break;
        case ESyncCommandType::AccountSetData:
          m_oResponse.init(eCommandType);
          doAccountSetData();
          break;
        case ESyncCommandType::AccountGetDirectoryList:
          m_oResponse.init(eCommandType);
          doAccountGetDirectoryList();
          break;
        case ESyncCommandType::AccountGetCommandList:
          m_oResponse.init(eCommandType);
          doUserGetCommandList();
          break;
        case ESyncCommandType::DirectoryGetFileList:
          m_oResponse.init(eCommandType);
          doDirectoryGetFileList();
          break;
        case ESyncCommandType::DirectoryGetFileInfo:
          m_oResponse.init(eCommandType);
          doDirectoryGetFileInfo();
          break;
        case ESyncCommandType::DirectoryGetDirectoryInfo:
          m_oResponse.init(eCommandType);
          doDirectoryGetDirectoryInfo();
          break;
        case ESyncCommandType::DirectoryCreateDirectory:
          m_oResponse.init(eCommandType);
          doDirectoryCreateDirectory();
          break;
        case ESyncCommandType::DirectoryRemoveDirectory:
          m_oResponse.init(eCommandType);
          doDirectoryRemoveDirectory();
          break;
        case ESyncCommandType::DirectoryUploadFile:
          m_oResponse.init(eCommandType);
          doDirectoryUploadFile();
          break;
        case ESyncCommandType::DirectoryRemoveFile:
          m_oResponse.init(eCommandType);
          doDirectoryRemoveFile();
          break;
        case ESyncCommandType::DirectoryDownloadFile:
          m_oResponse.init(eCommandType);
          doDirectoryDownloadFile();
          break;
        default:
          m_oResponse.init(ESyncCommandType::Unkown);
          m_oResponse.setResult(false);
          m_oResponse.setError(EError::CommandUnknown, "Unkown Command");
      }
      if (m_oUser.isValid())
        m_oUser.getDatabase()->endTransaction();
    }
    else
    {
      m_oResponse.setError(EError::CommandError, "Request malformed. Connection will get closed.");
      sendResponse();
      m_oSocket->close();
      stop();
    }
  }
}

bool CcSyncServerWorker::getRequest()
{
  bool bRet = false;
  CcByteArray oData(CcSyncGlobals::MaxRequestSize);
  m_oSocket->readArray(oData);
  if (m_oRequest.parseData(oData))
  {
    bRet = true;
  }
  else
  {
    m_oResponse.setError(EError::CommandError, "Message malformed");
  }
  return bRet;
}

bool CcSyncServerWorker::sendResponse()
{
  bool bRet = m_oSocket->writeArray(m_oResponse.getBinary());
  if (bRet)
    bRet = (m_oSocket->write("\0", 1) == SIZE_MAX)? false : true;
  return bRet;
}

bool CcSyncServerWorker::loadConfigsBySessionRequest()
{
  bool bRet = false;
  // Check all required data
  if (m_oUser.isValid() )
  { 
    bRet = true;
  }
  else if (m_oRequest.data().contains(CcSyncGlobals::Commands::Session))
  {
    if (loadConfigsBySession(m_oRequest.data()[CcSyncGlobals::Commands::Session].getValue().getString()))
    {
      bRet = true;
    }
    else
    {
      m_oResponse.setError(EError::LoginFailed, "Error: Session not valid");
      bRet = false;
    }
  }
  else
  {
    m_oResponse.setError(EError::LoginFailed, "Error: No login data not available");
  }
  return bRet;
}


bool CcSyncServerWorker::loadConfigsBySession(const CcString& sSession)
{
  bool bRet = false;
  m_oUser = m_oServer->getUserByToken(sSession);
  if (m_oUser.isValid())
  {
    bRet = true;
  }
  else
  {
    m_oResponse.setError(EError::LoginFailed, "Error: Login data not valid");
  }
  return bRet;
}

bool CcSyncServerWorker::receiveFile(CcFile* pFile, CcSyncFileInfo& oFileInfo)
{
  bool bRet = false;
  bool bTransfer = true;
  CcCrc32 oCrc;
  uint64 uiReceived = 0;
  while (bTransfer)
  {
    if (uiReceived < oFileInfo.getSize())
    {
      size_t uiBufferSize = CcSyncGlobals::TransferSize;
      if (oFileInfo.getSize() - uiReceived < CcSyncGlobals::TransferSize)
      {
        uiBufferSize = static_cast<size_t>(oFileInfo.getSize() - uiReceived);
      }
      CcByteArray oByteArray(uiBufferSize);
      m_oSocket->readArray(oByteArray);
      if (oByteArray.size() > 0)
      {
        oCrc.append(oByteArray);
        uiReceived += oByteArray.size();
        if (!pFile->writeArray(oByteArray))
        {
          bRet = false;
          bTransfer = false;
        }
      }
      else
      {
        bTransfer = false;
      }
    }
    else
    {
      oFileInfo.crc() = oCrc.getValue();
      bTransfer = false;
      CcByteArray oByteArray(CcSyncGlobals::TransferSize);
      m_oSocket->readArray(oByteArray);
      m_oRequest.parseData(oByteArray);
      if (m_oRequest.getCommandType() == ESyncCommandType::Crc)
      {
        if (m_oRequest.getCrc() == oCrc)
        {
          bRet = true;
        }
        else
        {
          bRet = false;
        }
      }
      else
      {
        bRet = false;
      }
    }
  }
  return bRet;
}

bool CcSyncServerWorker::sendFile(const CcString& sPath)
{
  bool bRet = false;
  CcFile oFile(sPath);
  CcCrc32 oCrc;
  if (oFile.open(EOpenFlags::Read | EOpenFlags::ShareRead))
  {
    bool bTransfer = true;
    while (bTransfer)
    {
      CcByteArray oBuffer(CcSyncGlobals::TransferSize);
      oFile.readArray(oBuffer);
      if (oBuffer.size() > 0)
      {
        oCrc.append(oBuffer);
        if (m_oSocket->writeArray(oBuffer) == false)
        {
          bTransfer = false;
        }
      }
      else
      {
        bRet = true;
        bTransfer = false;
      }
    }
  }
  if (bRet == true)
  {
    CcByteArray oByteArray(CcSyncGlobals::TransferSize);
    m_oSocket->readArray(oByteArray);
    m_oRequest.parseData(oByteArray);
    if (m_oRequest.getCommandType() == ESyncCommandType::Crc)
    {
      if (m_oRequest.getCrc() == oCrc)
      {
        bRet = true;
      }
      else
      {
        bRet = false;
      }
    }
    else
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcSyncServerWorker::loadDirectory()
{
  bool bRet = false;
  // Check all required data
  if (m_oRequest.data().contains(CcSyncGlobals::Commands::DirectoryGetFileList::DirectoryName))
  {
    CcString sDirectoryName = m_oRequest.data()[CcSyncGlobals::Commands::DirectoryGetFileList::DirectoryName].getValue().getString();
    if (m_pDirectory != nullptr)
    {
      if (m_pDirectory->getName() == sDirectoryName)
      {
        bRet = true;
      }
      else
      {
        CCMONITORDELETE(m_pDirectory);
        delete m_pDirectory;
      }
    }
    if (m_pDirectory == nullptr)
    {
      CcSyncDirectoryConfig* pDirectory = m_oUser.getAccountConfig()->DirectoryList().getDirectoryByName(sDirectoryName);
      if (pDirectory != nullptr)
      {
        m_pDirectory = new CcSyncDirectory(m_oUser.getDatabase(), pDirectory);
        CCMONITORNEW(m_pDirectory);
        bRet = true;
      }
      else
      {
        m_oResponse.setError(EError::ConfigFolderNotFound, "Requested Directory not found.");
      }
    }
  }
  else
  {
    m_oResponse.setError(EError::CommandRequiredParameter, "No Directory specified.");
  }
  return bRet;
}

void CcSyncServerWorker::doServerGetInfo()
{
  m_oResponse.setError(EError::CommandNotImplemented, "Not yet implemented");
  sendResponse();
}

void CcSyncServerWorker::doServerAccountCreate()
{
  m_oResponse.setError(EError::CommandNotImplemented, "Not yet implemented");
  sendResponse();
}

void CcSyncServerWorker::doAccountCreate()
{
  m_oResponse.setError(EError::CommandNotImplemented, "Not yet implemented");
  sendResponse();
}

void CcSyncServerWorker::doAccountLogin()
{
  if (m_oRequest.getData().contains(CcSyncGlobals::Commands::AccountLogin::Account) &&
      m_oRequest.getData().contains(CcSyncGlobals::Commands::AccountLogin::Username) &&
      m_oRequest.getData().contains(CcSyncGlobals::Commands::AccountLogin::Password) )
  {
    CcString sAccount = m_oRequest.data()[CcSyncGlobals::Commands::AccountLogin::Account].getValue().getString();
    CcString sUserName = m_oRequest.data()[CcSyncGlobals::Commands::AccountLogin::Username].getValue().getString();
    CcString sPassword = m_oRequest.data()[CcSyncGlobals::Commands::AccountLogin::Password].getValue().getString();
    CcSyncUser oUser = m_oServer->loginUser(sAccount, sUserName, sPassword);
    if (oUser.isValid())
    {
      m_oUser = oUser;
      m_oResponse.setLogin(oUser.getToken());
    }
    else
    {
      CCDEBUG("AccountLogin failed: " + sUserName);
      m_oResponse.setError(EError::LoginFailed, "Login Failed");
    }
  }
  else
  {
    CCDEBUG("AccountLogin failed due to wrong parameters");
    m_oResponse.setError(EError::CommandRequiredParameter, "User login data missing.");
  }
  sendResponse();
}

void CcSyncServerWorker::doAccountGetData()
{
  if (loadConfigsBySessionRequest())
  {
    m_oResponse.addAccountInfo(*m_oUser.getAccountConfig());
  }
  sendResponse();
}

void CcSyncServerWorker::doAccountSetData()
{
  m_oResponse.setError(EError::CommandNotImplemented, "Not yet implemented");
  sendResponse();
}

void CcSyncServerWorker::doAccountGetDirectoryList()
{
  m_oResponse.setError(EError::CommandNotImplemented, "Not yet implemented");
  sendResponse();
}

void CcSyncServerWorker::doUserGetCommandList()
{
  m_oResponse.setError(EError::CommandNotImplemented, "Not yet implemented");
  sendResponse();
}

void CcSyncServerWorker::doDirectoryGetFileList()
{
  // Check all required data
  if (loadConfigsBySessionRequest() &&
      loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::Commands::DirectoryGetFileList::Id))
    {
      size_t uiId = m_oRequest.data()[CcSyncGlobals::Commands::DirectoryGetFileList::Id].getValue().getSize();
      CcSyncFileInfoList oDirectoryInfos = m_pDirectory->getDirectoryInfoListById(uiId);
      CcSyncFileInfoList oFileInfos      = m_pDirectory->getFileInfoListById(uiId);
      m_oResponse.addDirectoryDirectoryInfoList(oDirectoryInfos, oFileInfos);
    }
    else
    {
      m_oResponse.setError(EError::CommandRequiredParameter, "At least one parameter is missing.");
    }
  }
  sendResponse();
}

void CcSyncServerWorker::doDirectoryGetFileInfo()
{
  // Check all required data
  if (loadConfigsBySessionRequest() &&
      loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::Commands::DirectoryGetFileList::Id))
    {
      uint64 uiFileId = m_oRequest.data()[CcSyncGlobals::Commands::DirectoryGetFileInfo::Id].getValue().getUint64();
      CcSyncFileInfo oDirInfo = m_pDirectory->getFileInfoById(uiFileId);
      m_oResponse.addFileInfo(oDirInfo);
    }
    else
    {
      m_oResponse.setError(EError::CommandRequiredParameter, "Required parameters not found.");
    }
  }
  sendResponse();
}

void CcSyncServerWorker::doDirectoryGetDirectoryInfo()
{
  if (loadConfigsBySessionRequest() &&
    loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::Commands::DirectoryGetDirectoryInfo::Id))
    {
      uint64 uiDirId = m_oRequest.data()[CcSyncGlobals::Commands::DirectoryGetDirectoryInfo::Id].getValue().getUint64();
      CcSyncDirInfo oDirInfo = m_pDirectory->getDirectoryInfoById(uiDirId);
      m_oResponse.addFileInfo(oDirInfo);
    }
    else
    {
      m_oResponse.setError(EError::CommandRequiredParameter, "At least one parameter is missing.");
    }
  }
  sendResponse();
}

void CcSyncServerWorker::doDirectoryCreateDirectory()
{
  if (loadConfigsBySessionRequest() &&
    loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::FileInfo::DirId) &&
        m_oRequest.data().contains(CcSyncGlobals::FileInfo::Name) &&
        m_oRequest.data().contains(CcSyncGlobals::FileInfo::Modified))
    {
      CcSyncFileInfo oFileInfo = m_oRequest.getFileInfo();
      if (m_pDirectory->directorySubDirExists(oFileInfo.getDirId(), oFileInfo.getName()) == false)
      {
        if (m_pDirectory->directoryCreate(oFileInfo))
        {
          m_oResponse.addFileInfo(oFileInfo);
        }
        else
        {
          m_oResponse.setError(EError::FSDirCreateFailed, "Error on creating sub directory.");
        }
      }
      else
      {
        // Directory already exits, turn back Info
        oFileInfo = m_pDirectory->getDirectoryInfoFromSubdir(oFileInfo.getDirId(), oFileInfo.getName());
        if (oFileInfo.getName().length() > 0)
        {
          m_oResponse.addFileInfo(oFileInfo);
        }
        else
        {
          m_oResponse.setError(EError::FSDirAlreadyExists, "Enexpected: Directory already exists, but not findable?");
        }
      }
    }
    else
    {
      m_oResponse.setError(EError::CommandRequiredParameter, "At least one parameter is missing.");
    }
  }
  sendResponse();
}

void CcSyncServerWorker::doDirectoryRemoveDirectory()
{
  if (loadConfigsBySessionRequest() &&
      loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::FileInfo::Id) &&
        m_oRequest.data().contains(CcSyncGlobals::FileInfo::Name))
    {
      CcSyncFileInfo oFileInfo = m_oRequest.getFileInfo();
      if (m_pDirectory->directoryExists(oFileInfo.getId()))
      {
        if (m_pDirectory->directoryEmpty(oFileInfo.getId()))
        {
          m_pDirectory->directoryListRemoveWithHistory(oFileInfo);
        }
        else
        {
          m_oResponse.setError(EError::FSDirNotEmpty, "Directory is not empty");
        }
      }
      else
      {
        m_oResponse.setError(EError::FSDirNotFound, "Directory Not Found in database");
      }
    }
    else
    {
      m_oResponse.setError(EError::CommandRequiredParameter, "At least one parameter is missing.");
    }
  }
  sendResponse();
}

void CcSyncServerWorker::doDirectoryUploadFile()
{
  if (loadConfigsBySessionRequest() &&
      loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::FileInfo::DirId) &&
      m_oRequest.data().contains(CcSyncGlobals::FileInfo::Name) &&
      m_oRequest.data().contains(CcSyncGlobals::FileInfo::Modified))
    {
      CcSyncFileInfo oFileInfo = m_oRequest.getFileInfo();
      if (m_pDirectory->directoryExists(oFileInfo.getDirId()))
      {
        if (m_pDirectory->fileInDirExists(oFileInfo.getDirId(), oFileInfo) == false)
        {
          CcString sFullPath = m_pDirectory->getFullDirPathById(oFileInfo.getDirId());
          sFullPath.appendPath(oFileInfo.getName());
          CcFile oFile(sFullPath);
          if (oFile.open(EOpenFlags::Overwrite))
          {
            sendResponse();
            if (receiveFile(&oFile, oFileInfo))
            {
              oFile.close();
              if (m_pDirectory->fileListCreate(oFileInfo))
              {
                m_oResponse.init(ESyncCommandType::Crc);
                m_oResponse.addFileInfo(oFileInfo);
              }
              else
              {
                oFile.remove(sFullPath);
                m_oResponse.init(ESyncCommandType::Crc);
                m_oResponse.setError(EError::FSFileCreateFailed, "File add to database failed");
              }
            }
            else
            {
              oFile.close();
              CcFile::remove(sFullPath);
              m_oResponse.init(ESyncCommandType::Crc);
              m_oResponse.setError(EError::FileTransferFailed, "Crc comparision failed");
            }
          }
          else
          {
            m_oResponse.setError(EError::FSFileCreateFailed, "Unable to create temporary file for upload.");
          }
        }
        else
        {
          m_oResponse.setError(EError::FSFileAlreadyExisting, "File already part of database");
          oFileInfo = m_pDirectory->getFileInfoByFilename(oFileInfo.getDirId(), oFileInfo.getName());
          // send back the file already stored in database
          if(oFileInfo.getId() > 0)
            m_oResponse.addFileInfo(oFileInfo);
        }
      }
      else
      {
        m_oResponse.setError(EError::FSDirNotFound, "Parent Directory for file not existing");
      }
    }
    else
    {
      if (!m_oRequest.data().contains(CcSyncGlobals::FileInfo::DirId))
        m_oResponse.setError(EError::CommandRequiredParameter, "Required parameter not found: " + CcSyncGlobals::FileInfo::DirId);
      if (!m_oRequest.data().contains(CcSyncGlobals::FileInfo::Name))
        m_oResponse.setError(EError::CommandRequiredParameter, "Required parameter not found: " + CcSyncGlobals::FileInfo::Name);
      if (!m_oRequest.data().contains(CcSyncGlobals::FileInfo::Modified))
        m_oResponse.setError(EError::CommandRequiredParameter, "Required parameter not found: " + CcSyncGlobals::FileInfo::Modified);
    }
  }
  sendResponse();
}

void CcSyncServerWorker::doDirectoryRemoveFile()
{
  if (loadConfigsBySessionRequest() &&
      loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::FileInfo::DirId)           &&
        m_oRequest.data().contains(CcSyncGlobals::FileInfo::Name))
    {
      CcSyncFileInfo oFileInfo = m_oRequest.getFileInfo();
      if (m_pDirectory->directoryExists(oFileInfo.getDirId()) &&
          m_pDirectory->fileInDirExists(oFileInfo.getDirId(), oFileInfo) == true)
      {
        m_pDirectory->fileListRemoveWithHistory(oFileInfo);
      }
      else
      {
        m_oResponse.setError(EError::FSFileNotFound, "File Not Found");
      }
    }
    else
    {
      m_oResponse.setError(EError::CommandRequiredParameter, "Required parameters not found.");
    }
  }
  sendResponse();
}

void CcSyncServerWorker::doDirectoryDownloadFile()
{
  if (loadConfigsBySessionRequest() &&
      loadDirectory())
  {
    if (m_oRequest.data().contains(CcSyncGlobals::FileInfo::Id))
    {
      uint64 uiFileId = m_oRequest.data()[CcSyncGlobals::FileInfo::Id].getValue().getUint64();
      CcSyncFileInfo oFileInfo = m_pDirectory->getFileInfoById(uiFileId);
      m_oResponse.addFileInfo(oFileInfo);
      sendResponse();
      CcString sFullFilePath = m_pDirectory->getFullFilePathById(uiFileId);
      if (sendFile(sFullFilePath))
      {
        m_oResponse.init(ESyncCommandType::Crc);
      }
      else
      {
        CCDEBUG("DirectoryDownloadFile send File failed:");
        CCDEBUG("    " + sFullFilePath);
        m_oResponse.setError(EError::FSFileCrcFailed, "FileTransfer failed");
      }
    }
    else
    {
      m_oResponse.setError(EError::CommandRequiredParameter, "Required parameters not found.");
    }
  }
  sendResponse();
}
