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
 * @brief     Implementation of class STM32F407SystemGpioPin
 **/
#include <STM32F407SystemGpioPin.h>
#include "CcKernel.h"
#include <stm32f4xx_hal.h>
#include <STM32F407Driver.h>

STM32F407SystemGpioPin::STM32F407SystemGpioPin(void* pPort, uint8 uiPinNr) :
  m_pPort(static_cast<GPIO_TypeDef*>(pPort)),
  m_uiPinNr(static_cast<uint32>(1) << uiPinNr)
{
  CcStatic_memsetZeroObject(m_oGpioInitStruct);
  m_oGpioInitStruct.Alternate = 0;
  m_oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  m_oGpioInitStruct.Pin   = m_uiPinNr;
  m_oGpioInitStruct.Pull  = GPIO_NOPULL;
  m_oGpioInitStruct.Speed = GPIO_SPEED_HIGH;
  reconfigure();
}

STM32F407SystemGpioPin::~STM32F407SystemGpioPin()
{
}

bool STM32F407SystemGpioPin::setDirection(EDirection eDirection, size_t uiValue)
{
  switch(eDirection)
  {
    case EDirection::Input:
      m_oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
      break;
    case EDirection::Output:
      m_oGpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
      break;
    case EDirection::Alternate:
      m_oGpioInitStruct.Mode      = GPIO_MODE_AF_PP;
      m_oGpioInitStruct.Alternate = uiValue;
      break;
    case EDirection::Analog:
      m_oGpioInitStruct.Mode  = GPIO_MODE_ANALOG;
      break;
    default:
      m_oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  }
  reconfigure();

  return false;
}

IGpioPin::EDirection STM32F407SystemGpioPin::getDirection()
{
  EDirection eDirection = EDirection::Unknown;
  switch(m_oGpioInitStruct.Mode)
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

void STM32F407SystemGpioPin::setValue(bool bValue)
{
  HAL_GPIO_WritePin(m_pPort, m_uiPinNr, bValue?GPIO_PIN_SET:GPIO_PIN_RESET);
}

bool STM32F407SystemGpioPin::getValue()
{
  return HAL_GPIO_ReadPin(m_pPort, m_uiPinNr) == GPIO_PIN_SET;
}

bool STM32F407SystemGpioPin::toggle()
{
  HAL_GPIO_TogglePin(m_pPort, m_uiPinNr);
  return true;
}

bool STM32F407SystemGpioPin::setSpeedValue(size_t uiValue)
 {
  m_oGpioInitStruct.Speed  = uiValue;
  reconfigure();
  return true;
 }

void STM32F407SystemGpioPin::reconfigure()
{
  HAL_GPIO_Init(m_pPort, &m_oGpioInitStruct);
}
