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
  deleteBuffer();
}

CcIp& CcIp::operator=(CcIp&& oToMove)
{
  if (&oToMove != this)
  {
    deleteBuffer();
    Data.m_pBuffer = oToMove.Data.m_pBuffer;
    m_eIpType = oToMove.m_eIpType;
    oToMove.Data.m_pBuffer = nullptr;
    oToMove.m_eIpType = EIpType::Unknown;
  }
  return *this;
}

CcIp& CcIp::operator=(const CcIp& oToCopy)
{
  if(oToCopy.m_eIpType == EIpType::IPv4)
  {
    setIpV4(static_cast<uint8*>(oToCopy.Data.m_pBuffer));
  }
  return *this;
}

bool CcIp::operator==(const CcIp& oToCompare) const
{
  bool bRet = false;
  if (m_eIpType == oToCompare.m_eIpType)
  {
    if (m_eIpType == EIpType::IPv4 &&
        *static_cast<uint32*>(Data.m_pBuffer) == *static_cast<uint32*>(oToCompare.Data.m_pBuffer))
    {
      bRet = true;
    }
    else if(m_eIpType == EIpType::IPv6 &&
            static_cast<uint64*>(Data.m_pBuffer)[0] == static_cast<uint64*>(oToCompare.Data.m_pBuffer)[0] &&
            static_cast<uint64*>(Data.m_pBuffer)[1] == static_cast<uint64*>(oToCompare.Data.m_pBuffer)[1])
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
      *static_cast<uint32*>(Data.m_pBuffer) < *static_cast<uint32*>(oToCompare.Data.m_pBuffer))
    {
      bRet = true;
    }
    else if (m_eIpType == EIpType::IPv6 &&
             static_cast<uint64*>(Data.m_pBuffer)[0] < static_cast<uint64*>(oToCompare.Data.m_pBuffer)[0] &&
             static_cast<uint64*>(Data.m_pBuffer)[1] < static_cast<uint64*>(oToCompare.Data.m_pBuffer)[1])
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
      *static_cast<uint32*>(Data.m_pBuffer) > *static_cast<uint32*>(oToCompare.Data.m_pBuffer))
    {
      bRet = true;
    }
    else if (m_eIpType == EIpType::IPv6 &&
             static_cast<uint64*>(Data.m_pBuffer)[0] > static_cast<uint64*>(oToCompare.Data.m_pBuffer)[0] &&
             static_cast<uint64*>(Data.m_pBuffer)[1] > static_cast<uint64*>(oToCompare.Data.m_pBuffer)[1])
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
    bRet = false;
  }
  return bRet;
}

bool CcIp::setIpV4(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0)
{
  checkBuffer(EIpType::IPv4);
  Data.m_pArrayV4[0] = uiIp0;
  Data.m_pArrayV4[1] = uiIp1;
  Data.m_pArrayV4[2] = uiIp2;
  Data.m_pArrayV4[3] = uiIp3;
  return true;
}

bool CcIp::setIpV4(const uint8* pIpV4, bool bSwap)
{
  checkBuffer(EIpType::IPv4);
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
    *static_cast<uint32*>(Data.m_pBuffer) += iToAdd;
  }
  else if (m_eIpType == EIpType::IPv6)
  {
    if (static_cast<uint64*>(Data.m_pBuffer)[0] > 0xffffffff00000000)
    {
      static_cast<uint64*>(Data.m_pBuffer)[0]+= iToAdd;
      static_cast<uint64*>(Data.m_pBuffer)[1]++;
    }
    else
    {
      static_cast<uint64*>(Data.m_pBuffer)[0] += iToAdd;
    }
  }
  return *this;
}

bool CcIp::isNullIp() const
{
  bool bRet = false;
  if (m_eIpType == EIpType::IPv4)
  {
    bRet = *static_cast<uint32*>(Data.m_pBuffer) == 0;
  }
  else if (m_eIpType == EIpType::IPv6)
  {
    bRet =  static_cast<uint64*>(Data.m_pBuffer)[0] == 0;
    bRet &= static_cast<uint64*>(Data.m_pBuffer)[1] == 0;
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
      uiRet = *Data.m_pArrayV4Uint32;
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

void CcIp::checkBuffer(EIpType eType)
{
  if(eType != m_eIpType)
  {
    createBuffer(eType);
  }
}

void CcIp::createBuffer(EIpType eType)
{
  deleteBuffer();
  m_eIpType = eType;
  switch (m_eIpType)
  {
    case EIpType::IPv4:
      Data.m_pArrayV4 = new uint8[4]{0};
      break;
    case EIpType::IPv6:
      Data.m_pArrayV6 = new uint16[8]{0};
      break;
    default:
      Data.m_pBuffer = nullptr;
      break;
  }
}

void CcIp::deleteBuffer()
{
  switch (m_eIpType)
  {
    case EIpType::IPv4:
    {
      uint8* pBuffer = Data.m_pArrayV4;
      CCDELETE(pBuffer);
      break;
    }
    case EIpType::IPv6:
    {
      uint16* pBuffer = Data.m_pArrayV6;
      CCDELETE(pBuffer);
      break;
    }
    default:
      break;
  }
  // m_pBuffer is alway null after delete
  Data.m_pBuffer = nullptr;
}
