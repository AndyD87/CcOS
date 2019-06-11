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
 * @brief     Class IDevice
 */

#include "IDevice.h"

const CcString IDevice::sAll("All");
const CcString IDevice::sCpu("Cpu");
const CcString IDevice::sUart("Uart");
const CcString IDevice::sSpi("Spi");
const CcString IDevice::sI2C("I2C");
const CcString IDevice::sDisplay("Display");
const CcString IDevice::sTouchPanel("TouchPanel");
const CcString IDevice::sNetwork("Network");
const CcString IDevice::sTimer("Timer");
const CcString IDevice::sCamera("Camera");
const CcString IDevice::sLed("Led");
const CcString IDevice::sHdd("Hdd");
const CcString IDevice::sGPIOPort("GPIOPort");
const CcString IDevice::sGPIOPin("GPIOPin");

const CcString& IDevice::getString(EDeviceType eType)
{
  switch (eType)
  {
    case EDeviceType::All:
      return sAll;
    case EDeviceType::Cpu:
      return sCpu;
    case EDeviceType::Uart:
      return sUart;
    case EDeviceType::Spi:
      return sSpi;
    case EDeviceType::I2C:
      return sI2C;
    case EDeviceType::Display:
      return sDisplay;
    case EDeviceType::TouchPanel:
      return sTouchPanel;
    case EDeviceType::Network:
      return sNetwork;
    case EDeviceType::Timer:
      return sTimer;
    case EDeviceType::Camera:
      return sCamera;
    case EDeviceType::Led:
      return sLed;
    case EDeviceType::Hdd:
      return sHdd;
    case EDeviceType::GPIOPort:
      return sGPIOPort;
    case EDeviceType::GPIOPin:
      return sGPIOPin;
  }
  return sAll;
}
