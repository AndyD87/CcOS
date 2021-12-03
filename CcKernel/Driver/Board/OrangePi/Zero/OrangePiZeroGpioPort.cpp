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
 * @brief     Implementation of class OrangePiZeroGpioPort
 **/
#include <cstddef>
#include <OrangePiZeroGpioPort.h>
#include "CcKernel.h"

#include "wiringPi.h"

OrangePiZeroGpioPort::OrangePiZeroGpioPort()
{
}

OrangePiZeroGpioPort::~OrangePiZeroGpioPort()
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

CcStatus OrangePiZeroGpioPort::onState(EState eState)
{
  CcStatus oStatus;
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

IGpioPin* OrangePiZeroGpioPort::getPin(uint8 uiNr)
{
  if(m_aPins[uiNr] == nullptr)
  {

  }
  return m_aPins[uiNr];
}

bool OrangePiZeroGpioPort::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
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

bool OrangePiZeroGpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
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

IGpioPin::EDirection OrangePiZeroGpioPort::getDirection(size_t uiPin)
{
  IGpioPin::EDirection eDirection = IGpioPin::EDirection::Unknown;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    eDirection = pPin->getDirection();
  }
  return eDirection;
}

bool OrangePiZeroGpioPort::setPinValue(size_t uiPin, bool bValue)
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

bool OrangePiZeroGpioPort::getPinValue(size_t uiPin)
{
  bool bRet = false;
  IGpioPin* pPin = getPin(uiPin);
  if(pPin)
  {
    bRet = pPin->getValue();
  }
  return bRet;
}

bool OrangePiZeroGpioPort::setSpeedValue(size_t uiPin, size_t uiValue)
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
