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
 * @brief     Implemtation of class CcSyncAccountConfigClientCommand
 */
#include "CcSyncGlobals.h"
#include "CcSyncAccountConfig.h"
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlNodeList.h"
#include "CcXml/CcXmlFile.h"
#include "CcXml/CcXmlUtil.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"
#include "CcStringUtil.h"
#include "Communication/DCommonPorts.h"
#include "CcDirectory.h"

CcSyncAccountConfig::CcSyncAccountConfig()
{
  m_bValid = false;
}

CcSyncAccountConfig::CcSyncAccountConfig(const CcXmlNode& pAccountNode)
{
  parseXml(pAccountNode);
}

CcSyncAccountConfig::CcSyncAccountConfig(const CcJsonObject& pAccountNode)
{
  parseJson(pAccountNode);
}

CcSyncAccountConfig& CcSyncAccountConfig::operator=(const CcSyncAccountConfig& oToCopy)
{
  m_bValid  = oToCopy.m_bValid;
  m_sName   = oToCopy.m_sName;
  m_oPassword = oToCopy.m_oPassword;
  m_sDatabaseFile = oToCopy.m_sDatabaseFile;
  m_oServer = oToCopy.m_oServer;
  m_oDirectoryList = oToCopy.m_oDirectoryList;
  m_oCommandList = oToCopy.m_oCommandList;
  return *this;
}

CcSyncAccountConfig& CcSyncAccountConfig::operator=(CcSyncAccountConfig&& oToMove)
{
  if (this != &oToMove)
  {
    m_bValid = oToMove.m_bValid;
    m_sName = std::move(oToMove.m_sName);
    m_oPassword = std::move(oToMove.m_oPassword);
    m_sDatabaseFile = std::move(oToMove.m_sDatabaseFile);
    m_oServer = std::move(oToMove.m_oServer);
    m_oDirectoryList = std::move(oToMove.m_oDirectoryList);
    m_oCommandList = std::move(oToMove.m_oCommandList);
  }
  return *this;
}

CcXmlNode CcSyncAccountConfig::getXmlNode() const
{
  CcXmlNode oAccountNode(EXmlNodeType::Node);
  oAccountNode.setName(CcSyncGlobals::Client::ConfigTags::Account);
  {
    CcXmlNode oAccountName(EXmlNodeType::Node);
    oAccountName.setName(CcSyncGlobals::Client::ConfigTags::UserName);
    oAccountName.setValue(m_sName);
    oAccountNode.append(std::move(oAccountName));

    CcXmlNode oAccountPassword(EXmlNodeType::Node);
    oAccountName.setName(CcSyncGlobals::Client::ConfigTags::UserPassword);
    oAccountName.setValue(""); // Hide Password
    oAccountNode.append(std::move(oAccountPassword));

    CcXmlNode oAccountServer(EXmlNodeType::Node);
    oAccountServer.setName(CcSyncGlobals::Client::ConfigTags::Server);
    {
      CcXmlNode oAccountServerHost(EXmlNodeType::Node);
      oAccountServerHost.setName(CcSyncGlobals::Client::ConfigTags::ServerHost);
      oAccountServerHost.setValue(m_oServer.getHostname());
      oAccountServer.append(std::move(oAccountServerHost));

      CcXmlNode oAccountServerPort(EXmlNodeType::Node);
      oAccountServerPort.setName(CcSyncGlobals::Client::ConfigTags::ServerPort);
      oAccountServerPort.setValue(m_oServer.getPortString());
      oAccountServer.append(std::move(oAccountServerHost));
    }
    oAccountNode.append(std::move(oAccountServer));

    CcXmlNode oAccountDatabaseFile(EXmlNodeType::Node);
    oAccountDatabaseFile.setName(CcSyncGlobals::Client::ConfigTags::Database);
    oAccountDatabaseFile.setValue(m_sDatabaseFile); // Hide Password
    oAccountNode.append(std::move(oAccountDatabaseFile));

    for (CcSyncDirectoryConfig& oDirConfig : m_oDirectoryList)
    {
      oAccountNode.append(std::move(oDirConfig.getXmlNode()));
    }
  }
  return oAccountNode;
}

CcJsonObject CcSyncAccountConfig::getJsonNode() const
{
  CcJsonObject oAccountNode;
  oAccountNode.add(CcJsonData(CcSyncGlobals::Commands::AccountGetData::Name, getName()));
  oAccountNode.add(CcJsonData(CcSyncGlobals::Commands::AccountGetData::Password, ""));
  oAccountNode.add(CcJsonData(CcSyncGlobals::Commands::AccountGetData::Database, getDatabaseFilePath()));

  CcJsonObject oServer;
  oServer.add(CcJsonData(CcSyncGlobals::Commands::AccountGetData::ServerHost, getServer().getHostname()));
  oServer.add(CcJsonData(CcSyncGlobals::Commands::AccountGetData::ServerPort, getServer().getPort()));
  oAccountNode.add(CcJsonData(oServer, CcSyncGlobals::Commands::AccountGetData::Server));

  CcJsonArray oDirArray;
  for (CcSyncDirectoryConfig& oDirConfig : getDirectoryList())
  {
    CcJsonObject oDirInfo;
    oDirInfo.add(CcJsonData(CcSyncGlobals::Commands::AccountGetData::DirName, oDirConfig.getName()));
    oDirInfo.add(CcJsonData(CcSyncGlobals::Commands::AccountGetData::DirName, oDirConfig.getLocation()));
    oDirArray.add(CcJsonData(oDirInfo, ""));
  }
  oAccountNode.add(CcJsonData(oDirArray, CcSyncGlobals::Commands::AccountGetData::Directories));
  return oAccountNode;
}

bool CcSyncAccountConfig::parseXml(const CcXmlNode& pAccountNode)
{
  m_oServer.setPort(PORT_CcSync);
  m_bValid = false;
  if (!pAccountNode.isNull() &&
        xmlFindServerConfig(pAccountNode) &&
        xmlFindUserConfig(pAccountNode) &&
        xmlFindCommandsConfig(pAccountNode) &&
        xmlFindDirectoriesConfig(pAccountNode) &&
        xmlFindDatabaseConfig(pAccountNode))
  {
    m_bValid = true;
  }
  return m_bValid;
}

bool CcSyncAccountConfig::parseJson(const CcJsonObject& pAccountNode)
{
  m_oServer.setPort(PORT_CcSync);
  m_bValid = false;
  if (  jsonFindServerConfig(pAccountNode) &&
        jsonFindUserConfig(pAccountNode) &&
        jsonFindCommandsConfig(pAccountNode) &&
        jsonFindDirectoriesConfig(pAccountNode) &&
        jsonFindDatabaseConfig(pAccountNode))
  {
    m_bValid = true;
  }
  return m_bValid;
}

bool CcSyncAccountConfig::checkLogin(const CcString sUsername, const CcPassword oPassword)
{
  bool bRet = false;
  if (m_sName == sUsername &&
      m_oPassword == oPassword)
  {
    bRet = true;
  }
  return bRet;
}

void CcSyncAccountConfig::overrideLocations(const CcString& sBasePath)
{
  for (CcSyncDirectoryConfig& oDirectoryConfig : m_oDirectoryList)
  {
    CcString sPath(sBasePath);
    sPath.appendPath(oDirectoryConfig.getName());
    oDirectoryConfig.overrideLocation(sPath);
  }
}

bool CcSyncAccountConfig::xmlFindServerConfig(const CcXmlNode& pNode)
{
  bool bRet = false;
  CcXmlNode& pServerNode = pNode.getNode(CcSyncGlobals::Client::ConfigTags::Server);
  if (!pServerNode.isNull())
  {
    CcXmlNode& pHostNode = pServerNode[CcSyncGlobals::Client::ConfigTags::ServerHost];
    CcXmlNode& pPortNode = pServerNode[CcSyncGlobals::Client::ConfigTags::ServerPort];
    if (!pHostNode.isNull() &&
      !pPortNode.isNull())
    {
      bRet = true;
      m_oServer.setHostname(pHostNode.getValue());
      m_oServer.setPort(pPortNode.getValue());
    }
    else
    {
      CCERROR("Server configuration failure in CcSyncClient configuration file");
    }
  }
  else
  {
    CCERROR("Server configuration not found in CcSyncClient configuration file");
  }
  return bRet;
}

bool CcSyncAccountConfig::xmlFindUserConfig(const CcXmlNode& pNode)
{
  bool bRet = false;
  CcXmlNode& pNameNode = pNode.getNode(CcSyncGlobals::Client::ConfigTags::UserName);
  CcXmlNode& pPassword = pNode.getNode(CcSyncGlobals::Client::ConfigTags::UserPassword);
  if (pNameNode.isNotNull() &&
    pPassword.isNotNull())
  {
    bRet = true;
    m_sName = pNameNode.getValue();
    m_oPassword = pPassword.getValue();
    CcXmlNode& pType = pPassword.getNode("Type");
    if (pType.isNotNull())
    {
      m_oPassword.setType(pType.getValue());
    }
    else
    {
      CCINFO("Password Type not defined, use default.");
    }
  }
  // @todo: load additional users if available

  return bRet;
}

bool CcSyncAccountConfig::xmlFindDirectoriesConfig(const CcXmlNode& pNode)
{
  bool bRet = false;
  CcXmlNodeList oDirectoryNodes = pNode.getNodes(CcSyncGlobals::Client::ConfigTags::Directory);
  for (CcXmlNode& pDirectoryNode : oDirectoryNodes)
  {
    bRet = true;
    CcSyncDirectoryConfig oDirectory(pDirectoryNode);
    m_oDirectoryList.append(std::move(oDirectory));
  }
  return bRet;
}

bool CcSyncAccountConfig::xmlFindCommandsConfig(const CcXmlNode& pNode)
{
  bool bRet = true;
  CcXmlNodeList oCommandNodes = pNode.getNodes(CcSyncGlobals::Client::ConfigTags::Command);
  for (CcXmlNode& pCommandNode : oCommandNodes)
  {
    CcSyncExecutionConfig oCommand(pCommandNode);
    m_oCommandList.append(oCommand);
  }
  return bRet;
}

bool CcSyncAccountConfig::xmlFindDatabaseConfig(const CcXmlNode& pNode)
{
  bool bRet = true;
  CcXmlNode& pCommandsNode = pNode.getNode(CcSyncGlobals::Client::ConfigTags::Database);
  if (pCommandsNode.isNotNull() && pCommandsNode.getValue() != "")
  {
    m_sDatabaseFile = pCommandsNode.getValue();
  }
  else
  {
    m_sDatabaseFile = "Client.";
    m_sDatabaseFile << getName() << CcSyncGlobals::SqliteExtension;
  }
  return bRet;
}

bool CcSyncAccountConfig::jsonFindServerConfig(const CcJsonObject& pNode)
{
  bool bRet = false;
  const CcJsonData& pServerNode = pNode[CcSyncGlobals::Client::ConfigTags::Server];
  if (pServerNode.isObject())
  {
    const CcJsonData& pHostNode = pServerNode[CcSyncGlobals::Client::ConfigTags::ServerHost];
    const CcJsonData& pPortNode = pServerNode[CcSyncGlobals::Client::ConfigTags::ServerPort];
    if (pHostNode.isValue() &&
        pPortNode.isValue())
    {
      bRet = true;
      m_oServer.setHostname(pHostNode.getValue().getString());
      m_oServer.setPort(pPortNode.getValue().getUint16());
    }
    else
    {
      CCERROR("Server configuration failure in CcSyncClient configuration file");
    }
  }
  else
  {
    CCERROR("Server configuration not found in CcSyncClient configuration file");
  }
  return bRet;
}

bool CcSyncAccountConfig::jsonFindUserConfig(const CcJsonObject& pNode)
{
  bool bRet = true;
  const CcJsonData& pNameNode = pNode[CcSyncGlobals::Client::ConfigTags::UserName];
  const CcJsonData& pPassword = pNode[CcSyncGlobals::Client::ConfigTags::UserPassword];
  if (pNameNode.isValue() &&
      pPassword.isValue())
  {
    bRet = true;
    m_sName = pNameNode.getValue().getString();
    m_oPassword = pPassword.getValue().getString();
    const CcJsonData& pType = pPassword["Type"];
    if (pType.isNotNull())
    {
      m_oPassword.setType(pType.getValue().getString());
    }
    else
    {
      CCDEBUG("Password Type not defined, use default.");
    }
  }
  // @todo: load additional users if available

  //// @todo: load additional users if available
  return bRet;
}

bool CcSyncAccountConfig::jsonFindDirectoriesConfig(const CcJsonObject& pNode)
{
  bool bRet = false;
  const CcJsonData& oDirectoryNodes = pNode[CcSyncGlobals::Client::ConfigTags::Directory];
  if (oDirectoryNodes.isObject())
  {
    for (const CcJsonData& pDirectoryNode : oDirectoryNodes.getJsonObject())
    {
      bRet = true;
      CcSyncDirectoryConfig oDirectory(pDirectoryNode.getJsonObject());
      m_oDirectoryList.append(std::move(oDirectory));
    }
  }
  return bRet;
}

bool CcSyncAccountConfig::jsonFindCommandsConfig(const CcJsonObject& pNode)
{
  bool bRet = true;
  CCUNUSED(pNode);
  //// @todo: move to each directory
  return bRet;
}

bool CcSyncAccountConfig::jsonFindDatabaseConfig(const CcJsonObject& pNode)
{
  bool bRet = true;
  const CcJsonData& oDatabaseNodes = pNode[CcSyncGlobals::Client::ConfigTags::Database];
  if (oDatabaseNodes.isValue() && oDatabaseNodes.getValue().getString() != "")
  {
    m_sDatabaseFile = oDatabaseNodes.getValue().getString();
  }
  return bRet;
}