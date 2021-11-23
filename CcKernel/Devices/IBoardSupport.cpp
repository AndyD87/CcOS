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
 * @brief     Class IBoardSupport
 */
#include "Devices/IBoardSupport.h"

IBoardSupport::CHwDevice  IBoardSupport::InvalidDevice;
IBoardSupport::CHwPin     IBoardSupport::InvalidPin;
IBoardSupport::CHwPort    IBoardSupport::InvalidPort;


IBoardSupport::CHwPin& IBoardSupport::CHwPort::getHwPin(uint16 uiPin)
{
  if(oHwPins.size() > uiPin)
  {
    return oHwPins[uiPin];
  }
  return InvalidPin;
}


IBoardSupport::~IBoardSupport()
{
  for(CHwDevice& pHwDevice : m_oHwDevices)
  {
    CCDELETE(pHwDevice.pDevice);
  }
  m_oHwDevices.clear();
}

uint32 IBoardSupport::getDeviceSize(EDeviceType eDeviceType) const
{
  uint32 uiSize = 0;
  for(IBoardSupport::CHwDevice& oDevice : m_oHwDevices)
  {
    if(oDevice.eDevice == eDeviceType)
    {
      uiSize++;
    }
  }
  return uiSize;
}

IBoardSupport::CHwDevice& IBoardSupport::getHwDevice(EDeviceType eDeviceType, uint32 uiDeviceNumber, uint32& uiFunctionNr)
{
  uiFunctionNr = 0;
  for(IBoardSupport::CHwDevice& oDevice : m_oHwDevices)
  {
    if(oDevice.eDevice == eDeviceType && oDevice.uiDeviceNr == uiDeviceNumber)
    {
      return oDevice;
    }
    uiFunctionNr++;
  }
  return InvalidDevice;
}

IBoardSupport::CHwPort& IBoardSupport::getHwPort(uint16 uiPort)
{
  for(IBoardSupport::CHwPort& oDevice : m_oHwPorts)
  {
    if(oDevice.uiPortNr == uiPort)
    {
      return oDevice;
    }
  }
  return InvalidPort;
}

bool IBoardSupport::verifyFreePort(const CHwDevice& oHwDevice)
{
  bool bFree = true;
  for(const CHwDevice::CPort& oPort : oHwDevice.oRequiredPins)
  {
    for(uint16 uiPin : oPort.oPins)
    {
      CHwPort& oHwPort = getHwPort(oPort.uiPort);
      if(&oHwPort != &InvalidPort)
      {
        CHwPin& oHwPin = oHwPort.getHwPin(uiPin);
        if(&oHwPin != &InvalidPin)
        {
          if(oHwPin.uiSelectedDeviceNr < m_oHwDevices.size())
          {
            bFree = false;
            break;
          }
        }
        else
        {
          bFree = false;
          break;
        }
      }
      else
      {
        bFree = false;
        break;
      }
    }
    if(bFree == false)
      break;
  }
  return bFree;
}

bool IBoardSupport::setHwDeviceUsage(const CHwDevice &oHwDevice, uint32 uiUsedFunction)
{
  bool bFree = true;
  for(const CHwDevice::CPort& oPort : oHwDevice.oRequiredPins)
  {
    for(uint16 uiPin : oPort.oPins)
    {
      CHwPort& oHwPort = getHwPort(oPort.uiPort);
      if(&oHwPort != &InvalidPort)
      {
        CHwPin& oHwPin = oHwPort.getHwPin(uiPin);
        if(&oHwPin != &InvalidPin)
        {
          if(oHwPin.uiSelectedDeviceNr >= m_oHwDevices.size())
          {
            oHwPin.uiSelectedDeviceNr = uiUsedFunction;
          }
          else
          {
            bFree = false;
            break;
          }
        }
        else
        {
          bFree = false;
          break;
        }
      }
      else
      {
        bFree = false;
        break;
      }
    }
    if(bFree == false)
      break;
  }
  return bFree;
}
