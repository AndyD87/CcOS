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

const CcString EDeviceType::sAll("All");
const CcString EDeviceType::sCpu("Cpu");
const CcString EDeviceType::sUart("Uart");
const CcString EDeviceType::sSpi("Spi");
const CcString EDeviceType::sI2C("I2C");
const CcString EDeviceType::sDisplay("Display");
const CcString EDeviceType::sTouchPanel("TouchPanel");
const CcString EDeviceType::sNetwork("Network");
const CcString EDeviceType::sTimer("Timer");
const CcString EDeviceType::sCamera("Camera");
const CcString EDeviceType::sLed("Led");
const CcString EDeviceType::sHdd("Hdd");
const CcString EDeviceType::sGPIOPort("GPIOPort");

const CcString& EDeviceType::getString()
{
  switch (eValue)
  {
    case All:
      return sAll;
    case Cpu:
      return sCpu;
    case Uart:
      return sUart;
    case Spi:
      return sSpi;
    case I2C:
      return sI2C;
    case Display:
      return sDisplay;
    case TouchPanel:
      return sTouchPanel;
    case Network:
      return sNetwork;
    case Timer:
      return sTimer;
    case Camera:
      return sCamera;
    case Led:
      return sLed;
    case Hdd:
      return sHdd;
    case GPIOPort:
      return sGPIOPort;
  }
  return sAll;
}
