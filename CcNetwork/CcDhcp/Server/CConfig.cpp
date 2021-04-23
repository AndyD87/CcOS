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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CConfig
 */
#include "Server/CConfig.h"
#include "Network/ISocket.h"
#include "Network/CcCommonPorts.h"
#include "Network/CcCommonIps.h"
#include "CcDhcpGlobals.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcGlobalStrings.h"

namespace NDhcp
{

namespace NServer
{

CConfig::CConfig() :
  m_oBindAddress(ESocketType::UDP, CcCommonIps::AnyAddress, CcCommonPorts::DHCP_SRV),
  m_oIpBegin(CcDhcpGlobals::Server::DefaultIpBegin),
  m_oIpEnd(CcDhcpGlobals::Server::DefaultIpEnd),
  m_oSubnet(CcDhcpGlobals::Server::DefaultSubnet),
  m_oNextServer(CcDhcpGlobals::Server::DefaultServerIp),
  m_uiLeaseTime(CcDhcpGlobals::Server::DefaultLeaseTime),
  m_uiRenewTime(CcDhcpGlobals::Server::DefaultRenewTime),
  m_uiRebindTime(CcDhcpGlobals::Server::DefaultRebindTime)
{
  m_oConfigMap.appendItem(CcConfigBinary::EType::IpV4Begin, &m_oIpBegin);
}

CConfig::~CConfig()
{
}

bool CConfig::loadConfigFile(const CcString& CCUNUSED_PARAM(sPath))
{
  bool bRet = false;
  //m_pPrivate->oConfigFile.setFile(sPath);
  //bRet = m_pPrivate->oConfigFile.readData();
  //if (bRet)
  //{
  //  bRet = m_pPrivate->oConfigFile.rootNode().isNotNull();
  //  if (bRet)
  //  {
  //    bRet = m_pPrivate->oConfigFile.rootNode().getName() == CcDhcpGlobals::Server::Config::RootNode;
  //    if (bRet)
  //    {
  //      bRet = parseConfigData();
  //    }
  //  }
  //}
  return bRet;
}

bool CConfig::parseConfigData()
{
  bool bRet = true;
  CcIp oIp;
  //CcXmlNode& rIpBeginNode = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::IpBegin];
  //if (rIpBeginNode.isNotNull() &&
  //    oIp.setIp(rIpBeginNode.innerText()))
  //{
  //  m_oConfig.setIpBegin(oIp);
  //}
  //CcXmlNode& rIpEndNode = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::IpEnd];
  //if (rIpEndNode.isNotNull() &&
  //  oIp.setIp(rIpEndNode.innerText()))
  //{
  //  m_oConfig.setIpEnd(oIp);
  //}
  //CcXmlNode& rSubnet = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Subnet];
  //if (rSubnet.isNotNull() &&
  //  oIp.setIp(rSubnet.innerText()))
  //{
  //  m_oConfig.setSubnet(oIp);
  //}
  //CcXmlNode& rGateway = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Gateway];
  //if (rGateway.isNotNull() &&
  //  oIp.setIp(rGateway.innerText()))
  //{
  //  m_oConfig.setGateway(oIp);
  //}
  //CcXmlNode& rDns1 = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Dns];
  //if (rDns1.isNotNull() &&
  //  oIp.setIp(rDns1.innerText()))
  //{
  //  m_oConfig.setDns1(oIp);
  //}
  //CcXmlNode& rBootfile = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::Bootfile];
  //if (rBootfile.isNotNull())
  //{
  //  m_oConfig.setBootfile(rBootfile.innerText());
  //}
  //CcXmlNode& rBootfileMbr = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::BootfileMbr];
  //if (rBootfileMbr.isNotNull())
  //{
  //  m_oConfig.setBootfileMbr(rBootfileMbr.innerText());
  //}
  //CcXmlNode& rBootfileEfi = m_pPrivate->oConfigFile.rootNode()[CcDhcpGlobals::Server::Config::BootfileMbr];
  //if (rBootfileMbr.isNotNull())
  //{
  //  m_oConfig.setBootfileEfi(rBootfileEfi.innerText());
  //}
  return bRet;
}



const CcString& CConfig::getBootfile() const
{
  return m_sBootfile;
}

const CcString& CConfig::getBootfile(const CcString& sVendorClass) const
{
  EDhcpVendorClassId eClassId = EDhcpVendorClassId::IAx86;
  CcStringList sl = sVendorClass.split("::");
  if (sl.size() > 2)
  {
    if (sl[0].compareInsensitve("PXEClient") &&
        sl[1].compareInsensitve("Arch"))
    {
      eClassId = static_cast<EDhcpVendorClassId>(sl[2].toUint16());
      return getBootfile(eClassId);
    }
  }
  return getBootfile();
}

const CcString& CConfig::getBootfile(EDhcpVendorClassId eClassId) const
{
  switch (eClassId)
  {
    // Default 32bit BIOS
    case EDhcpVendorClassId::IAx86:
      if (m_sBootfileMbrx64.length() > 0)
        return m_sBootfileMbrx64;
      else if (m_sBootfileMbrx86.length() > 0)
        return m_sBootfileMbrx86;
      else if (m_sBootfileMbr.length() > 0)
        return m_sBootfileMbr;
      else
        return m_sBootfile;
      break;
    case EDhcpVendorClassId::EFIIA32:
      if (m_sBootfileEfix86.length() > 0)
        return m_sBootfileEfix86;
      else if (m_sBootfileEfi.length() > 0)
        return m_sBootfileEfi;
      else
        return m_sBootfile;
      break;
    case EDhcpVendorClassId::EFIXscale:
    case EDhcpVendorClassId::EFIx86_64:
      if (m_sBootfileEfix64.length() > 0)
        return m_sBootfileEfix64;
      else if (m_sBootfileEfi.length() > 0)
        return m_sBootfileEfi;
      else
        return m_sBootfile;
      break;
    default:
      return m_sBootfile;
  }
}

}

}
