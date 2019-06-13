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
 * @brief     Implementation of Class STM3220GEVALLed
 *
 *            Control LED on STM3220GEVAL.
 *            LEDs are connected on PORTD 12-15
 */

#include <Driver/Board/ST/STM3220GEVAL/STM3220GEVALLed.h>
#include "CcKernel.h"
#include <stm32f2xx_hal.h>
#include "CcStatic.h"
#include "Devices/IGpioPort.h"
#include "Devices/IGpioPin.h"

class STM3220GEVALLedPrivate
{
public:
  IGpioPin* pLedPin = nullptr;
  IGpioPort* pLedPort;
};

STM3220GEVALLed::STM3220GEVALLed(uint8 uiLedNr)
{
  m_pPrivate = new STM3220GEVALLedPrivate();
  CCMONITORNEW(m_pPrivate);
  switch(uiLedNr)
  {
    case 0:
      mapPortPin(6,6);
      break;
    case 1:
      mapPortPin(6,8);
      break;
    case 2:
      mapPortPin(8,9);
      break;
    case 3:
      mapPortPin(2,7);
      break;

  }
}

STM3220GEVALLed::~STM3220GEVALLed()
{
  CCDELETE(m_pPrivate);
}

CcStatus STM3220GEVALLed::setMaxBirghtness(uint16 uiBrightness)
{
  CcStatus oSuccess = false;
  CCUNUSED(uiBrightness);
  return oSuccess;
}

CcStatus STM3220GEVALLed::on(uint16 uiBrightness)
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

CcStatus STM3220GEVALLed::off()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->setValue(false);
  }
  return oSuccess;
}

CcStatus STM3220GEVALLed::toggle()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->toggle();
  }
  return oSuccess;
}

bool STM3220GEVALLed::IsOn()
{
  bool oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = m_pPrivate->pLedPin->getValue();
  }
  return oSuccess;
}

void STM3220GEVALLed::mapPortPin(uint8 uiPort, uint8 uiPin)
{
  CcDeviceHandle oDevice = CcKernel::getDevice(EDeviceType::GpioPort, uiPort);
  m_pPrivate->pLedPort = oDevice.cast<IGpioPort>().ptr();
  if(m_pPrivate->pLedPort != nullptr)
  {
    m_pPrivate->pLedPin   = m_pPrivate->pLedPort->getPin(uiPin);
    m_pPrivate->pLedPin->setDirection(IGpioPin::EDirection::Output);
    m_pPrivate->pLedPin->reconfigure();
  }
}
