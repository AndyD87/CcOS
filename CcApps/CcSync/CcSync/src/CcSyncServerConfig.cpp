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
* @brief     Implemtation of class CcSyncServerConfigClientCommand
*/
#include "CcSyncGlobals.h"
#include "CcSyncServerConfig.h"
#include "Communication/DCommonPorts.h"
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlNodeList.h"
#include "CcXml/CcXmlUtil.h"
#include "CcStringUtil.h"
#include "CcKernel.h"

CcSyncServerConfig::CcSyncServerConfig() :
  m_uiPort(PORT_CcSync)
{
}

bool CcSyncServerConfig::readConfig(const CcString& sConfigFile)
{
  m_bValid = true;
  // preinit members
  m_sSslKeyFile = CcKernel::getConfigDir();
  m_sSslKeyFile.appendPath(CcSyncGlobals::ConfigDirName);
  m_sSslKeyFile.appendPath(CcSyncGlobals::DefaultKeyFile);
  m_sSslCertFile = CcKernel::getConfigDir();
  m_sSslCertFile.appendPath(CcSyncGlobals::ConfigDirName);
  m_sSslCertFile.appendPath(CcSyncGlobals::DefaultCertFile);

  m_oXmlFile.setFile(sConfigFile);
  m_oXmlFile.readData();
  CcXmlNode& oDocRootNode = m_oXmlFile.rootNode();
  if (oDocRootNode.isNotNull())
  {
    CcXmlNode& oRootNode = oDocRootNode.getNode(CcSyncGlobals::Server::ConfigTags::Root);
    if (oRootNode.isNotNull()             &&
        findServerConfig(oRootNode)       &&
        findLocationsConfig(oRootNode)    &&
        findAccountConfigs(oRootNode))
    {
      CcXmlNode& pPassword = oRootNode.getNode(CcSyncGlobals::Client::ConfigTags::UserPassword);
      if ( pPassword.isNotNull())
      {
        m_oRootPassword = pPassword.getValue();
        CcXmlNode& pType = pPassword.getNode("Type");
        if (pType.isNotNull())
        {
          m_oRootPassword.setType(pType.getValue());
        }
        else
        {
          CCINFO("Password Type not defined, use default.");
        }
      }
      m_bValid = true;
    }
    else
    {
      CCDEBUG("Error in configuration");
    }
  }
  else
  {
    CCDEBUG("No valid config file found");
  }
  return m_bValid;
}

CcSyncServerAccount* CcSyncServerConfig::findAccount(const CcString& sAccountName) const
{
  for (CcSyncServerAccount& rAccount : m_oAccountList)
  {
    if (rAccount.getName() == sAccountName)
      return &rAccount;
  }
  return nullptr;
}

CcSyncServerConfig& CcSyncServerConfig::operator=(const CcSyncServerConfig& oToCopy)
{
  m_bValid = oToCopy.m_bValid;
  m_bSsl = oToCopy.m_bSsl;
  m_bSslRequired = oToCopy.m_bSslRequired;
  m_oXmlFile = oToCopy.m_oXmlFile;
  return *this;
}

CcSyncServerConfig& CcSyncServerConfig::operator=(CcSyncServerConfig&& oToMove)
{
  if (this != &oToMove)
  {
    m_bValid = oToMove.m_bValid;
    m_bSsl = oToMove.m_bSsl;
    m_bSslRequired = oToMove.m_bSslRequired;
    m_oXmlFile = std::move(oToMove.m_oXmlFile);
  }
  return *this;
}

bool CcSyncServerConfig::findServerConfig(CcXmlNode& pNode)
{
  bool bRet = false;
  CcXmlNode& pTempNode = pNode.getNode(CcSyncGlobals::Server::ConfigTags::Port);
  if (pTempNode.isNotNull())
  {
    m_uiPort = pTempNode.getValue().toUint16(&bRet);
    pTempNode = pNode.getNode(CcSyncGlobals::Server::ConfigTags::Ssl);
    if (pTempNode.isNotNull())
    {
      m_bSsl = CcStringUtil::getBoolFromStirng(pTempNode.getValue());
    }
    pTempNode = pNode.getNode(CcSyncGlobals::Server::ConfigTags::SslRequired);
    if (pTempNode.isNotNull())
    {
      m_bSslRequired = CcStringUtil::getBoolFromStirng(pTempNode.getValue());
    }
    pTempNode = pNode.getNode(CcSyncGlobals::Server::ConfigTags::SslCert);
    if (pTempNode.isNotNull())
    {
      m_sSslCertFile = pTempNode.getValue();
    }
    pTempNode = pNode.getNode(CcSyncGlobals::Server::ConfigTags::SslKey);
    if (pTempNode.isNotNull())
    {
      m_sSslKeyFile = pTempNode.getValue();
    }
  }
  else
  {
    CCERROR("Server configuration not found in CcSyncClient configuration file");
  }
  return bRet;
}

bool CcSyncServerConfig::findAccountConfigs(CcXmlNode& pNode)
{
  bool bRet = false;
  CcXmlNodeList oAccountNodes = pNode.getNodes(CcSyncGlobals::Server::ConfigTags::Account);
  for (CcXmlNode& pAccountNode : oAccountNodes)
  {
    bRet = true;
    CcSyncServerAccount oAccountConfig(pAccountNode);
    if (oAccountConfig.isValid())
    {
      CcString sUserPath = m_oLocationList[0].getPath();
      sUserPath.appendPath(oAccountConfig.getName());
      oAccountConfig.setBasePath(sUserPath);
      m_oAccountList.append(oAccountConfig);
    }
  }
  return bRet;
}

bool CcSyncServerConfig::findLocationsConfig(CcXmlNode& pNode)
{
  bool bRet = true;
  CcXmlNode& pLocationsNode = pNode.getNode(CcSyncGlobals::Server::ConfigTags::Locations);
  if (pLocationsNode.isNotNull())
  {
    CcXmlNodeList pLocationNodes = pLocationsNode.getNodes(CcSyncGlobals::Server::ConfigTags::Location);
    for (CcXmlNode& pLocationNode : pLocationNodes)
    {
      CcSyncLocationConfig oConfigLocation;
      bRet &= oConfigLocation.readConfig(pLocationNode);
      m_oLocationList.append(oConfigLocation);
    }
  }
  else
  {
    CCWARNING("Locations not found in CcSyncClient configuration file");
  }
  return bRet;
}

