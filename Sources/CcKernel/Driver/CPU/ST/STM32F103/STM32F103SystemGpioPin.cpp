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
 * @brief     Implementation of class STM32F103SystemGpioPin
 **/
#include "STM32F103SystemGpioPin.h"
#include "CcKernel.h"

STM32F103SystemGpioPin::STM32F103SystemGpioPin(GPIO_TypeDef* pPort, uint8 uiPinNr) :
    pPort(pPort),
    uiPinNr(static_cast<uint16>(1) << uiPinNr)
{
  CcStatic_memsetZeroObject(oGpioInitStruct);
  oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  oGpioInitStruct.Pin   = uiPinNr;
  oGpioInitStruct.Pull  = GPIO_NOPULL;
  oGpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  reconfigure();
}

STM32F103SystemGpioPin::~STM32F103SystemGpioPin()
{
}

bool STM32F103SystemGpioPin::setDirection(EDirection eDirection, size_t uiValue)
{
  CCUNUSED(uiValue);
  switch(eDirection)
  {
    case EDirection::Input:
      oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
      break;
    case EDirection::Output:
      oGpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
      break;
    case EDirection::Alternate:
      oGpioInitStruct.Mode  = GPIO_MODE_AF_PP;
      break;
    case EDirection::Analog:
      oGpioInitStruct.Mode  = GPIO_MODE_ANALOG;
      break;
    default:
      oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  }
  reconfigure();

  return false;
}

IGpioPin::EDirection STM32F103SystemGpioPin::getDirection()
{
  EDirection eDirection = EDirection::Unknown;
  switch(oGpioInitStruct.Mode)
  {
    case GPIO_MODE_INPUT:
      eDirection = EDirection::Input;
      break;
    case GPIO_MODE_OUTPUT_OD:
      CCFALLTHROUGH;
    case GPIO_MODE_OUTPUT_PP:
      eDirection = EDirection::Output;
      break;
    case GPIO_MODE_AF_OD:
      CCFALLTHROUGH;
    case GPIO_MODE_AF_PP:
      eDirection = EDirection::Alternate;
      break;
    case GPIO_MODE_ANALOG:
      eDirection = EDirection::Analog;
      break;
  }
  return eDirection;
}

void STM32F103SystemGpioPin::setValue(bool bValue)
{
  HAL_GPIO_WritePin((GPIO_TypeDef*)pPort,
                    (uint16_t)uiPinNr,
                    (GPIO_PinState)(bValue?GPIO_PIN_SET:GPIO_PIN_RESET));
}

bool STM32F103SystemGpioPin::getValue()
{
  return HAL_GPIO_ReadPin(pPort, uiPinNr) == GPIO_PIN_SET;
}

bool STM32F103SystemGpioPin::toggle()
{
  HAL_GPIO_TogglePin(pPort, uiPinNr);
  return true;
}

bool STM32F103SystemGpioPin::setSpeedValue(size_t uiValue)
 {
  oGpioInitStruct.Speed  = uiValue;
  reconfigure();
  return true;
 }

void STM32F103SystemGpioPin::reconfigure()
{
  HAL_GPIO_Init(pPort, &oGpioInitStruct);
}
