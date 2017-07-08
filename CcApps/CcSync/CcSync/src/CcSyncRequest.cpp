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
 * @brief     Implemtation of class CcSyncRequest
 */
#include "CcSyncRequest.h"
#include "CcUser.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonData.h"
#include "CcSyncGlobals.h"
#include "CcByteArray.h"
#include "CcSyncFileInfo.h"
#include "Hash/CcCrc32.h"

CcSyncRequest::CcSyncRequest( void )
{
}

CcSyncRequest::CcSyncRequest(ESyncCommandType eCommandType)
{
  init(eCommandType);
}

CcSyncRequest::CcSyncRequest( const CcSyncRequest& oToCopy )
{
  operator=(oToCopy);
}

CcSyncRequest::CcSyncRequest( CcSyncRequest&& oToMove )
{
  operator=(std::move(oToMove));
}

CcSyncRequest::~CcSyncRequest( void )
{
}

CcSyncRequest& CcSyncRequest::operator=(const CcSyncRequest& oToCopy)
{
  CCUNUSED(oToCopy);
  return *this;
}

CcSyncRequest& CcSyncRequest::operator=(CcSyncRequest&& oToMove)
{
  if (this != &oToMove)
  {
  }
  return *this;
}

bool CcSyncRequest::operator==(const CcSyncRequest& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcSyncRequest::operator!=(const CcSyncRequest& oToCompare) const
{
  return !operator==(oToCompare);
}

bool CcSyncRequest::parseData(const CcString& oData)
{
  bool bRet = false;
  m_oData.clear();
  m_bHasAdditionalData = false;
  m_eType = ESyncCommandType::Unkown;
  CcJsonDocument oJsonDoc;
  if (oJsonDoc.parseDocument(oData))
  {
    m_oData = oJsonDoc.getJsonData().getJsonObject();
    if (getTypeFromData())
    {
      bRet = true;
    }
  }
  return bRet;
}

CcByteArray CcSyncRequest::getBinary()
{
  CcJsonDocument oJsonDoc(m_oData);
  CcByteArray oRet = oJsonDoc.getJsonDocument();
  return oRet;
}

CcCrc32 CcSyncRequest::getCrc()
{
  CcCrc32 oCrc;
  if (m_oData.contains(CcSyncGlobals::Commands::Crc::Crc))
    oCrc = m_oData[CcSyncGlobals::Commands::Crc::Crc].getValue().getUint32();
  return  oCrc;
}

bool CcSyncRequest::hasFileInfo()
{
  return false;
}

void CcSyncRequest::addFileInfo(const CcSyncFileInfo& oFileInfo)
{
  m_oData.append(std::move(oFileInfo.getJsonObject()));

}

CcSyncFileInfo CcSyncRequest::getFileInfo()
{
  CcSyncFileInfo oFileInfo;
  oFileInfo.fromJsonObject(m_oData);
  return oFileInfo;
}


void CcSyncRequest::init(ESyncCommandType eCommandType)
{
  m_oData.clear();
  m_bHasAdditionalData = false;
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::Command, (uint16) eCommandType));
  m_eType = eCommandType;
}

void CcSyncRequest::setCrc(const CcCrc32 & oCrc)
{
  init(ESyncCommandType::Crc);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::Crc::Crc, oCrc.getValue()));
}



void CcSyncRequest::setAccountLogin(const CcString& sAccount, const CcString& sUsername, const CcString& sPassword)
{
  init(ESyncCommandType::AccountLogin);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::AccountLogin::Account, sAccount));
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::AccountLogin::Username, sUsername));
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::AccountLogin::Password, sPassword));
}

void CcSyncRequest::setSession(const CcString& sSession)
{
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::Session, sSession));
}

void CcSyncRequest::setDirectoryCreateDirectory(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo)
{
  init(ESyncCommandType::DirectoryCreateDirectory);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryCreateDirectory::DirectoryName, sDirectoryName));
  addFileInfo(oFileInfo);
}

void CcSyncRequest::setDirectoryRemoveDirectory(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo)
{
  init(ESyncCommandType::DirectoryRemoveDirectory);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryCreateDirectory::DirectoryName, sDirectoryName));
  addFileInfo(oFileInfo);
}

void CcSyncRequest::setDirectoryUploadFile(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo)
{
  init(ESyncCommandType::DirectoryUploadFile);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryUploadFile::DirectoryName, sDirectoryName));
  addFileInfo(oFileInfo);
}

void CcSyncRequest::setDirectoryRemoveFile(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo)
{
  init(ESyncCommandType::DirectoryRemoveFile);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryUploadFile::DirectoryName, sDirectoryName));
  addFileInfo(oFileInfo);
}

void CcSyncRequest::setDirectoryDownloadFile(const CcString& sDirectoryName, uint64 uiFileId)
{
  init(ESyncCommandType::DirectoryDownloadFile);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryDownloadFile::DirectoryName, sDirectoryName));
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryDownloadFile::Id, uiFileId));
}

void CcSyncRequest::setDirectoryGetDirectoryInfo(const CcString& sDirectoryName, uint64 uiDirId)
{
  init(ESyncCommandType::DirectoryGetDirectoryInfo);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryGetDirectoryInfo::DirectoryName, sDirectoryName));
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryGetDirectoryInfo::Id, uiDirId));
}

void CcSyncRequest::setDirectoryGetFileInfo(const CcString& sDirectoryName, uint64 uiFileId)
{
  init(ESyncCommandType::DirectoryGetFileInfo);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryGetDirectoryInfo::DirectoryName, sDirectoryName));
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryGetDirectoryInfo::Id, uiFileId));
}

void CcSyncRequest::setDirectoryGetFileList(const CcString& sDirectoryName, uint64 uiDirId)
{
  init(ESyncCommandType::DirectoryGetFileList);
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryGetFileList::DirectoryName, sDirectoryName));
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::DirectoryGetFileList::Id, uiDirId));
}

bool CcSyncRequest::getTypeFromData()
{
  CcJsonData& oValue = m_oData[CcSyncGlobals::Commands::Command];
  if (oValue.isNotNull())
  {
    m_eType = (ESyncCommandType) oValue.value().getUint16();
    return true;
  }
  return false;
}
