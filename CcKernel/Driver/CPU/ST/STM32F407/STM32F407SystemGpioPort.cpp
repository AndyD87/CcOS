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
 * @brief     Implementation of class STM32F407SystemGpioPort
 **/
#include <STM32F407SystemGpioPort.h>
#include "CcKernel.h"
#include <STM32F407Driver.h>
#include "STM32F407SystemGpioPin.h"

STM32F407SystemGpioPort::STM32F407SystemGpioPort(uint8 uiPort)
{
  m_pPort = nullptr;
  switch(uiPort)
  {
    case 0:
      m_pPort = GPIOA;
      __HAL_RCC_GPIOA_CLK_ENABLE();
      break;
    case 1:
      m_pPort = GPIOB;
      __HAL_RCC_GPIOB_CLK_ENABLE();
      break;
    case 2:
      m_pPort = GPIOC;
      __HAL_RCC_GPIOC_CLK_ENABLE();
      break;
    case 3:
      m_pPort = GPIOD;
      __HAL_RCC_GPIOD_CLK_ENABLE();
      break;
    case 4:
      m_pPort = GPIOE;
      __HAL_RCC_GPIOE_CLK_ENABLE();
      break;
    case 5:
      m_pPort = GPIOF;
      __HAL_RCC_GPIOF_CLK_ENABLE();
      break;
    case 6:
      m_pPort = GPIOG;
      __HAL_RCC_GPIOG_CLK_ENABLE();
      break;
    case 7:
      m_pPort = GPIOH;
      __HAL_RCC_GPIOH_CLK_ENABLE();
      break;
    case 8:
      m_pPort = GPIOI;
      __HAL_RCC_GPIOI_CLK_ENABLE();
      break;
  }
}

STM32F407SystemGpioPort::~STM32F407SystemGpioPort()
{
  for(uint8 uiI = 0; uiI < count(); uiI++)
  {
    if(m_aPins[uiI] != nullptr)
    {
      delete m_aPins[uiI];
      m_aPins[uiI] = nullptr;
    }
  }
}

CcStatus STM32F407SystemGpioPort::onState(EState eState)
{
  CcStatus oStatus = false;
  switch(eState)
  {
    case EState::Start:
      oStatus = true;
      break;
    case EState::Pause:
      oStatus = true;
      break;
    case EState::Stop:
      oStatus = true;
      break;
    default:
      break;
  }
  return oStatus;
}

IGpioPin* STM32F407SystemGpioPort::getPin(uint8 uiNr)
{
  if(m_aPins[uiNr] == nullptr)
  {
    m_aPins[uiNr] = new STM32F407SystemGpioPin(m_pPort, uiNr);
  }
  return m_aPins[uiNr];
}

bool STM32F407SystemGpioPort::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
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

bool STM32F407SystemGpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
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

IGpioPin::EDirection STM32F407SystemGpioPort::getDirection(size_t uiPin)
{
  IGpioPin::EDirection eDirection = IGpioPin::EDirection::Unknown;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    eDirection = pPin->getDirection();
  }
  return eDirection;
}

bool STM32F407SystemGpioPort::setValue(size_t uiPin, bool bValue)
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

bool STM32F407SystemGpioPort::getValue(size_t uiPin)
{
  bool bRet = false;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = pPin->getValue();
  }
  return bRet;
}

bool STM32F407SystemGpioPort::setSpeedValue(size_t uiPin, size_t uiValue)
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
