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
 * @brief     Implementation of Class STM32F407VETBoardLed
 *
 *            Control LED on STM32F407VETBoard.
 *            LEDs are connected on PORTD 12-15
 */

#include "STM32F407VETBoardLed.h"
#include "CcKernel.h"
#include <stm32f4xx_hal.h>
#include "CcStatic.h"
#include "Devices/IGpioPort.h"
#include "Devices/IGpioPin.h"

class STM32F407VETBoardLedPrivate
{
public:
  IGpioPin* pLedPin = nullptr;
  static IGpioPort* pLedPort;
};

IGpioPort* STM32F407VETBoardLedPrivate::pLedPort = nullptr;

STM32F407VETBoardLed::STM32F407VETBoardLed(uint8 uiLedNr)
{
  CCNEW(m_pPrivate, STM32F407VETBoardLedPrivate);
  if(STM32F407VETBoardLedPrivate::pLedPort == nullptr)
  {
    CcDeviceHandle oDevice = CcKernel::getDevice(EDeviceType::GpioPort, 3);
    STM32F407VETBoardLedPrivate::pLedPort = oDevice.cast<IGpioPort>().ptr();
  }
  if(STM32F407VETBoardLedPrivate::pLedPort != nullptr)
  {
    if(uiLedNr < 4)
    {
      m_pPrivate->pLedPin   = STM32F407VETBoardLedPrivate::pLedPort->getPin(uiLedNr + 12);
      m_pPrivate->pLedPin->setDirection(IGpioPin::EDirection::Output);
    }
  }
}

STM32F407VETBoardLed::~STM32F407VETBoardLed()
{
  CCDELETE(m_pPrivate);
}

CcStatus STM32F407VETBoardLed::setMaxBirghtness(uint16 uiBrightness)
{
  CcStatus oSuccess = false;
  CCUNUSED(uiBrightness);
  return oSuccess;
}

CcStatus STM32F407VETBoardLed::on(uint16 uiBrightness)
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

CcStatus STM32F407VETBoardLed::off()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->setValue(false);
  }
  return oSuccess;
}

CcStatus STM32F407VETBoardLed::toggle()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->toggle();
  }
  return oSuccess;
}

bool STM32F407VETBoardLed::IsOn()
{
  bool oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = m_pPrivate->pLedPin->getValue();
  }
  return oSuccess;
}
