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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of class CcIp
 */
#include "Types/CcIp.h"
#include "CcStringList.h"

CcIp::CcIp()
{
}

CcIp::CcIp(const CcString& sIpString)
{
  setIp(sIpString);
}

CcIp::CcIp(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0)
{
  setIpV4(uiIp3, uiIp2, uiIp1, uiIp0);
}

CcIp::CcIp(uint8* pIp)
{
  CCUNUSED(pIp);
}

CcIp::~CcIp()
{
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
  m_bIpV4 = bIpV4;
  if (bIpV4)
  {
    m_pBuffer = new uint8[4];
  }
  else
  {
    m_pBuffer = new uint16[8];
  }
}
