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
* @brief     Implemtation of class CcSyncClientConfigClientCommand
*/
#include "CcSyncGlobals.h"
#include "CcSyncClientConfig.h"
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlNodeList.h"
#include "CcStringUtil.h"
#include "Communication/DCommonPorts.h"
#include "CcDirectory.h"

CcSyncClientConfig::CcSyncClientConfig()
{
}

bool CcSyncClientConfig::readConfig(const CcString& sConfigFile)
{
  bool bRet = false;
  m_oXmlFile.setFile(sConfigFile);
  m_oXmlFile.readData();
  CcXmlNode& oDocRootNode = m_oXmlFile.rootNode();
  if (oDocRootNode.isNotNull())
  {
    CcXmlNode& oRootNode = oDocRootNode.getNode(CcSyncGlobals::Client::ConfigTags::Root);
    if (oRootNode.isNotNull())
    {
      CcXmlNodeList oAccountNodes = oRootNode.getNodes(CcSyncGlobals::Client::ConfigTags::Account);
      for (CcXmlNode& pAccountNode : oAccountNodes)
      {
        CcSyncAccountConfig oAccount(pAccountNode);
        if (oAccount.isValid())
        {
          bRet = true;
          m_oAccounts.append(oAccount);
        }
      }
    }
  }
  return bRet;
}

bool CcSyncClientConfig::readConfigFromServer(const CcString& sConfigFile, const CcString &sBasePath)
{
  bool bRet = false;
  if (readConfig(sConfigFile))
  {
    for (CcSyncAccountConfig& rAccountConfig : m_oAccounts)
    {
      rAccountConfig.overrideLocations(sBasePath);
    }
    bRet = true;
  }
  return bRet;
}

CcSyncAccountConfig* CcSyncClientConfig::getAccountConfig(const CcString& sAccountname)
{
  for (CcSyncAccountConfig& oAccount : m_oAccounts)
  {
    if (oAccount.getName().compareInsensitve(sAccountname))
    {
      return &oAccount;
    }
  }
  return nullptr;
}

CcSyncAccountConfig* CcSyncClientConfig::getFirstAccountConfig()
{
  if (m_oAccounts.size() > 0)
  {
    return &m_oAccounts[0];
  }
  return nullptr;
}
