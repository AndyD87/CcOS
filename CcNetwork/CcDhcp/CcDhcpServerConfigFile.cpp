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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcDhcpServerConfigFile
 */
#include "CcDhcpServerConfigFile.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcDhcpGlobals.h"
#include "CcXml/CcXmlFile.h"

/**
 * @brief Private storage class for CcDhcpServerConfigFile
 */
class CcDhcpServerConfigFile::CPrivate
{
public:
  CcXmlFile oConfigFile;
};

CcDhcpServerConfigFile::CcDhcpServerConfigFile()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcDhcpServerConfigFile::~CcDhcpServerConfigFile()
{
  CCDELETE(m_pPrivate);
}

bool CcDhcpServerConfigFile::loadConfigFile(const CcString& sPath)
{
  bool bRet = false;
  m_pPrivate->oConfigFile.setFile(sPath);
  bRet = m_pPrivate->oConfigFile.readData();
  if (bRet)
  {
    bRet = m_pPrivate->oConfigFile.rootNode().isNotNull();
    if (bRet)
    {
      bRet = m_pPrivate->oConfigFile.rootNode().getName() == CcDhcpGlobals::Server::Config::RootNode;
      if (bRet)
      {
        bRet = parseConfigData();
      }
    }
  }
  return bRet;
}

bool CcDhcpServerConfigFile::parseConfigData()
{
  bool bRet = true;
  CcIp oIp;
  CcXmlNode& rIpBeginNode = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::IpBegin];
  if (rIpBeginNode.isNotNull() &&
      oIp.setIp(rIpBeginNode.innerText()))
  {
    m_oConfig.setIpBegin(oIp);
  }
  CcXmlNode& rIpEndNode = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::IpEnd];
  if (rIpEndNode.isNotNull() &&
    oIp.setIp(rIpEndNode.innerText()))
  {
    m_oConfig.setIpEnd(oIp);
  }
  CcXmlNode& rSubnet = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Subnet];
  if (rSubnet.isNotNull() &&
    oIp.setIp(rSubnet.innerText()))
  {
    m_oConfig.setSubnet(oIp);
  }
  CcXmlNode& rGateway = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Gateway];
  if (rGateway.isNotNull() &&
    oIp.setIp(rGateway.innerText()))
  {
    m_oConfig.setGateway(oIp);
  }
  CcXmlNode& rDns1 = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Dns];
  if (rDns1.isNotNull() &&
    oIp.setIp(rDns1.innerText()))
  {
    m_oConfig.setDns1(oIp);
  }
  CcXmlNode& rBootfile = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Bootfile];
  if (rBootfile.isNotNull())
  {
    m_oConfig.setBootfile(rBootfile.innerText());
  }
  CcXmlNode& rBootfileMbr = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::BootfileMbr];
  if (rBootfileMbr.isNotNull())
  {
    m_oConfig.setBootfileMbr(rBootfileMbr.innerText());
  }
  CcXmlNode& rBootfileEfi = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::BootfileMbr];
  if (rBootfileMbr.isNotNull())
  {
    m_oConfig.setBootfileEfi(rBootfileEfi.innerText());
  }
  return bRet;
}
