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
 * @brief     Class CcSocketAddressInfo
 */
#include "Network/CcSocketAddressInfo.h"
#include "Network/ISocket.h"
#include "CcStatic.h"
#include "CcStringList.h"
#include "CcGlobalStrings.h"

#define Cc_AF_INET  2

#define Cc_SOCK_STREAM     1  
#define Cc_SOCK_DGRAM      2  
#define Cc_SOCK_RAW        3  
#define Cc_SOCK_RDM        4  
#define Cc_SOCK_SEQPACKET  5 

#define Cc_IPPROTO_TCP 6
#define Cc_IPPROTO_UDP 17

#define Cc_AI_PASSIVE 1

CcSocketAddressInfo::CcSocketAddressInfo()
{
  init(ESocketType::Unknown);
}

CcSocketAddressInfo::CcSocketAddressInfo(ESocketType eSocketType)
{
  init(eSocketType);
}

CcSocketAddressInfo::CcSocketAddressInfo(ESocketType eSocketType, const CcIp& rIp, uint16 uiPort)
{
  init(eSocketType);
  setPort(uiPort);
  setIp(rIp);
}

CcSocketAddressInfo::CcSocketAddressInfo(const CcSocketAddressInfo& oToCopy)
{
  init(ESocketType::Unknown);
  operator=(oToCopy);
}

CcSocketAddressInfo::~CcSocketAddressInfo()
{
}

CcSocketAddressInfo& CcSocketAddressInfo::operator=(CcSocketAddressInfo&& oToMove)
{
  if (this != &oToMove)
  {
    ai_flags      = oToMove.ai_flags;
    ai_family     = oToMove.ai_family;
    ai_socktype   = oToMove.ai_socktype;
    ai_protocol   = oToMove.ai_protocol;
    ai_addrlen    = oToMove.ai_addrlen;
    ai_canonname  = oToMove.ai_canonname;
    CcStatic::memcpy(&ai_addr, &oToMove.ai_addr, sizeof(CcTypes_sockaddr_in));
    ai_next       = oToMove.ai_next;
    oToMove.ai_addrlen = 0;
    oToMove.ai_canonname = nullptr;
    oToMove.ai_next = nullptr;
  }
  return *this;
}

CcSocketAddressInfo& CcSocketAddressInfo::operator=(const CcSocketAddressInfo& oToCopy)
{
  ai_flags      = oToCopy.ai_flags;
  ai_family     = oToCopy.ai_family;
  ai_socktype   = oToCopy.ai_socktype;
  ai_protocol   = oToCopy.ai_protocol;
  ai_canonname  = oToCopy.ai_canonname;
  ai_next       = oToCopy.ai_next;

  CcStatic::memcpy(&ai_addr, &oToCopy.ai_addr, sizeof(CcTypes_sockaddr_in));
  ai_addrlen = sizeof(CcTypes_sockaddr_in);

  return *this;
}

void CcSocketAddressInfo::init(ESocketType eSocketType)
{
  ai_addrlen = sizeof(CcTypes_sockaddr_in);
  CcStatic::memset(&ai_addr, 0, sizeof(CcTypes_sockaddr_in));
  switch (eSocketType)
  {
    case ESocketType::TCP:
      ai_family = Cc_AF_INET;
      ai_socktype = Cc_SOCK_STREAM;
      ai_protocol = Cc_IPPROTO_TCP;
      ai_flags = Cc_AI_PASSIVE;
      ai_addr.sin_family = Cc_AF_INET;
      break;
    case ESocketType::UDP:
      ai_family = Cc_AF_INET;
      ai_socktype = Cc_SOCK_DGRAM;
      ai_protocol = Cc_IPPROTO_UDP;
      ai_addr.sin_family = Cc_AF_INET;
      break;
    default:
      break;
  }
}

void CcSocketAddressInfo::setPort(uint16 uiPort)
{
  ai_addr.sin_port = htons(uiPort);
}

void CcSocketAddressInfo::setIp(const CcIp& oIp)
{
  ai_addr.sin_addr.S_un_b.s_b4 = oIp.getIpV4_0();
  ai_addr.sin_addr.S_un_b.s_b3 = oIp.getIpV4_1();
  ai_addr.sin_addr.S_un_b.s_b2 = oIp.getIpV4_2();
  ai_addr.sin_addr.S_un_b.s_b1 = oIp.getIpV4_3();
}

void CcSocketAddressInfo::setIpPort(const CcString& sIpPort, bool* pbOk)
{
  size_t uiPosSeparator = sIpPort.find(CcGlobalStrings::Seperators::Colon);
  if (uiPosSeparator < sIpPort.length() &&
      uiPosSeparator < sIpPort.length() + 1)
  {
    setIp(sIpPort.substr(0, uiPosSeparator));
    setPort(sIpPort.substr(uiPosSeparator + 1));
    if (pbOk != nullptr)
      *pbOk = true;
  }
  else
  {
    if (pbOk != nullptr)
      *pbOk = false;
  }
}

CcIp CcSocketAddressInfo::getIp() const
{
  CcIp oRet;
  oRet.setIpV4( (uint8)  (ai_addr.sin_addr.S_addr & 0x000000ff),
                (uint8) ((ai_addr.sin_addr.S_addr & 0x0000ff00) >> 8),
                (uint8) ((ai_addr.sin_addr.S_addr & 0x00ff0000) >> 16),
                (uint8) ((ai_addr.sin_addr.S_addr & 0xff000000) >> 24));
  return oRet;
}

uint16 CcSocketAddressInfo::getPort() const
{
  return htons(ai_addr.sin_port);
}

CcString CcSocketAddressInfo::getPortString() const
{
  CcString sRet;
  sRet.appendNumber(getPort());
  return sRet;
}

void CcSocketAddressInfo::setAddressData(CcTypes_sockaddr_in *pData, size_t uiSizeofData)
{
  if (uiSizeofData <= ai_addrlen)
  {
    CcStatic::memcpy(&ai_addr, pData, uiSizeofData);
  }
}
