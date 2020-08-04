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
 * @brief     Implementation of class STM32F303VCT6SystemGpioPort
 **/
#include <STM32F303VCT6SystemGpioPort.h>
#include "CcKernel.h"
#include <STM32F303VCT6Driver.h>
#include "STM32F303VCT6SystemGpioPin.h"

STM32F303VCT6SystemGpioPort::STM32F303VCT6SystemGpioPort(uint8 uiPort)
{
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
}

STM32F303VCT6SystemGpioPort::~STM32F303VCT6SystemGpioPort()
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

IGpioPin* STM32F303VCT6SystemGpioPort::getPin(uint8 uiNr)
{
  if(aPins[uiNr] == nullptr)
  {
    aPins[uiNr] = new STM32F303VCT6SystemGpioPin(pPort, uiNr);
  }
  return aPins[uiNr];
}


bool STM32F303VCT6SystemGpioPort::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
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

bool STM32F303VCT6SystemGpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
{
  bool bRet = false;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = true;
    pPin->setDirection(eDirection);
  }
  return bRet;
}

IGpioPin::EDirection STM32F303VCT6SystemGpioPort::getDirection(size_t uiPin)
{
  IGpioPin::EDirection eDirection = IGpioPin::EDirection::Unknown;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    eDirection = pPin->getDirection();
  }
  return eDirection;
}

bool STM32F303VCT6SystemGpioPort::setValue(size_t uiPin, bool bValue)
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

bool STM32F303VCT6SystemGpioPort::getValue(size_t uiPin)
{
  bool bRet = false;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = pPin->getValue();
  }
  return bRet;
}

bool STM32F303VCT6SystemGpioPort::setSpeedValue(size_t uiPin, size_t uiValue)
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
