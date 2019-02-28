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
 * @page      CcDhcp
 * @subpage   CcDhcpServerConfig
 *
 * @page      CcDhcpServerConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpServerConfig
 **/
#ifndef _CcDhcpServerConfig_H_
#define _CcDhcpServerConfig_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "Network/CcCommonPorts.h"
#include "Network/CcSocketAddressInfo.h"

 // http://www.networksorcery.com/enp/rfc/rfc4578.txt
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
class CcDhcpSHARED CcDhcpServerConfig
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpServerConfig();

  /**
   * @brief Destructor
   */
  ~CcDhcpServerConfig();

  const CcSocketAddressInfo& getBindAddress() const
    { return m_oBindAddress; }

  void setIpBegin(const CcIp& oBegin)
    { m_oIpBegin = oBegin;}
  void setIpEnd(const CcIp& oEnd)
    { m_oIpEnd = oEnd;}
  void setSubnet(const CcIp& oSubnet)
    { m_oSubnet = oSubnet;}
  void setGateway(const CcIp& oGateway)
    { m_oGateway= oGateway;}
  void setDns1(const CcIp& oDns1)
    { m_oDns1= oDns1;}
  void setDns2(const CcIp& oDns2)
    { m_oDns2= oDns2;}
  void setNextServer(const CcIp& oNextServer)
    { m_oNextServer = oNextServer;}

  void setBootfile(const CcString& sBootfile)
    { m_sBootfile = sBootfile; }
  void setBootfileMbr(const CcString& sBootfileMbr)
    { m_sBootfileMbr = sBootfileMbr; }
  void setBootfileEfi(const CcString& sBootfileEfi)
    { m_sBootfileEfi = sBootfileEfi; }

  const CcIp& getIpBegin() const
    { return m_oIpBegin; }
  const CcIp& getIpEnd() const
    { return m_oIpEnd; }
  const CcIp& getSubnet() const
    { return m_oSubnet; }
  const CcIp& getGateway() const
    { return m_oGateway; }
  const CcIp& getDns1() const
    { return m_oDns1; }
  const CcIp& getDns2() const
    { return m_oDns2; }
  const CcIp& getNextServer() const
    { return m_oNextServer; }

  const CcString& getBootfile() const;
  const CcString& getBootfile(const CcString& sVendorClass) const;
  const CcString& getBootfile(EDhcpVendorClassId eVendorClassId) const;
  
  uint32 getLeaseTime() const
    { return m_uiLeaseTime; }
  uint32 getRenewTime() const
    { return m_uiRenewTime; }
  uint32 getRebindTime() const
    { return m_uiRebindTime; }
  
  uint32& leaseTime()
    { return m_uiLeaseTime; }
  uint32& renewTime()
    { return m_uiRenewTime; }
  uint32& rebindTime()
    { return m_uiRebindTime; }
private:
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
#endif /* _CcDhcpServerConfig_H_ */
