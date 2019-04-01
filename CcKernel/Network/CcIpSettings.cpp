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
 * @brief     Implementation of class CcIpSettings
 */
#include <Network/CcIpSettings.h>

void CcIpSettings::setSubnet(const CcIp& oSubnet)
{
  uint8 uiPos = 0x80;
  uint8 uiNextValue = 0;
  uiSubnet = 0;
  if(oSubnet.getIpV4_3() == 0xff)
  {
    uiSubnet=8;
    if(oSubnet.getIpV4_2() == 0xff)
    {
      uiSubnet=16;
      if(oSubnet.getIpV4_1() == 0xff)
      {
        uiSubnet=24;
        uiNextValue = oSubnet.getIpV4_0();
      }
      else
      {
        uiNextValue = oSubnet.getIpV4_1();
      }
    }
    else
    {
      uiNextValue = oSubnet.getIpV4_2();
    }
  }
  else
  {
    uiNextValue = oSubnet.getIpV4_3();
  }
  while(uiNextValue & uiPos)
  {
    uiSubnet++;
    uiPos >>= 1;
  }
}

CcIp CcIpSettings::getSubnetIp()
{
  CcIp oRet;
  if (uiSubnet >= 8)
  {
    oRet.getIpV4_3() = 0xff;
    if (uiSubnet >= 16)
    {
      oRet.getIpV4_2() = 0xff;
      if (uiSubnet >= 24)
      {
        oRet.getIpV4_1() = 0xff;
        oRet.getIpV4_0() = setTopBits(uiSubnet - 24);
      }
      else
      {
        oRet.getIpV4_1() = setTopBits(uiSubnet - 16);
      }
    }
    else
    {
      oRet.getIpV4_2() = setTopBits(uiSubnet - 8);
    }
  }
  else
  {
    oRet.getIpV4_3() = setTopBits(uiSubnet);
  }
  return oRet;
}

uint8 CcIpSettings::setTopBits(uint8 uiNumber)
{
  uint8 uiPos = 0x80;
  uint8 uiValue = 0;
  while(uiNumber)
  {
    uiNumber--;
    uiValue |= uiPos;
    uiPos >>= 1;
  }
  return uiValue;
}

CcIpSettings& CcIpSettings::operator=(const CcIpSettings& oToCopy)
{
  pInterface = oToCopy.pInterface;
  oIpAddress = oToCopy.oIpAddress;
  uiSubnet = oToCopy.uiSubnet;
  oGateway = oToCopy.oGateway;
  oDns1 = oToCopy.oDns1;
  oDns2 = oToCopy.oDns2;
  return *this;
}

CcIpSettings& CcIpSettings::operator=(CcIpSettings&& oToMove)
{
  if (this != &oToMove)
  {
    pInterface = std::move(oToMove.pInterface);
    oIpAddress = std::move(oToMove.oIpAddress);
    uiSubnet = oToMove.uiSubnet;
    oGateway = std::move(oToMove.oGateway);
    oDns1 = std::move(oToMove.oDns1);
    oDns2 = std::move(oToMove.oDns2);
    oToMove.uiSubnet = 0;
  }
  return *this;
}

bool CcIpSettings::operator==(const CcIpSettings& oToCompare) const
{
  bool bRet = false;
  if (pInterface == oToCompare.pInterface &&
      oIpAddress == oToCompare.oIpAddress &&
      uiSubnet == oToCompare.uiSubnet &&
      oGateway == oToCompare.oGateway &&
      oDns1 == oToCompare.oDns1 &&
      oDns2 == oToCompare.oDns2)
  {
    bRet = true;
  }
  return bRet;
}