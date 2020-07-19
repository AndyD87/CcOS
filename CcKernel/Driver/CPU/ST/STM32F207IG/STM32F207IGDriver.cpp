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
 * @brief     Implementation of Class STM32F207IGDriver
 */

#include "Driver/CPU/ST/STM32F207IG/STM32F207IG.h"
#include "Driver/CPU/ST/STM32F207IG/STM32F207IGDriver.h"
//#include "Driver/CPU/ST/STM32F207IG/STM32F207IGTimer.h"
#include "Driver/CPU/ST/STM32F207IG/STM32F207IGSystemGpioPort.h"
#include "Driver/CPU/ST/STM32F207IG/STM32F207IGCpu.h"

#ifdef CCOS_GENERIC_NETWORK
  #include "Driver/CPU/ST/STM32F207IG/STM32F207IGNetwork.h"
#endif

#include "CcKernel.h"
#include <stm32f2xx_hal.h>

#define NUMBER_OF_PORTS 9
IGpioPort* g_pPort[NUMBER_OF_PORTS];

STM32F207IGDriver::STM32F207IGDriver ()
{
}

STM32F207IGDriver::~STM32F207IGDriver ()
{
}

CCEXTERNC void SystemCrashed()
{
  HAL_Init();
}

CcStatus STM32F207IGDriver::entry()
{
  // Setup microcontroller
  HAL_Init();
  setupSystem();
#ifdef HAL_WWDG_MODULE_ENABLED
  setupWatchdog();
#endif // HAL_WWDG_MODULE_ENABLED
  // Setup Gpios
  for(uint8 uiPortNr = 0; uiPortNr <NUMBER_OF_PORTS; uiPortNr++)
  {
    g_pPort[uiPortNr] = new STM32F207IGSystemGpioPort(uiPortNr);
    CcKernel::addDevice(CcDeviceHandle(g_pPort[uiPortNr], EDeviceType::GpioPort));
  }
#ifdef CCOS_GENERIC_NETWORK
  IDevice* pNetworkDevice = new STM32F207IGNetwork();
  CcKernel::addDevice(CcDeviceHandle(pNetworkDevice,EDeviceType::Network));
#endif
  return true;
}

CcStatus STM32F207IGDriver::unload()
{
  while(m_oSystemDevices.size())
  {
    delete m_oSystemDevices[0].ptr();
    m_oSystemDevices.remove(0);
  }
  // System device should never fail, we would have big problems
  return true;
}

IGpioPort* STM32F207IGDriver::getGpioPort(size_t uiNr)
{
  IGpioPort* pPort = nullptr;
  if(uiNr < 8)
  {
    pPort = g_pPort[uiNr];
  }
  return pPort;
}

void STM32F207IGDriver::setupSystem()
{
  STM32F207IGCpu* pCpu = new STM32F207IGCpu();
  CcKernel::addDevice(CcDeviceHandle(pCpu,EDeviceType::Cpu));
  m_oSystemDevices.append(pCpu);
}

void STM32F207IGDriver::setupWatchdog()
{

}
