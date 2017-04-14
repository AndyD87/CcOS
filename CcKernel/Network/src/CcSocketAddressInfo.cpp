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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSocketAddressInfo
 */
#include "Network/CcSocketAddressInfo.h"
#include "stdio.h"
#include "string.h"
#include "CcStringList.h"

CcSocketAddressInfo::CcSocketAddressInfo()
{
}

CcSocketAddressInfo::CcSocketAddressInfo(const CcSocketAddressInfo& oToCopy)
{
  memcpy(&m_oAddressData, &oToCopy.m_oAddressData, sizeof(&m_oAddressData));
}

CcSocketAddressInfo::~CcSocketAddressInfo( void )
{
}

void CcSocketAddressInfo::setPort(uint16 uiPort)
{
  ((CcTypes_sockaddr_in*) &m_oAddressData)->sin_port = uiPort;
}

void CcSocketAddressInfo::setIp(const CcString& sIpString, bool& bOk)
{
  CcStringList ipList(sIpString.split(","));
  if (ipList.size() >= 4)
  {
    setIp(ipList.at(0).toUint8(), ipList.at(0).toUint8(), ipList.at(0).toUint8(), ipList.at(0).toUint8());
    bOk = true;
  }
  else
  {
    bOk = false;
  }
}

void CcSocketAddressInfo::setIp(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0)
{
  ((CcTypes_sockaddr_in*) &m_oAddressData)->sin_addr.S_un_b.s_b4 = uiIp3;
  ((CcTypes_sockaddr_in*) &m_oAddressData)->sin_addr.S_un_b.s_b3 = uiIp2;
  ((CcTypes_sockaddr_in*) &m_oAddressData)->sin_addr.S_un_b.s_b2 = uiIp1;
  ((CcTypes_sockaddr_in*) &m_oAddressData)->sin_addr.S_un_b.s_b1 = uiIp0;
}

ipv4_t CcSocketAddressInfo::getIPv4(void) const
{
  ipv4_t oRet;
  struct CcTypes_sockaddr_in *s = (struct CcTypes_sockaddr_in *)&m_oAddressData;
  oRet.ip4 = (uint8)  (s->sin_addr.S_addr & 0x000000ff);
  oRet.ip3 = (uint8) ((s->sin_addr.S_addr & 0x0000ff00) >> 8);
  oRet.ip2 = (uint8) ((s->sin_addr.S_addr & 0x00ff0000) >> 16);
  oRet.ip1 = (uint8) ((s->sin_addr.S_addr & 0xff000000) >> 24);
  return oRet;
}

CcString CcSocketAddressInfo::getIPv4String() const
{
  CcString sRet;
  sRet.appendIPv4(getIPv4());
  return sRet;
}

void CcSocketAddressInfo::setIpPort(const CcString& sIpString, bool& bOk)
{
  CcStringList ipList(sIpString.split(","));
  if (ipList.size() >= 6)
  {
    setIp(ipList.at(0).toUint8(), ipList.at(0).toUint8(), ipList.at(0).toUint8(), ipList.at(0).toUint8());
    uint16 uiPort;
    uiPort = ipList.at(4).toUint8() * 0x0100;
    uiPort += ipList.at(5).toUint8();
    setPort(uiPort);
    bOk = true;
  }
  else
  {
    bOk = false;
  }
}

uint16 CcSocketAddressInfo::getPort(void) const
{
  uint16 oRet;
  struct CcTypes_sockaddr_in *s = (struct CcTypes_sockaddr_in *)&m_oAddressData;
  oRet = (uint16) (s->sin_port);
  return oRet;
}

CcString CcSocketAddressInfo::getPortString() const
{
  CcString sRet;
  sRet.appendNumber(getPort());
  return sRet;
}

void CcSocketAddressInfo::setAddressData(CcTypes_sockaddr_in *pData, size_t uiSizeofData)
{
  if (uiSizeofData <= sizeof(m_oAddressData))
  {
    memcpy(&m_oAddressData, pData, uiSizeofData);
  }
}