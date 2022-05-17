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
 * @brief     Class CConfig
 **/
#pragma once

#include "CcBase.h"
#include "CcDhcp.h"
#include "Network/CcCommonPorts.h"
#include "Network/CcSocketAddressInfo.h"
#include "Config/CcConfig.h"

namespace NDhcp
{

namespace NServer
{

//! @brief Enum of dhcp vendor from @see http://www.networksorcery.com/enp/rfc/rfc4578.txt
enum class EDhcpVendorClassId
{
  IAx86 = 0,
  NecPc98 = 1,
  IA64 = 2,
  DecAlpha = 3,
  ArcX86 = 4,
  IntelLeanClient = 5,
  EFIIA32 = 6,
  EFI = 7,
  EFIXscale = 8,
  EFIx86_64 = 9,
};
/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CConfig
{
public:
  /**
   * @brief Constructor
   */
  CConfig();

  /**
   * @brief Destructor
   */
  ~CConfig();

  bool loadConfigFile(const CcString& sPath);
  bool parseConfigData();

  //! @return Bind address for dhcp listining interface
  const CcSocketAddressInfo& getBindAddress() const
  { return m_oBindAddress; }
  //! @return Editable bind address for dhcp listining interface
  CcSocketAddressInfo& getBindAddress()
  { return m_oBindAddress; }
  //! @return Ip address range begin
  const CcIp& getIpBegin() const
  { return m_oIpBegin; }
  //! @return Ip address range end
  const CcIp& getIpEnd() const
  { return m_oIpEnd; }
  //! @return Subnet for client
  const CcIp& getSubnet() const
  { return m_oSubnet; }
  //! @return Gateway for client
  const CcIp& getGateway() const
  { return m_oGateway; }
  //! @return Get first dns server
  const CcIp& getDns1() const
  { return m_oDns1; }
  //! @return Get second dns server
  const CcIp& getDns2() const
  { return m_oDns2; }
  //! @return Get next server for pxe boot
  const CcIp& getNextServer() const
  { return m_oNextServer; }

  //! @return Get bootfile
  const CcString& getBootfile() const;
  /**
   * @brief Get bootfile depending on vendor class
   * @param sVendorClass: Vendor class as string
   * @return Bootfile as string
   */
  const CcString& getBootfile(const CcString& sVendorClass) const;
  /**
   * @brief Get bootfile depending on vendor class
   * @param eVendorClassId: Vendor class as enum
   * @return Bootfile as string
   */
  const CcString& getBootfile(EDhcpVendorClassId eVendorClassId) const;

  //! @return Get leastime for client
  uint32 getLeaseTime() const
  { return m_uiLeaseTime; }
  //! @return Get renew time for client
  uint32 getRenewTime() const
  { return m_uiRenewTime; }
  //! @return Get rebind time for client
  uint32 getRebindTime() const
  { return m_uiRebindTime; }

  //! @param oBegin: Set ip range begin
  void setIpBegin(const CcIp& oBegin)
  { m_oIpBegin = oBegin; }
  //! @param oEnd: Set ip range end
  void setIpEnd(const CcIp& oEnd)
  { m_oIpEnd = oEnd; }
  //! @param oSubnet: Set subnet
  void setSubnet(const CcIp& oSubnet)
  { m_oSubnet = oSubnet; }
  //! @param oGateway: Set gateway
  void setGateway(const CcIp& oGateway)
  { m_oGateway = oGateway; }
  //! @param oDns1: Set first dns
  void setDns1(const CcIp& oDns1)
  { m_oDns1 = oDns1; }
  //! @param oDns2: Set second dns
  void setDns2(const CcIp& oDns2)
  { m_oDns2 = oDns2; }
  //! @param oNextServer: Set next server for pxe boot
  void setNextServer(const CcIp& oNextServer)
  { m_oNextServer = oNextServer; }
  //! @param sBootfile: Set bootfile to load from next server
  void setBootfile(const CcString& sBootfile)
  { m_sBootfile = sBootfile; }
  //! @param sBootfileMbr: Set bootfile for mbr boot
  void setBootfileMbr(const CcString& sBootfileMbr)
  { m_sBootfileMbr = sBootfileMbr; }
  //! @param sBootfileEfi: Set bootfile for efi boot
  void setBootfileEfi(const CcString& sBootfileEfi)
  { m_sBootfileEfi = sBootfileEfi; }

  //! @return Get editable lease time
  uint32& leaseTime()
  { return m_uiLeaseTime; }
  //! @return Get renew lease time
  uint32& renewTime()
  { return m_uiRenewTime; }
  //! @return Get rebind lease time
  uint32& rebindTime()
  { return m_uiRebindTime; }
private:
  CcConfigMap         m_oConfigMap;

  CcSocketAddressInfo m_oBindAddress;

  CcIp                m_oGateway;
  CcIp                m_oIpBegin;
  CcIp                m_oIpEnd;
  CcIp                m_oSubnet;
  CcIp                m_oDns1;
  CcIp                m_oDns2;

  CcIp                m_oNextServer;

  uint32              m_uiLeaseTime;
  uint32              m_uiRenewTime;
  uint32              m_uiRebindTime;

  CcString            m_sBootfile;
  CcString            m_sBootfileEfi;
  CcString            m_sBootfileEfix86;
  CcString            m_sBootfileEfix64;
  CcString            m_sBootfileMbr;
  CcString            m_sBootfileMbrx86;
  CcString            m_sBootfileMbrx64;
};

}

}
