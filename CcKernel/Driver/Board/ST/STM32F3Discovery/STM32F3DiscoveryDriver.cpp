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
 * @brief     Implementation of Class STM32F4DiscoveryDriver
 */

#include "STM32F4Discovery.h"
#include "STM32F4DiscoveryDriver.h"
#include "CcKernel.h"
#include "STM32F4DiscoveryLed.h"

STM32F4DiscoveryDriver::STM32F4DiscoveryDriver()
{
}

STM32F4DiscoveryDriver::~STM32F4DiscoveryDriver()
{
}

CcStatus STM32F4DiscoveryDriver::entry()
{
  // Load all leds:
  CcDeviceHandle hDevice(new STM32F4DiscoveryLed(0), EDeviceType::Led);
  CcKernel::addDevice(hDevice);
  CcDeviceHandle hDevice1(new STM32F4DiscoveryLed(1), EDeviceType::Led);
  CcKernel::addDevice(hDevice1);
  CcDeviceHandle hDevice2(new STM32F4DiscoveryLed(2), EDeviceType::Led);
  CcKernel::addDevice(hDevice2);
  CcDeviceHandle hDevice3(new STM32F4DiscoveryLed(3), EDeviceType::Led);
  CcKernel::addDevice(hDevice3);
  CcDeviceHandle hDevice4(new STM32F4DiscoveryLed(4), EDeviceType::Led);
  CcKernel::addDevice(hDevice4);
  CcDeviceHandle hDevice5(new STM32F4DiscoveryLed(5), EDeviceType::Led);
  CcKernel::addDevice(hDevice5);
  CcDeviceHandle hDevice6(new STM32F4DiscoveryLed(6), EDeviceType::Led);
  CcKernel::addDevice(hDevice6);
  CcDeviceHandle hDevice7(new STM32F4DiscoveryLed(7), EDeviceType::Led);
  CcKernel::addDevice(hDevice7);
  return true;
}
