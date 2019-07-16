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
 * @brief     Implementation of class CcIp
 */
#include "Types/CcIp.h"
#include "CcStringList.h"
#include "Network/CcCommonIps.h"
#include "CcStatic.h"

CcIp::CcIp()
{
  setIpV4(0, 0, 0, 0);
}

CcIp::CcIp(const CcString& sIpString)
{
  setIp(sIpString);
}

CcIp::CcIp(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0)
{
  setIpV4(uiIp3, uiIp2, uiIp1, uiIp0);
}

CcIp::CcIp(const uint8* pIpV4, bool bSwap)
{
  setIpV4(pIpV4, bSwap);
}

CcIp::~CcIp()
{
}

CcIp& CcIp::operator=(CcIp&& oToMove)
{
  if (&oToMove != this)
  {
    CcStatic::memcpy(Data.m_pArrayV6, oToMove.Data.m_pArrayV6, sizeof(oToMove.Data.m_pArrayV6));
    m_eIpType = oToMove.m_eIpType;
  }
  return *this;
}

CcIp& CcIp::operator=(const CcIp& oToCopy)
{
  CcStatic::memcpy(Data.m_pArrayV6, oToCopy.Data.m_pArrayV6, sizeof(oToCopy.Data.m_pArrayV6));
  m_eIpType = oToCopy.m_eIpType;
  return *this;
}

bool CcIp::operator==(const CcIp& oToCompare) const
{
  bool bRet = false;
  if (m_eIpType == oToCompare.m_eIpType)
  {
    if (m_eIpType == EIpType::IPv4 &&
        Data.m_pArrayV4Uint32 == oToCompare.Data.m_pArrayV4Uint32)
    {
      bRet = true;
    }
    else if(m_eIpType == EIpType::IPv6 &&
            Data.m_pArrayV6Uint64[0] == oToCompare.Data.m_pArrayV6Uint64[0] &&
            Data.m_pArrayV6Uint64[1] == oToCompare.Data.m_pArrayV6Uint64[1])
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcIp::operator<(const CcIp& oToCompare) const
{
  bool bRet = false;
  if (m_eIpType == oToCompare.m_eIpType)
  {
    if (m_eIpType == EIpType::IPv4 &&
        Data.m_pArrayV4Uint32 < oToCompare.Data.m_pArrayV4Uint32)
    {
      bRet = true;
    }
    else if(m_eIpType == EIpType::IPv6 &&
            Data.m_pArrayV6Uint64[1] < oToCompare.Data.m_pArrayV6Uint64[1] &&
            Data.m_pArrayV6Uint64[0] < oToCompare.Data.m_pArrayV6Uint64[0])
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcIp::operator>(const CcIp& oToCompare) const
{
  bool bRet = false;
  if (m_eIpType == oToCompare.m_eIpType)
  {
    if (m_eIpType == EIpType::IPv4 &&
        Data.m_pArrayV4Uint32 > oToCompare.Data.m_pArrayV4Uint32)
    {
      bRet = true;
    }
    else if(m_eIpType == EIpType::IPv6 &&
            Data.m_pArrayV6Uint64[1] > oToCompare.Data.m_pArrayV6Uint64[1] &&
            Data.m_pArrayV6Uint64[0] > oToCompare.Data.m_pArrayV6Uint64[0])
    {
      bRet = true;
    }
  }
  return bRet;
}


bool CcIp::setIp(const CcString& sIpString)
{
  bool bRet = false;
  CcStringList ipList(sIpString.split("."));
  if (ipList.size() >= 4)
  {
    setIpV4(ipList.at(0).toUint8(), ipList.at(1).toUint8(), ipList.at(2).toUint8(), ipList.at(3).toUint8());
    bRet = true;
  }
  else
  {
    setIpV4(0,0,0,0);
    bRet = false;
  }
  return bRet;
}

bool CcIp::setIpV4(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0)
{
  m_eIpType = EIpType::IPv4;
  Data.m_pArrayV4[0] = uiIp0;
  Data.m_pArrayV4[1] = uiIp1;
  Data.m_pArrayV4[2] = uiIp2;
  Data.m_pArrayV4[3] = uiIp3;
  return true;
}

bool CcIp::setIpV4(const uint8* pIpV4, bool bSwap)
{
  m_eIpType = EIpType::IPv4;
  if (!bSwap)
  {
    Data.m_pArrayV4[0] = pIpV4[0];
    Data.m_pArrayV4[1] = pIpV4[1];
    Data.m_pArrayV4[2] = pIpV4[2];
    Data.m_pArrayV4[3] = pIpV4[3];
  }
  else
  {
    Data.m_pArrayV4[3] = pIpV4[0];
    Data.m_pArrayV4[2] = pIpV4[1];
    Data.m_pArrayV4[1] = pIpV4[2];
    Data.m_pArrayV4[0] = pIpV4[3];
  }
  return true;
}

CcIp& CcIp::add(uint32 iToAdd)
{
  if (m_eIpType == EIpType::IPv4)
  {
    Data.m_pArrayV4Uint32 += iToAdd;
  }
  else if (m_eIpType == EIpType::IPv6)
  {
    uint64 uiTemp = Data.m_pArrayV6Uint64[0];
    Data.m_pArrayV6Uint64[0] += iToAdd;
    // overflow detected?
    if (uiTemp > Data.m_pArrayV6Uint64[0])
    {
      Data.m_pArrayV6Uint64[1]++;
    }
  }
  return *this;
}

bool CcIp::isNullIp() const
{
  bool bRet = false;
  if (m_eIpType == EIpType::IPv4)
  {
    bRet = Data.m_pArrayV4Uint32 == 0;
  }
  else if (m_eIpType == EIpType::IPv6)
  {
    bRet =  Data.m_pArrayV6Uint64[0] == 0;
    bRet &= Data.m_pArrayV6Uint64[1] == 0;
  }
  return bRet;
}

bool CcIp::isMulticastIp() const
{
  bool bRet = false;
  if (*this > CcCommonIps::MulticastBegin &&
      *this < CcCommonIps::MulticastEnd)
  {
    bRet = true;
  }
  return bRet;
}

uint32 CcIp::getUint32(bool bReverse) const
{
  uint32 uiRet = 0;
  if (m_eIpType == EIpType::IPv4)
  {
    if(bReverse)
    {
      uiRet  = ( static_cast<uint32>(Data.m_pArrayV4[3]));
      uiRet |= ( static_cast<uint32>(Data.m_pArrayV4[2]) << 8);
      uiRet |= ( static_cast<uint32>(Data.m_pArrayV4[1]) << 16);
      uiRet |= ( static_cast<uint32>(Data.m_pArrayV4[0]) << 24);
    }
    else
    {
      uiRet = Data.m_pArrayV4Uint32;
    }
  }
  return uiRet;
}


CcString CcIp::getString() const
{
  CcString sRet;
  if (m_eIpType == EIpType::IPv4)
  {
    sRet.appendNumber(Data.m_pArrayV4[3]);
    sRet.append('.');
    sRet.appendNumber(Data.m_pArrayV4[2]);
    sRet.append('.');
    sRet.appendNumber(Data.m_pArrayV4[1]);
    sRet.append('.');
    sRet.appendNumber(Data.m_pArrayV4[0]);
  }
  return sRet;
}
