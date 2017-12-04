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

CcIp::CcIp()
{
  createBuffer(true);
}

CcIp::CcIp(const CcString& sIpString)
{
  setIp(sIpString);
}

CcIp::CcIp(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0)
{
  setIpV4(uiIp3, uiIp2, uiIp1, uiIp0);
}

CcIp::CcIp(uint8* pIpV4)
{
  setIpV4(pIpV4);
}

CcIp::~CcIp()
{
}

CcIp& CcIp::operator=(const CcIp& oToCopy)
{
  if(m_bIpV4)
  {
    setIpV4(static_cast<uint8*>(oToCopy.m_pBuffer));
  }
  return *this;
}

CcIp& CcIp::operator=(CcIp&& oToMove)
{
  if (&oToMove != this)
  {
    deleteBuffer();
    m_pBuffer = oToMove.m_pBuffer;
    m_bIpV4 = oToMove.m_bIpV4;
    oToMove.m_pBuffer = nullptr;
  }
  return *this;
}

bool CcIp::operator==(const CcIp& oToCompare) const
{
  bool bRet = false;
  if (m_bIpV4 == oToCompare.m_bIpV4)
  {
    if (m_bIpV4 == true &&
        *static_cast<uint32*>(m_pBuffer) == *static_cast<uint32*>(oToCompare.m_pBuffer))
    {
      bRet = true;
    }
    else if(static_cast<uint64*>(m_pBuffer)[0] == static_cast<uint64*>(oToCompare.m_pBuffer)[0] &&
            static_cast<uint64*>(m_pBuffer)[1] == static_cast<uint64*>(oToCompare.m_pBuffer)[1])
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcIp::operator<(const CcIp& oToCompare) const
{
  bool bRet = false;
  if (m_bIpV4 == oToCompare.m_bIpV4)
  {
    if (m_bIpV4 == true &&
      *static_cast<uint32*>(m_pBuffer) < *static_cast<uint32*>(oToCompare.m_pBuffer))
    {
      bRet = true;
    }
    else if (static_cast<uint64*>(m_pBuffer)[0] < static_cast<uint64*>(oToCompare.m_pBuffer)[0] &&
             static_cast<uint64*>(m_pBuffer)[1] < static_cast<uint64*>(oToCompare.m_pBuffer)[1])
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcIp::operator>(const CcIp& oToCompare) const
{
  bool bRet = false;
  if (m_bIpV4 == oToCompare.m_bIpV4)
  {
    if (m_bIpV4 == true &&
      *static_cast<uint32*>(m_pBuffer) > *static_cast<uint32*>(oToCompare.m_pBuffer))
    {
      bRet = true;
    }
    else if (static_cast<uint64*>(m_pBuffer)[0] > static_cast<uint64*>(oToCompare.m_pBuffer)[0] &&
             static_cast<uint64*>(m_pBuffer)[1] > static_cast<uint64*>(oToCompare.m_pBuffer)[1])
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
  createBuffer(true);
  static_cast<uint8*>(m_pBuffer)[0] = uiIp0;
  static_cast<uint8*>(m_pBuffer)[1] = uiIp1;
  static_cast<uint8*>(m_pBuffer)[2] = uiIp2;
  static_cast<uint8*>(m_pBuffer)[3] = uiIp3;
  return true;
}

bool CcIp::setIpV4(const uint8* pIpV4)
{
  createBuffer(true);
  static_cast<uint8*>(m_pBuffer)[0] = pIpV4[0];
  static_cast<uint8*>(m_pBuffer)[1] = pIpV4[1];
  static_cast<uint8*>(m_pBuffer)[2] = pIpV4[2];
  static_cast<uint8*>(m_pBuffer)[3] = pIpV4[3];
  return true;
}

CcIp& CcIp::add(uint32 iToAdd)
{
  if (m_bIpV4)
  {
    *static_cast<uint32*>(m_pBuffer) += iToAdd;
  }
  else
  {
    if (static_cast<uint64*>(m_pBuffer)[0] > 0xffffffff00000000)
    {
      static_cast<uint64*>(m_pBuffer)[0]+= iToAdd;
      static_cast<uint64*>(m_pBuffer)[1]++;
    }
    else
    {
      static_cast<uint64*>(m_pBuffer)[0] += iToAdd;
    }
  }
  return *this;
}

bool CcIp::isNullIp() const
{
  bool bRet = false;
  if (m_bIpV4)
  {
    bRet = *static_cast<uint32*>(m_pBuffer) == 0;
  }
  else
  {
    bRet =  static_cast<uint64*>(m_pBuffer)[0] == 0;
    bRet &= static_cast<uint64*>(m_pBuffer)[1] == 0;
  }
  return bRet;
}

uint32 CcIp::getUint32() const
{
  uint32 uiRet = 0;
  if (m_bIpV4)
  {
    uiRet  = ( static_cast<uint8>(static_cast<uint8*>(m_pBuffer)[3]));
    uiRet |= ( static_cast<uint8>(static_cast<uint8*>(m_pBuffer)[2]) << 8);
    uiRet |= ( static_cast<uint8>(static_cast<uint8*>(m_pBuffer)[1]) << 16);
    uiRet |= ( static_cast<uint8>(static_cast<uint8*>(m_pBuffer)[0]) << 24);
  }
  return uiRet;
}


CcString CcIp::getString() const
{
  CcString sRet;
  if (m_bIpV4)
  {
    sRet.appendNumber(static_cast<uint8*>(m_pBuffer)[3]);
    sRet.append('.');
    sRet.appendNumber(static_cast<uint8*>(m_pBuffer)[2]);
    sRet.append('.');
    sRet.appendNumber(static_cast<uint8*>(m_pBuffer)[1]);
    sRet.append('.');
    sRet.appendNumber(static_cast<uint8*>(m_pBuffer)[0]);
  }
  return sRet;
}


void CcIp::createBuffer(bool bIpV4)
{
  deleteBuffer();
  m_bIpV4 = bIpV4;
  if (bIpV4)
  {
    m_pBuffer = new uint8[4]{0};
  }
  else
  {
    m_pBuffer = new uint16[8]{0};
  }
}

void CcIp::deleteBuffer()
{
  if(m_bIpV4)
  {
    uint8* pBuffer = static_cast<uint8*>(m_pBuffer);
    CCDELETE(pBuffer);
  }
  else
  {
    uint16* pBuffer = static_cast<uint16*>(m_pBuffer);
    CCDELETE(pBuffer);
  }
  m_pBuffer = nullptr;
}
