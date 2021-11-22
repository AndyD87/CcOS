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
 * @brief     Implementation of Class OrangePiZeroLed
 *
 *            Control LED on OrangePiZero.
 *            LEDs are connected on PORTD 12-15
 */

#include "OrangePiZeroLed.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "Devices/IGpioPort.h"
#include "Devices/IGpioPin.h"

class OrangePiZeroLedPrivate
{
public:
  IGpioPin* pLedPin = nullptr;
  static IGpioPort* pLedPort;
};

IGpioPort* OrangePiZeroLedPrivate::pLedPort = nullptr;

OrangePiZeroLed::OrangePiZeroLed(uint8 uiLedNr)
{
  CCNEW(m_pPrivate, OrangePiZeroLedPrivate);
  if(OrangePiZeroLedPrivate::pLedPort == nullptr)
  {
    CcDevice oDevice = CcKernel::getDevice(EDeviceType::GpioPort, 4);
    OrangePiZeroLedPrivate::pLedPort = oDevice.getDevice<IGpioPort>();
  }
  if(OrangePiZeroLedPrivate::pLedPort != nullptr)
  {
    if(uiLedNr < 4)
    {
      m_pPrivate->pLedPin   = OrangePiZeroLedPrivate::pLedPort->getPin(uiLedNr + 8);
      m_pPrivate->pLedPin->setDirection(IGpioPin::EDirection::Output);
    }
  }
}

OrangePiZeroLed::~OrangePiZeroLed()
{
  CCDELETE(m_pPrivate);
}

CcStatus OrangePiZeroLed::setMaxBirghtness(uint16 uiBrightness)
{
  CcStatus oSuccess = false;
  CCUNUSED(uiBrightness);
  return oSuccess;
}

CcStatus OrangePiZeroLed::on(uint16 uiBrightness)
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

CcStatus OrangePiZeroLed::off()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->setValue(false);
  }
  return oSuccess;
}

CcStatus OrangePiZeroLed::toggle()
{
  CcStatus oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = true;
    m_pPrivate->pLedPin->toggle();
  }
  return oSuccess;
}

bool OrangePiZeroLed::IsOn()
{
  bool oSuccess = false;
  if(m_pPrivate->pLedPin)
  {
    oSuccess = m_pPrivate->pLedPin->getValue();
  }
  return oSuccess;
}
