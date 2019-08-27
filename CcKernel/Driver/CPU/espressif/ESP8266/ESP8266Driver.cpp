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
 * @brief     Implementation of Class ESP8266Driver
 */

#include "ESP8266Driver.h"
#include "CcKernel.h"
#include "ESP8266Cpu.h"
#include "ESP8266GpioPort.h"

ESP8266Driver::ESP8266Driver ()
{
}

ESP8266Driver::~ESP8266Driver ()
{
}

CcStatus ESP8266Driver::entry()
{
  CCNEWTYPE(pCpu, ESP8266Cpu);
  m_oSystemDevices.append(pCpu);
  CCNEWTYPE(pPort, ESP8266GpioPort);
  m_oSystemDevices.append(pPort);
  CcKernel::addDevice(CcDeviceHandle(pCpu, EDeviceType::Cpu));
  CcKernel::addDevice(CcDeviceHandle(pPort, EDeviceType::GpioPort));
  return true;
}

CcStatus ESP8266Driver::unload()
{
  while(m_oSystemDevices.size())
  {
    IDevice* pDevice = m_oSystemDevices[0].ptr();
    CCDELETE(pDevice);
    m_oSystemDevices.remove(0);
  }
  // System device should never fail, we would have big problems
  return true;
}
