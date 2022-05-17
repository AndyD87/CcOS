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
 * @brief     Implementation of Class STM32F103Driver
 */

#include "Driver/CPU/ST/STM32F103/STM32F103.h"
#include "Driver/CPU/ST/STM32F103/STM32F103Driver.h"
#include "Driver/CPU/ST/STM32F103/STM32F103Timer.h"
#include "Driver/CPU/ST/STM32F103/STM32F103SystemGpioPort.h"
#include "Driver/CPU/ST/STM32F103/STM32F103Cpu.h"

#ifdef CCOS_GENERIC_NETWORK
  #include "Driver/CPU/ST/STM32F103/STM32F103Network.h"
#endif

#include "CcKernel.h"
#include "CcDevice.h"
#include <stm32f1xx_hal.h>

STM32F103Driver::STM32F103Driver ()
{
}

STM32F103Driver::~STM32F103Driver ()
{
}

CcStatus STM32F103Driver::entry()
{
  // Setup microcontroller
  HAL_Init();
  setupSystem();
#ifdef HAL_WWDG_MODULE_ENABLED
  setupWatchdog();
#endif // HAL_WWDG_MODULE_ENABLED
  // Setup Gpio
  for(uint8 uiPortNr = 0; uiPortNr < NUMBER_OF_PORTS; uiPortNr++)
  {
    m_pPort[uiPortNr] = new STM32F103SystemGpioPort(uiPortNr);
    CcKernel::addDevice(CcDevice(m_pPort[uiPortNr], EDeviceType::GpioPort));
  }
#ifdef CCOS_GENERIC_NETWORK
  IDevice* pNetworkDevice = new STM32F103Network();
  CcKernel::addDevice(CcDevice(pNetworkDevice,EDeviceType::Network));
#endif

  // Setup Timer2
  IDevice* pTimerDevice = new STM32F103Timer();
  CcKernel::addDevice(CcDevice(pTimerDevice,EDeviceType::Timer));
  m_oSystemDevices.append(pTimerDevice);
  return true;
}

CcStatus STM32F103Driver::unload()
{
  while(m_oSystemDevices.size())
  {
    delete m_oSystemDevices[0].ptr();
    m_oSystemDevices.remove(0);
  }
  // System device should never fail, we would have big problems
  return true;
}

IGpioPort* STM32F103Driver::getGpioPort(size_t uiNr)
{
  IGpioPort* pPort = nullptr;
  if(uiNr < NUMBER_OF_PORTS)
  {
    pPort = m_pPort[uiNr];
  }
  return pPort;
}

void STM32F103Driver::setupSystem()
{
  STM32F103Cpu* pCpu = new STM32F103Cpu();
  CcKernel::addDevice(CcDevice(pCpu,EDeviceType::Cpu));
  m_oSystemDevices.append(pCpu);
}

void STM32F103Driver::setupWatchdog()
{

}
