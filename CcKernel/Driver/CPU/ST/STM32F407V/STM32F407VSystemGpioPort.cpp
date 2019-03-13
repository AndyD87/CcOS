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
 * @brief     Implementation of class STM32F407VSystemGpioPort
 **/
#include <STM32F407VSystemGpioPort.h>
#include "CcKernel.h"
#include <stm32f4xx_hal.h>
#include <STM32F407VDriver.h>
#include "STM32F407VSystemGpioPin.h"

class STM32F407VSystemGpioPortPrivate
{
public:
  STM32F407VSystemGpioPortPrivate(GPIO_TypeDef* pPort) :
    pPort(pPort)
    {}
  GPIO_TypeDef* pPort;
  IGpioPin* aPins[NUMBER_OF_PINS] = {nullptr};
};

STM32F407VSystemGpioPort::STM32F407VSystemGpioPort(uint8 uiPort)
{
  GPIO_TypeDef* pPort = nullptr;
  switch(uiPort)
  {
    case 0:
      pPort = GPIOA;
      __HAL_RCC_GPIOA_CLK_ENABLE();
      break;
    case 1:
      pPort = GPIOB;
      __HAL_RCC_GPIOB_CLK_ENABLE();
      break;
    case 2:
      pPort = GPIOC;
      __HAL_RCC_GPIOC_CLK_ENABLE();
      break;
    case 3:
      pPort = GPIOD;
      __HAL_RCC_GPIOD_CLK_ENABLE();
      break;
    case 4:
      pPort = GPIOE;
      __HAL_RCC_GPIOE_CLK_ENABLE();
      break;
    case 5:
      pPort = GPIOF;
      __HAL_RCC_GPIOF_CLK_ENABLE();
      break;
  }
  m_pPrivate = new STM32F407VSystemGpioPortPrivate(pPort);
}

STM32F407VSystemGpioPort::~STM32F407VSystemGpioPort()
{
  CCDELETE(m_pPrivate);
}

IGpioPin* STM32F407VSystemGpioPort::getPin(uint8 uiNr)
{
  if(m_pPrivate->aPins[uiNr] == nullptr)
  {
    m_pPrivate->aPins[uiNr] = new STM32F407VSystemGpioPin(m_pPrivate->pPort, uiNr);
  }
  return m_pPrivate->aPins[uiNr];
}
