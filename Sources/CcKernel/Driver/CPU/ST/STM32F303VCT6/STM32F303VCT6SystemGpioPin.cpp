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
 * @brief     Implementation of class STM32F303VCT6SystemGpioPin
 **/
#include <STM32F303VCT6SystemGpioPin.h>
#include "CcKernel.h"
#include <stm32f3xx_hal.h>
#include <STM32F303VCT6Driver.h>

class STM32F303VCT6SystemGpioPinPrivate
{
public:
  STM32F303VCT6SystemGpioPinPrivate(GPIO_TypeDef* pPort, uint32 uiPinNr) :
    pPort(pPort),
    uiPinNr(uiPinNr)
    {}
  GPIO_TypeDef* pPort;
  uint32 uiPinNr;
  GPIO_InitTypeDef oGpioInitStruct;
};

STM32F303VCT6SystemGpioPin::STM32F303VCT6SystemGpioPin(void* pPort, uint8 uiPinNr) :
    m_pPrivate(new STM32F303VCT6SystemGpioPinPrivate(
        (static_cast<GPIO_TypeDef*>(pPort)),
        (static_cast<uint32>(1) << uiPinNr)))
{
  CcStatic_memsetZeroObject(m_pPrivate->oGpioInitStruct);
  m_pPrivate->oGpioInitStruct.Alternate = 0;
  m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  m_pPrivate->oGpioInitStruct.Pin   = m_pPrivate->uiPinNr;
  m_pPrivate->oGpioInitStruct.Pull  = GPIO_NOPULL;
  m_pPrivate->oGpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  reconfigure();
}

STM32F303VCT6SystemGpioPin::~STM32F303VCT6SystemGpioPin()
{
  CCDELETE(m_pPrivate);
}

bool STM32F303VCT6SystemGpioPin::setDirection( EDirection eDirection, size_t uiValue)
{
  CCUNUSED(uiValue);
  switch(eDirection)
  {
    case EDirection::Input:
      m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
      break;
    case EDirection::Output:
      m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
      break;
    case EDirection::Alternate:
      m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_AF_PP;
      m_pPrivate->oGpioInitStruct.Alternate  = uiValue;
      break;
    case EDirection::Analog:
      m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_ANALOG;
      break;
    default:
      m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  }
  return false;
}

IGpioPin::EDirection STM32F303VCT6SystemGpioPin::getDirection()
{
  EDirection eDirection = EDirection::Unknown;
  switch(m_pPrivate->oGpioInitStruct.Mode)
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

void STM32F303VCT6SystemGpioPin::setValue(bool bValue)
{
  HAL_GPIO_WritePin(m_pPrivate->pPort, m_pPrivate->uiPinNr, bValue?GPIO_PIN_SET:GPIO_PIN_RESET);
}

bool STM32F303VCT6SystemGpioPin::getValue()
{
  return HAL_GPIO_ReadPin(m_pPrivate->pPort, m_pPrivate->uiPinNr) == GPIO_PIN_SET;
}

bool STM32F303VCT6SystemGpioPin::toggle()
{
  HAL_GPIO_TogglePin(m_pPrivate->pPort, m_pPrivate->uiPinNr);
  return true;
}

bool STM32F303VCT6SystemGpioPin::setSpeedValue(size_t uiValue)
{
  m_pPrivate->oGpioInitStruct.Speed  = uiValue;
  reconfigure();
  return true;
}

void STM32F303VCT6SystemGpioPin::reconfigure()
{
  HAL_GPIO_Init(m_pPrivate->pPort, &m_pPrivate->oGpioInitStruct);
}
