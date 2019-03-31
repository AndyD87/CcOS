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
 * @brief     Implementation of class CcMacAddress
 */
#include <Network/CcMacAddress.h>
#include "CcStringList.h"

CcMacAddress::CcMacAddress()
{
}

CcMacAddress::CcMacAddress(const CcString& sMacString)
{
  setMac(sMacString);
}

CcMacAddress::CcMacAddress(uint8 uiMac5, uint8 uiMac4, uint8 uiMac3, uint8 uiMac2, uint8 uiMac1, uint8 uiMac0)
{
  setMac(uiMac5, uiMac4, uiMac3, uiMac2, uiMac1, uiMac0);
}

CcMacAddress::CcMacAddress(uint8* pMac, bool bRevert)
{
  setMac(pMac, bRevert);
}

CcMacAddress::~CcMacAddress()
{
}

CcMacAddress& CcMacAddress::operator=(CcMacAddress&& oToMove)
{
  if (&oToMove != this)
  {
    setMac(static_cast<uint8*>(oToMove.m_pBuffer));
  }
  return *this;
}

CcMacAddress& CcMacAddress::operator=(const CcMacAddress& oToCopy)
{
  setMac(oToCopy.m_pBuffer);
  return *this;
}

bool CcMacAddress::operator==(const CcMacAddress& oToCompare) const
{
  bool bRet = false;
  if (m_pBuffer[0] == oToCompare.m_pBuffer[0] &&
      m_pBuffer[1] == oToCompare.m_pBuffer[1] &&
      m_pBuffer[2] == oToCompare.m_pBuffer[2] &&
      m_pBuffer[3] == oToCompare.m_pBuffer[3] &&
      m_pBuffer[4] == oToCompare.m_pBuffer[4] &&
      m_pBuffer[5] == oToCompare.m_pBuffer[5])
  {
    bRet = true;
  }
  return bRet;
}

bool CcMacAddress::setMac(const CcString& sMacString)
{
  bool bRet = false;
  CcStringList MacList = sMacString.split(CcGlobalStrings::Seperators::MacAddress);
  if (MacList.size() < 6)
  {
    MacList = sMacString.split(CcGlobalStrings::Seperators::MacAddressMinus);
    if (MacList.size() < 6)
    {
      if(sMacString.length() == 12)
        MacList = sMacString.splitEvery(2);
    }
  }
  if (MacList.size() >= 6)
  {
    bool bOk;
    setMac( MacList.at(0).toUint8(&bOk, 16), 
            MacList.at(1).toUint8(&bOk, 16), 
            MacList.at(2).toUint8(&bOk, 16), 
            MacList.at(3).toUint8(&bOk, 16), 
            MacList.at(4).toUint8(&bOk, 16), 
            MacList.at(5).toUint8(&bOk, 16));
    bRet = true;
  }
  else
  {
    bRet = false;
  }
  return bRet;
}

bool CcMacAddress::setMac(uint8 uiMac5, uint8 uiMac4, uint8 uiMac3, uint8 uiMac2, uint8 uiMac1, uint8 uiMac0)
{
  m_pBuffer[0] = uiMac0;
  m_pBuffer[1] = uiMac1;
  m_pBuffer[2] = uiMac2;
  m_pBuffer[3] = uiMac3;
  m_pBuffer[4] = uiMac4;
  m_pBuffer[5] = uiMac5;
  return true;
}

bool CcMacAddress::setMac(const uint8* pMac, bool bRevert)
{
  if (bRevert)
  {
    m_pBuffer[0] = pMac[5];
    m_pBuffer[1] = pMac[4];
    m_pBuffer[2] = pMac[3];
    m_pBuffer[3] = pMac[2];
    m_pBuffer[4] = pMac[1];
    m_pBuffer[5] = pMac[0];
  }
  else
  {
    m_pBuffer[0] = pMac[0];
    m_pBuffer[1] = pMac[1];
    m_pBuffer[2] = pMac[2];
    m_pBuffer[3] = pMac[3];
    m_pBuffer[4] = pMac[4];
    m_pBuffer[5] = pMac[5];
  }
  return true;
}

CcString CcMacAddress::getString(const CcString& sSeparator, bool bUpper) const
{
  CcString sRet;
  sRet.append(CcString::fromNumber(m_pBuffer[5], 16).fillBeginUpToLength("0", 2));
  sRet.append(sSeparator);
  sRet.append(CcString::fromNumber(m_pBuffer[4], 16).fillBeginUpToLength("0", 2));
  sRet.append(sSeparator);
  sRet.append(CcString::fromNumber(m_pBuffer[3], 16).fillBeginUpToLength("0", 2));
  sRet.append(sSeparator);
  sRet.append(CcString::fromNumber(m_pBuffer[2], 16).fillBeginUpToLength("0", 2));
  sRet.append(sSeparator);
  sRet.append(CcString::fromNumber(m_pBuffer[1], 16).fillBeginUpToLength("0", 2));
  sRet.append(sSeparator);
  sRet.append(CcString::fromNumber(m_pBuffer[0], 16).fillBeginUpToLength("0", 2));
  if (bUpper)
    sRet.toUpper();
  return sRet;
}

bool CcMacAddress::isNull() const
{
  bool bRet = false;
  if (m_pBuffer[0] == 0 &&
      m_pBuffer[1] == 0 &&
      m_pBuffer[2] == 0 &&
      m_pBuffer[3] == 0 &&
      m_pBuffer[4] == 0 &&
      m_pBuffer[5] == 0)
  {
    bRet = true;
  }
  return bRet;
}
