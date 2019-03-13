/**
 * @author     Andreas Dirmeier
 * @copyright  Andreas Dirmeier (c) 2015
 * @par       Language: C++11
 */
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class STM32F407VSystemGpioPin
 **/
#include <STM32F407VSystemGpioPin.h>
#include "CcKernel.h"
#include <stm32f4xx_hal.h>
#include <STM32F407VDriver.h>

class STM32F407VSystemGpioPinPrivate
{
public:
  STM32F407VSystemGpioPinPrivate(GPIO_TypeDef* pPort, uint32 uiPinNr) :
    pPort(pPort),
    uiPinNr(uiPinNr)
    {}
  GPIO_TypeDef* pPort;
  uint32 uiPinNr;
  GPIO_InitTypeDef oGpioInitStruct;
};

STM32F407VSystemGpioPin::STM32F407VSystemGpioPin(void* pPort, uint8 uiPinNr) :
    m_pPrivate(new STM32F407VSystemGpioPinPrivate(
        (static_cast<GPIO_TypeDef*>(pPort)),
        (static_cast<uint32>(1) << uiPinNr)))
{
  CcStatic_memsetZeroObject(m_pPrivate->oGpioInitStruct);
  m_pPrivate->oGpioInitStruct.Alternate = false;
  m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  m_pPrivate->oGpioInitStruct.Pin   = m_pPrivate->uiPinNr;
  m_pPrivate->oGpioInitStruct.Pull  = GPIO_NOPULL;
  m_pPrivate->oGpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  reconfigure();
}

STM32F407VSystemGpioPin::~STM32F407VSystemGpioPin()
{
  CCDELETE(m_pPrivate);
}

bool STM32F407VSystemGpioPin::setDirection( EDirection eDirection)
{
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
      break;
    case EDirection::Analog:
      m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_ANALOG;
      break;
    default:
      m_pPrivate->oGpioInitStruct.Mode  = GPIO_MODE_INPUT;
  }
  reconfigure();
  return false;
}

IGpioPin::EDirection STM32F407VSystemGpioPin::getDirection()
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

void STM32F407VSystemGpioPin::setValue(bool bValue)
{
  HAL_GPIO_WritePin(m_pPrivate->pPort, m_pPrivate->uiPinNr, bValue?GPIO_PIN_SET:GPIO_PIN_RESET);
}

bool STM32F407VSystemGpioPin::getValue()
{
  return HAL_GPIO_ReadPin(m_pPrivate->pPort, m_pPrivate->uiPinNr) == GPIO_PIN_SET;
}

bool STM32F407VSystemGpioPin::toggle()
{
  HAL_GPIO_TogglePin(m_pPrivate->pPort, m_pPrivate->uiPinNr);
  return true;
}

void STM32F407VSystemGpioPin::reconfigure()
{
  HAL_GPIO_Init(GPIOD, &m_pPrivate->oGpioInitStruct);
}
