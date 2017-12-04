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
#ifndef CcDhcpServerConfig_H_
#define CcDhcpServerConfig_H_

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
  CcDhcpServerConfig(void);

  /**
   * @brief Destructor
   */
  ~CcDhcpServerConfig(void);

  const CcSocketAddressInfo& getBindAddress()
    { return m_oBindAddress; }

  void setIpV4Begin(const CcIp& oBegin)
    { m_oIpV4Begin = oBegin;}
  void setIpV4End(const CcIp& oEnd)
    { m_oIpV4End = oEnd;}

  const CcIp& getIpV4Begin() const
    { return m_oIpV4Begin; }
  const CcIp& getIpV4End() const
    { return m_oIpV4End; }
  const CcIp& getIpV4Subnet() const
    { return m_oIpV4Subnet; }
  const CcIp& getIpV4Default() const
    { return m_oIpV4Default; }
  const CcIp& getIpV4Dns1() const
    { return m_oIpV4Dns1; }
  const CcIp& getIpV4Dns2() const
    { return m_oIpV4Dns2; }


  const CcIp& getIpV4Server() const
    { return m_oIpV4Server; }

  const CcString& getBootfile();
  const CcString& getBootfile(const CcString& sVendorClass);
  const CcString& getBootfile(EDhcpVendorClassId eVendorClassId);
  
  uint32& leaseTime()
    { return m_uiLeaseTime; }
  uint32& renewTime()
    { return m_uiRenewTime; }
  uint32& rebindTime()
    { return m_uiRebindTime; }
private:
  CcSocketAddressInfo m_oBindAddress;

  CcIp                m_oIpV4Server;
  CcIp                m_oIpV4Begin;
  CcIp                m_oIpV4End;
  CcIp                m_oIpV4Subnet;
  CcIp                m_oIpV4Default;
  CcIp                m_oIpV4Dns1;
  CcIp                m_oIpV4Dns2;

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
#endif /* CcDhcpServerConfig_H_ */
