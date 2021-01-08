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
 * @brief     Implementation of class STM32F103SystemGpioPort
 **/
#include <STM32F103SystemGpioPort.h>
#include "CcKernel.h"
#include <stm32f1xx_hal.h>
#include <STM32F103Driver.h>
#include "STM32F103SystemGpioPin.h"

STM32F103SystemGpioPort::STM32F103SystemGpioPort(uint8 uiPort)
{
  pPort = nullptr;
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
  }
}

STM32F103SystemGpioPort::~STM32F103SystemGpioPort()
{
  for(uint8 uiI = 0; uiI < count(); uiI++)
  {
    if(aPins[uiI] != nullptr)
    {
      delete aPins[uiI];
      aPins[uiI] = nullptr;
    }
  }
}

IGpioPin* STM32F103SystemGpioPort::getPin(uint8 uiNr)
{
  if(aPins[uiNr] == nullptr)
  {
    aPins[uiNr] = new STM32F103SystemGpioPin(pPort, uiNr);
  }
  return aPins[uiNr];
}

bool STM32F103SystemGpioPort::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
{
  CCUNUSED(uiValue);
  bool bSuccess = true;
  for(int i = 0; i < count(); i++)
  {
    if((1 << i) & uiPinMask)
    {
      IGpioPin* pPin = getPin(i);
      if(pPin)
      {
        pPin->setDirection(eDirection, uiValue);
      }
    }
  }
  return bSuccess;
}

bool STM32F103SystemGpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
{
  bool bRet = false;
  CCUNUSED(uiValue);
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = true;
    pPin->setDirection(eDirection);
  }
  return bRet;
}

IGpioPin::EDirection STM32F103SystemGpioPort::getDirection(size_t uiPin)
{
  IGpioPin::EDirection eDirection = IGpioPin::EDirection::Unknown;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    eDirection = pPin->getDirection();
  }
  return eDirection;
}

bool STM32F103SystemGpioPort::setValue(size_t uiPin, bool bValue)
{
  bool bRet = false;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = true;
    pPin->setValue(bValue);
  }
  return bRet;
}

bool STM32F103SystemGpioPort::getValue(size_t uiPin)
{
  bool bRet = false;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = pPin->getValue();
  }
  return bRet;
}

bool STM32F103SystemGpioPort::setSpeedValue(size_t uiPin, size_t uiValue)
{
  bool bRet = false;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = true;
    pPin->setSpeedValue(uiValue);
  }
  return bRet;
}
