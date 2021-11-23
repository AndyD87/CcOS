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
 * @brief     Implementation of class OrangePiZeroGpioPin
 **/
#include <OrangePiZeroGpioPin.h>
#include "CcKernel.h"

#include "wiringPi.h"

OrangePiZeroGpioPin::OrangePiZeroGpioPin(void* pPort, uint8 uiPinNr) :
{
  reconfigure();
}

OrangePiZeroGpioPin::~OrangePiZeroGpioPin()
{
}

bool OrangePiZeroGpioPin::setDirection(EDirection eDirection, size_t uiValue)
{
  switch(eDirection)
  {
    case EDirection::Input:
      m_oGpioInitStruct.Mode  = INPUT;
      break;
    case EDirection::Output:
      m_oGpioInitStruct.Mode  = 0;//GPIO_MODE_OUTPUT_PP;
      break;
    case EDirection::Alternate:
      m_oGpioInitStruct.Mode      = 0;//GPIO_MODE_AF_PP;
      m_oGpioInitStruct.Alternate = 0;//uiValue;
      break;
    case EDirection::Analog:
      m_oGpioInitStruct.Mode      = 0;//GPIO_MODE_ANALOG;
      break;
    default:
      m_oGpioInitStruct.Mode      = 0;//GPIO_MODE_INPUT;
  }
  reconfigure();

  return false;
}

IGpioPin::EDirection OrangePiZeroGpioPin::getDirection()
{
  EDirection eDirection = EDirection::Unknown;
  return eDirection;
}

void OrangePiZeroGpioPin::setValue(bool bValue)
{
  //HAL_GPIO_WritePin(m_pPort, m_uiPinNr, bValue?GPIO_PIN_SET:GPIO_PIN_RESET);
}

bool OrangePiZeroGpioPin::getValue()
{
  //return HAL_GPIO_ReadPin(m_pPort, m_uiPinNr) == GPIO_PIN_SET;
  return false;
}

bool OrangePiZeroGpioPin::toggle()
{
  return false;
}

bool OrangePiZeroGpioPin::setSpeedValue(size_t uiValue)
 {
  reconfigure();
  return false;
 }

void OrangePiZeroGpioPin::reconfigure()
{

}
