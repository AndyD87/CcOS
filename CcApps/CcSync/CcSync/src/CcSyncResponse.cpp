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
 * @brief     Implemtation of class CcSyncResponse
 */
#include "CcSyncResponse.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonData.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcSyncGlobals.h"
#include "CcByteArray.h"
#include "CcSyncAccountConfig.h"

CcSyncResponse::CcSyncResponse( void )
{
}

CcSyncResponse::CcSyncResponse( const CcSyncResponse& oToCopy )
{
  operator=(oToCopy);
}

CcSyncResponse::CcSyncResponse( CcSyncResponse&& oToMove )
{
  operator=(std::move(oToMove));
}

CcSyncResponse::~CcSyncResponse( void )
{
}

CcSyncResponse& CcSyncResponse::operator=(const CcSyncResponse& oToCopy)
{
  CCUNUSED(oToCopy);
  return *this;
}

CcSyncResponse& CcSyncResponse::operator=(CcSyncResponse&& oToMove)
{
  if(this != &oToMove)
  {
  }
  return *this;
}

bool CcSyncResponse::operator==(const CcSyncResponse& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcSyncResponse::operator!=(const CcSyncResponse& oToCompare) const
{
  return !operator==(oToCompare);
}

bool CcSyncResponse::parseData(const CcString& oData)
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

void CcSyncResponse::init(ESyncCommandType eCommandType)
{
  m_oData.clear();
  m_bHasAdditionalData = false;
  m_oData.add(CcJsonData("Command", (uint16) eCommandType));
  m_eType = eCommandType;
}

bool CcSyncResponse::hasError()
{
  CcJsonData& rErrorValue = m_oData[CcSyncGlobals::Commands::ErrorCode];
  if (rErrorValue.isValue())
    return true;
  return false;
}

CcError CcSyncResponse::getError()
{
  CcError oError = m_oData[CcSyncGlobals::Commands::ErrorCode].getValue().getUint32();
  return oError;
}

CcString CcSyncResponse::getErrorMsg()
{
  CcJsonData& oErrorMsg = m_oData[CcSyncGlobals::Commands::ErrorMsg];
  CcString sError;
  if (oErrorMsg.isValue())
    sError = oErrorMsg.getValue().getString();
  return sError;
}

CcByteArray CcSyncResponse::getBinary()
{
  CcJsonDocument oJsonDoc(m_oData);
  CcByteArray oRet = oJsonDoc.getJsonDocument();
  return oRet;
}

void CcSyncResponse::setLogin(const CcString& sUserToken)
{
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::AccountLogin::Session, sUserToken));
}

void CcSyncResponse::setResult(bool uiResult)
{
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::Result, uiResult));
}

void CcSyncResponse::setError(CcError uiErrorCode, const CcString& sErrorMsg)
{
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::ErrorCode, uiErrorCode.getErrorUnit()));
  m_oData.add(CcJsonData(CcSyncGlobals::Commands::ErrorMsg, sErrorMsg));
}

void CcSyncResponse::addAccountInfo(const CcSyncAccountConfig& oAccountConfig)
{
  m_oData.add(CcJsonData(oAccountConfig.getJsonNode(), CcSyncGlobals::Commands::AccountGetData::Account));
}

CcSyncAccountConfig CcSyncResponse::getAccountInfo()
{
  CcSyncAccountConfig oAccountConfig;
  CcJsonData& rAccountNode = m_oData[CcSyncGlobals::Commands::AccountGetData::Account];
  if (rAccountNode.isObject())
  {
    oAccountConfig.parseJson(rAccountNode.getJsonObject());
  }
  return oAccountConfig;
}

CcString CcSyncResponse::getSession()
{
  CcString sSession;
  if (m_oData.contains(CcSyncGlobals::Commands::AccountLogin::Session))
  {
    sSession = m_oData[CcSyncGlobals::Commands::AccountLogin::Session].getValue().getString();
  }
  return sSession;
}

void CcSyncResponse::addDirectoryDirectoryInfoList(const CcSyncFileInfoList& oDirectoryInfoList, const CcSyncFileInfoList& oFileInfoList)
{
  CcJsonData oDirectoriesNode(CcSyncGlobals::Commands::DirectoryGetFileList::DirsNode);
  oDirectoriesNode.setJsonArray();
  for (CcSyncFileInfo& oFileInfo : oDirectoryInfoList)
  {
    oDirectoriesNode.array().add(CcJsonData( oFileInfo.getJsonObject(), ""));
  }
  m_oData.append(std::move(oDirectoriesNode));

  CcJsonData oFilesNode(CcSyncGlobals::Commands::DirectoryGetFileList::FilesNode);
  oFilesNode.setJsonArray();
  for (CcSyncFileInfo& oFileInfo : oFileInfoList)
  {
    oFilesNode.array().add(CcJsonData(oFileInfo.getJsonObject(),""));
  }
  m_oData.append(std::move(oFilesNode));
}

bool CcSyncResponse::hasFileInfo()
{
  return false;
}

void CcSyncResponse::addFileInfo(const CcSyncFileInfo& oFileInfo)
{
  m_oData.append(std::move(oFileInfo.getJsonObject()));
}

CcSyncFileInfo CcSyncResponse::getFileInfo()
{
  CcSyncFileInfo oFileInfo;
  oFileInfo.fromJsonObject(m_oData);
  return oFileInfo;
}

bool CcSyncResponse::getDirectoryDirectoryInfoList(CcSyncFileInfoList& oDirectoryInfoList, CcSyncFileInfoList& oFileInfoList)
{
  bool bRet = false;
  if (m_oData.contains(CcSyncGlobals::Commands::DirectoryGetFileList::DirsNode))
  {
    CcJsonArray& oJsonArray = m_oData[CcSyncGlobals::Commands::DirectoryGetFileList::DirsNode].array();
    for (CcJsonData& oJsonData : oJsonArray)
    {
      CcJsonObject& oJsonFileArray = oJsonData.object();
      CcSyncFileInfo oFileInfo;
      oFileInfo.fromJsonObject(oJsonFileArray);
      oDirectoryInfoList.append(std::move(oFileInfo));
    }
  }
  if (m_oData.contains(CcSyncGlobals::Commands::DirectoryGetFileList::FilesNode))
  {
    CcJsonArray& oJsonArray = m_oData[CcSyncGlobals::Commands::DirectoryGetFileList::FilesNode].array();
    for (CcJsonData& oJsonData : oJsonArray)
    {
      CcJsonObject& oJsonFileArray = oJsonData.object();
      CcSyncFileInfo oFileInfo;
      oFileInfo.fromJsonObject(oJsonFileArray);
      oFileInfoList.append(std::move(oFileInfo));
    }
  }
  return bRet;
}

bool CcSyncResponse::getTypeFromData()
{
  CcJsonData& oValue = m_oData[CcSyncGlobals::Commands::Command];
  if (oValue.isValue())
  {
    m_eType = (ESyncCommandType) oValue.value().getUint16();
    return true;
  }
  return false;
}
