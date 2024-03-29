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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcIpInterface
 */
#include "Network/CcIpInterface.h"
#include "Devices/CcDeviceNetwork.h"

void CcIpInterface::setSubnet(const CcIp& oSubnet)
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

CcIp CcIpInterface::getSubnetIp()
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

CcDeviceNetwork CcIpInterface::getDevice() const
{
  return CcDeviceNetwork(pDevice);
}

uint8 CcIpInterface::setTopBits(uint8 uiNumber)
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

bool CcIpInterface::isInSubnet(const CcIp& oIp)
{
  uint32 uiIp = oIpAddress.getUint32(false);
  uint32 uiIpToTest = oIp.getUint32(false);
  uint32 uiMask = 0xffffffff << (32 - uiSubnet);
  uint32 uiMin = uiIp & uiMask;
  uint32 uiMax = uiIp | ~uiMask;
  if( uiIpToTest > uiMin &&
      uiIpToTest <= uiMax )
    return true;
  else
    return false;
}

CcIpInterface& CcIpInterface::operator=(const CcIpInterface& oToCopy)
{
  pDevice = oToCopy.pDevice;
  oIpAddress = oToCopy.oIpAddress;
  uiSubnet = oToCopy.uiSubnet;
  oGateway = oToCopy.oGateway;
  return *this;
}

CcIpInterface& CcIpInterface::operator=(CcIpInterface&& oToMove)
{
  if (this != &oToMove)
  {
    pDevice = CCMOVE(oToMove.pDevice);
    oIpAddress = CCMOVE(oToMove.oIpAddress);
    uiSubnet = oToMove.uiSubnet;
    oGateway = CCMOVE(oToMove.oGateway);
    oToMove.uiSubnet = 0;
  }
  return *this;
}

bool CcIpInterface::operator==(const CcIpInterface& oToCompare) const
{
  bool bRet = false;
  if (pDevice == oToCompare.pDevice &&
      oIpAddress == oToCompare.oIpAddress &&
      uiSubnet == oToCompare.uiSubnet &&
      oGateway == oToCompare.oGateway)
  {
    bRet = true;
  }
  return bRet;
}
