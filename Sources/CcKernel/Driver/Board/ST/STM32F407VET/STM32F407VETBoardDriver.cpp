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
 * @brief     Implementation of Class STM32F407VETBoardDriver
 */

#include "STM32F407VETBoard.h"
#include "STM32F407VETBoardDriver.h"
#include "CcKernel.h"
#include "STM32F407VETBoardLed.h"
#include "CcDevice.h"

STM32F407VETBoardDriver::STM32F407VETBoardDriver()
{
}

STM32F407VETBoardDriver::~STM32F407VETBoardDriver()
{
}

CcStatus STM32F407VETBoardDriver::entry()
{
  // Load all leds:
  CcDevice hDevice(new STM32F407VETBoardLed(0), EDeviceType::Led);
  CcKernel::addDevice(hDevice);
  CcDevice hDevice1(new STM32F407VETBoardLed(1), EDeviceType::Led);
  CcKernel::addDevice(hDevice1);
  return true;
}

CcStatus STM32F407VETBoardDriver::unload()
{
  return true;
}
