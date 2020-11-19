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
 * @brief     Implementation of Class STM3220GEVALDriver
 */

#include <Driver/Board/ST/STM3220GEVAL/STM3220GEVAL.h>
#include <Driver/Board/ST/STM3220GEVAL/STM3220GEVALDriver.h>
#include <Driver/Board/ST/STM3220GEVAL/STM3220GEVALLed.h>
#include "CcKernel.h"

STM3220GEVALDriver::STM3220GEVALDriver()
{
}

STM3220GEVALDriver::~STM3220GEVALDriver()
{
}

CcStatus STM3220GEVALDriver::entry()
{
  // Load all leds:
  CcDevice hDevice(new STM3220GEVALLed(0), EDeviceType::Led);
  CcKernel::addDevice(hDevice);
  CcDevice hDevice1(new STM3220GEVALLed(1), EDeviceType::Led);
  CcKernel::addDevice(hDevice1);
  CcDevice hDevice2(new STM3220GEVALLed(2), EDeviceType::Led);
  CcKernel::addDevice(hDevice2);
  CcDevice hDevice3(new STM3220GEVALLed(3), EDeviceType::Led);
  CcKernel::addDevice(hDevice3);
  return true;
}

CcStatus STM3220GEVALDriver::unload()
{
  return true;
}
