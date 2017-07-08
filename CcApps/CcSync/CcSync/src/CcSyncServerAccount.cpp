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
* @brief     Implemtation of class CcSyncServerAccountClientCommand
*/
#include "CcSyncGlobals.h"
#include "CcSyncServerAccount.h"
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlUtil.h"
#include "CcStringUtil.h"
#include "CcSyncClientConfig.h"
#include "CcSyncDbClient.h"
#include "CcSqlite.h"
#include "CcFile.h"

CcSyncServerAccount::CcSyncServerAccount(CcXmlNode& pAccountNode)
{
  CcXmlNode& pTempNode = pAccountNode.getNode(CcSyncGlobals::Server::ConfigTags::AccountName);
  if (pTempNode.isNotNull())
  {
    m_sName = pTempNode.getValue();
    pTempNode = pAccountNode.getNode(CcSyncGlobals::Server::ConfigTags::AccountPassword);
    if (pTempNode.isNotNull())
    {
      m_bIsValid = true;
      CcXmlNode& pPwdType = pTempNode.getNode(CcSyncGlobals::Server::ConfigTags::AccountPasswordType);
      if (pPwdType.isNotNull())
      {
        m_oPassword.setPassword(pTempNode.getValue(), CcPassword::getTypeByString(pPwdType.getValue()));
      }
      else
      {
        m_oPassword.setPassword(pTempNode.getValue());
      }
    }
  }
  CcXmlNode& pAdmin = pAccountNode.getNode(CcSyncGlobals::Client::ConfigTags::Admin);
  if (pAdmin.isNotNull())
  {
    m_bIsAdmin = CcXmlUtil::getBoolFromNodeValue(pAdmin, false);
  }
}

CcSyncServerAccount::CcSyncServerAccount(const CcSyncServerAccount& oToCopy)
{
  operator=(oToCopy);
}

CcSyncServerAccount::CcSyncServerAccount(CcSyncServerAccount&& oToMove)
{
  operator=(std::move(oToMove));
}

CcSyncServerAccount::~CcSyncServerAccount(void)
{
  if (m_pClientConfig != nullptr)
  {
    CCMONITORDELETE(m_pClientConfig); 
    delete m_pClientConfig;
  }
  if (m_pDatabase != nullptr)
  {
    CCMONITORDELETE(m_pDatabase); 
    delete m_pDatabase;
  }
}

CcSyncServerAccount& CcSyncServerAccount::operator=(const CcSyncServerAccount& oToCopy)
{
  m_sName = oToCopy.m_sName;
  m_oPassword = oToCopy.m_oPassword;
  m_pDatabase = oToCopy.m_pDatabase;
  m_sBasePath = oToCopy.m_sBasePath;
  m_pClientConfig = oToCopy.m_pClientConfig;
  return *this;
}

CcSyncServerAccount& CcSyncServerAccount::operator=(CcSyncServerAccount&& oToMove)
{
  if (this != &oToMove)
  {
    m_sName = std::move(oToMove.m_sName);
    m_oPassword = std::move(oToMove.m_oPassword);
    m_sBasePath = std::move(oToMove.m_sBasePath);

    CCMONITORDELETE(m_pClientConfig);
    delete m_pClientConfig;
    CCMONITORDELETE(m_pDatabase);
    delete m_pDatabase;
    m_pClientConfig = oToMove.m_pClientConfig;
    m_pDatabase = oToMove.m_pDatabase;
    oToMove.m_pDatabase = nullptr;
    oToMove.m_pClientConfig = nullptr;
  }
  return *this;
}

CcSyncClientConfig* CcSyncServerAccount::clientConfig()
{
  if (m_pClientConfig == nullptr)
  {
    CcSyncClientConfig* pClientConfig = new CcSyncClientConfig(); 
    CCMONITORNEW(pClientConfig);
    CcString sConfigFilePath(m_sBasePath);
    sConfigFilePath.appendPath(CcSyncGlobals::Client::ConfigFileName);;
    if (pClientConfig->readConfigFromServer(sConfigFilePath, m_sBasePath))
    {
      m_pClientConfig = pClientConfig;
    }
    else
    {
      CCMONITORDELETE(m_pClientConfig); delete m_pClientConfig;
    }
  }
  return m_pClientConfig;
}

CcSyncDbClient* CcSyncServerAccount::database()
{
  if (m_pDatabase == nullptr)
  {
    if (clientConfig() != nullptr)
    {
      m_pDatabase = new CcSyncDbClient();
      CCMONITORNEW(m_pDatabase);
      CcString sConfigFilePath(m_sBasePath);
      sConfigFilePath.appendPath(CcSyncGlobals::Client::DatabaseFileName);
      if (m_pDatabase->openDatabase(sConfigFilePath))
      {
        return m_pDatabase;
      }
      else
      {
        CCMONITORDELETE(m_pDatabase); 
        delete m_pDatabase;
      }
    }
  }
  return m_pDatabase;
}
