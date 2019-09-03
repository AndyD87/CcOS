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
 * @brief     Class IDevice
 */

#include "IDevice.h"

uint32 CcDeviceHandle::s_uiId = 0;

const CcString CcDeviceHandle::sAll       ("All");
const CcString CcDeviceHandle::sCpu       ("Cpu");
const CcString CcDeviceHandle::sUart      ("Uart");
const CcString CcDeviceHandle::sSpi       ("Spi");
const CcString CcDeviceHandle::sI2C       ("I2C");
const CcString CcDeviceHandle::sDisplay   ("Display");
const CcString CcDeviceHandle::sTouchPanel("TouchPanel");
const CcString CcDeviceHandle::sNetwork   ("Network");
const CcString CcDeviceHandle::sTimer     ("Timer");
const CcString CcDeviceHandle::sCamera    ("Camera");
const CcString CcDeviceHandle::sLed       ("Led");
const CcString CcDeviceHandle::sHdd       ("Hdd");
const CcString CcDeviceHandle::sGpioPort  ("GpioPort");
const CcString CcDeviceHandle::sGpioPin   ("GpioPin");
const CcString CcDeviceHandle::sWlan            ("Wlan");
const CcString CcDeviceHandle::sWlanClient      ("WlanClient");
const CcString CcDeviceHandle::sWlanAccessPoint ("WlanAccessPoint");

const CcString& CcDeviceHandle::getTypeString(EDeviceType eType)
{
  switch (eType)
  {
    case EDeviceType::All:
      return sAll;
    case EDeviceType::Cpu:
      return sCpu;
    case EDeviceType::Uart:
      return sUart;
    case EDeviceType::Spi:
      return sSpi;
    case EDeviceType::I2C:
      return sI2C;
    case EDeviceType::Display:
      return sDisplay;
    case EDeviceType::TouchPanel:
      return sTouchPanel;
    case EDeviceType::Network:
      return sNetwork;
    case EDeviceType::Timer:
      return sTimer;
    case EDeviceType::Camera:
      return sCamera;
    case EDeviceType::Led:
      return sLed;
    case EDeviceType::Hdd:
      return sHdd;
    case EDeviceType::GpioPort:
      return sGpioPort;
    case EDeviceType::GpioPin:
      return sGpioPin;
    case EDeviceType::Wlan:
      return sWlan;
    case EDeviceType::WlanAccessPoint:
      return sWlanAccessPoint;
    case EDeviceType::WlanClient:
      return sWlanClient;
  }
  return sAll;
}

EDeviceType CcDeviceHandle::getTypeFromString(const CcString& sType, bool* bOk)
{
  EDeviceType eType = EDeviceType::All;
  if (bOk != nullptr) *bOk = true;
  if      (sType == sAll       )  eType = EDeviceType::All;
  else if (sType == sCpu       )  eType = EDeviceType::Cpu;
  else if (sType == sUart      )  eType = EDeviceType::Uart;
  else if (sType == sSpi       )  eType = EDeviceType::Spi;
  else if (sType == sI2C       )  eType = EDeviceType::I2C;
  else if (sType == sDisplay   )  eType = EDeviceType::Display;
  else if (sType == sTouchPanel)  eType = EDeviceType::TouchPanel;
  else if (sType == sNetwork   )  eType = EDeviceType::Network;
  else if (sType == sTimer     )  eType = EDeviceType::Timer;
  else if (sType == sCamera    )  eType = EDeviceType::Camera;
  else if (sType == sLed       )  eType = EDeviceType::Led;
  else if (sType == sHdd       )  eType = EDeviceType::Hdd;
  else if (sType == sGpioPort  )  eType = EDeviceType::GpioPort;
  else if (sType == sGpioPin   )  eType = EDeviceType::GpioPin;
  else if (sType == sWlan      )  eType = EDeviceType::Wlan;
  else if (sType == sWlanAccessPoint) eType = EDeviceType::WlanAccessPoint;
  else if (sType == sWlanClient)      eType = EDeviceType::WlanClient;
  else if (bOk != nullptr) *bOk = false;
  return eType;
}

CcStatus IDevice::setState(EState eState)
{
  CcStatus oStatus;
  switch(eState)
  {
    case EState::Start:
      oStatus = setState(EState::Starting);
      break;
    case EState::Starting:
      oStatus = setState(EState::Run);
      break;
    case EState::Run:
      oStatus = setState(EState::Running);
      break;
    case EState::Pause:
      oStatus = setState(EState::Paused);
      break;
    case EState::Stop:
      oStatus = setState(EState::Stopping);
      break;
    case EState::Stopping:
      oStatus = setState(EState::Stopped);
      break;
    case EState::Running:
      CCFALLTHROUGH;
    case EState::Paused:
      CCFALLTHROUGH;
    case EState::Stopped:
      CCFALLTHROUGH;
      // Do nothing on final states
      break;
    default:
      break;
  }
  return oStatus;
}
