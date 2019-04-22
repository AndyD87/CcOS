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
 * @brief     Implementation of Class STM32F407VDriver
 */

#include "Driver/CPU/ST/STM32F407V/STM32F407V.h"
#include "Driver/CPU/ST/STM32F407V/STM32F407VDriver.h"
#include "Driver/CPU/ST/STM32F407V/STM32F407VTimer.h"
#include "Driver/CPU/ST/STM32F407V/STM32F407VSystemGpioPort.h"
#include "Driver/CPU/ST/STM32F407V/STM32F407VCpu.h"

#ifdef CCOS_DRIVER_NETWORK
  #include "Driver/CPU/ST/STM32F407V/STM32F407VNetwork.h"
#endif

#include "CcKernel.h"
#include <stm32f4xx_hal.h>

#define NUMBER_OF_PORTS 9
IGpioPort* g_pPort[NUMBER_OF_PORTS];

STM32F407VDriver::STM32F407VDriver ()
{
}

STM32F407VDriver::~STM32F407VDriver ()
{
}

CCEXTERNC void SystemCrashed()
{
  HAL_Init();
}

CcStatus STM32F407VDriver::entry()
{
  // Setup microcontroller
  HAL_Init();
  setupSystem();
#ifdef HAL_WWDG_MODULE_ENABLED
  setupWatchdog();
#endif // HAL_WWDG_MODULE_ENABLED
  // Setup GPIOs
  for(uint8 uiPortNr = 0; uiPortNr < NUMBER_OF_PORTS; uiPortNr++)
  {
    g_pPort[uiPortNr] = new STM32F407VSystemGpioPort(uiPortNr);
    CcKernel::addDevice(CcDeviceHandle(g_pPort[uiPortNr], EDeviceType::GPIOPort));
  }
#ifdef CCOS_DRIVER_NETWORK
  IDevice* pNetworkDevice = new STM32F407VNetwork();
  CcKernel::addDevice(CcDeviceHandle(pNetworkDevice,EDeviceType::Network));
#endif

  // Setup Timer2
  IDevice* pTimerDevice = new STM32F407VTimer();
  CcKernel::addDevice(CcDeviceHandle(pTimerDevice,EDeviceType::Timer));
  m_oSystemDevices.append(pTimerDevice);
  return true;
}

CcStatus STM32F407VDriver::unload()
{
  while(m_oSystemDevices.size())
  {
    delete m_oSystemDevices[0].ptr();
    m_oSystemDevices.remove(0);
  }
  // System device should never fail, we would have big problems
  return true;
}

IGpioPort* STM32F407VDriver::getGpioPort(size_t uiNr)
{
  IGpioPort* pPort = nullptr;
  if(uiNr < 8)
  {
    pPort = g_pPort[uiNr];
  }
  return pPort;
}

void STM32F407VDriver::setupSystem()
{
  STM32F407VCpu* pCpu = new STM32F407VCpu();
  CcKernel::addDevice(CcDeviceHandle(pCpu,EDeviceType::Cpu));
  m_oSystemDevices.append(pCpu);
}

void STM32F407VDriver::setupWatchdog()
{

}
