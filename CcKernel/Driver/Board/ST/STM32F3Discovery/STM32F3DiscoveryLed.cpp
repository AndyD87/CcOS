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
 * @brief     Implementation of Class STM32F4DiscoveryLed
 *
 *            Control LED on STM32F4Discovery.
 *            LEDs are connected on PORTD 12-15
 */

#include "STM32F4DiscoveryLed.h"
#include "CcKernel.h"
#include <stm32f4xx_hal.h>
#include "CcStatic.h"
#include "Devices/IGpioPort.h"
#include "Devices/IGpioPin.h"

class STM32F4DiscoveryLedPrivate
{
public:
  IGpioPin* pLedPin = nullptr;
  static IGpioPort* pLedPort;
};

IGpioPort* STM32F4DiscoveryLedPrivate::pLedPort = nullptr;

STM32F4DiscoveryLed::STM32F4DiscoveryLed(uint8 uiLedNr)
{
  m_pPrivate = new STM32F4DiscoveryLedPrivate();
  CCMONITORNEW(m_pPrivate);
  if(STM32F4DiscoveryLedPrivate::pLedPort == nullptr)
  {
    CcDeviceHandle oDevice = CcKernel::getDevice(EDeviceType::GpioPort, 4);
    STM32F4DiscoveryLedPrivate::pLedPort = oDevice.cast<IGpioPort>().ptr();
  }
  if(STM32F4DiscoveryLedPrivate::pLedPort != nullptr)
  {
    if(uiLedNr < 4)
    {
      m_pPrivate->pLedPin   = STM32F4DiscoveryLedPrivate::pLedPort->getPin(uiLedNr + 8);
      m_pPrivate->pLedPin->setDirection(IGpioPin::EDirection::Output);
      m_pPrivate->pLedPin->reconfigure();
    }
  }
}

STM32F4DiscoveryLed::~STM32F4DiscoveryLed()
{
  CCDELETE(m_pPrivate);
}

CcStatus STM32F4DiscoveryLed::setMaxBirghtness(uint16 uiBrightness)
{
  CcStatus oSuccess = false;
  CCUNUSED(uiBrightness);
  return oSuccess;
}

CcStatus STM32F4DiscoveryLed::on(uint16 uiBrightness)
{
  CCUNUSED(uiBrightness);
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->setValue(true);
  }
  return oSuccess;
}

CcStatus STM32F4DiscoveryLed::off()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->setValue(false);
  }
  return oSuccess;
}

CcStatus STM32F4DiscoveryLed::toggle()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->toggle();
  }
  return oSuccess;
}

bool STM32F4DiscoveryLed::IsOn()
{
  bool oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = m_pPrivate->pLedPin->getValue();
  }
  return oSuccess;
}
