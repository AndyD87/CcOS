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
 * @brief     Implementation of class CcDevice
 */

#include "CcDevice.h"
#include "Devices/CcDeviceCamera.h"
#include "Devices/CcDeviceClock.h"
#include "Devices/CcDeviceCpu.h"
#include "Devices/CcDeviceDisplay.h"
#include "Devices/CcDeviceEeprom.h"
#include "Devices/CcDeviceGpioPin.h"
#include "Devices/CcDeviceGpioPort.h"
#include "Devices/CcDeviceGrayscaleMap.h"
#include "Devices/CcDeviceHdd.h"
#include "Devices/CcDeviceI2C.h"
#include "Devices/CcDeviceIoDevice.h"
#include "Devices/CcDeviceLed.h"
#include "Devices/CcDeviceNetwork.h"
#include "Devices/CcDevicePixMap.h"
#include "Devices/CcDeviceSdCard.h"
#include "Devices/CcDeviceSpi.h"
#include "Devices/CcDeviceTimer.h"
#include "Devices/CcDeviceTouch.h"
#include "Devices/CcDeviceUart.h"
#include "Devices/CcDeviceUsb.h"
#include "Devices/CcDeviceUsbHid.h"
#include "Devices/CcDeviceWlan.h"
#include "Devices/CcDeviceWlanAccessPoint.h"
#include "Devices/CcDeviceWlanClient.h"


uint32 CcDevice::s_uiId = 0;
CcDevice CcDevice::NullDevice(nullptr);

CcConstStringClass_C(sAll       ,"All", CcDevice);
CcConstStringClass_C(sCpu       ,"Cpu", CcDevice);
CcConstStringClass_C(sUart      ,"Uart", CcDevice);
CcConstStringClass_C(sSpi       ,"Spi", CcDevice);
CcConstStringClass_C(sI2C       ,"I2C", CcDevice);
CcConstStringClass_C(sDisplay   ,"Display", CcDevice);
CcConstStringClass_C(sTouchPanel,"TouchPanel", CcDevice);
CcConstStringClass_C(sNetwork   ,"Network", CcDevice);
CcConstStringClass_C(sTimer     ,"Timer", CcDevice);
CcConstStringClass_C(sCamera    ,"Camera", CcDevice);
CcConstStringClass_C(sLed       ,"Led", CcDevice);
CcConstStringClass_C(sHdd       ,"Hdd", CcDevice);
CcConstStringClass_C(sGpioPort  ,"GpioPort", CcDevice);
CcConstStringClass_C(sGpioPin   ,"GpioPin", CcDevice);
CcConstStringClass_C(sWlan            ,"Wlan", CcDevice);
CcConstStringClass_C(sWlanClient      ,"WlanClient", CcDevice);
CcConstStringClass_C(sWlanAccessPoint ,"WlanAccessPoint", CcDevice);
CcConstStringClass_C(sEeprom    ,"Eeprom", CcDevice);
CcConstStringClass_C(sClock     ,"Clock", CcDevice);
CcConstStringClass_C(sUsb       ,"Usb", CcDevice);

const CcString& CcDevice::getTypeString(EDeviceType eType)
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
    case EDeviceType::Eeprom:
      return sEeprom;
    case EDeviceType::Clock:
      return sClock;
    case EDeviceType::Usb:
      return sUsb;
  }
  return sAll;
}

EDeviceType CcDevice::getTypeFromString(const CcString& sType, bool* bOk)
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
  else if (sType == sWlanClient)  eType = EDeviceType::WlanClient;
  else if (sType == sEeprom    )  eType = EDeviceType::Eeprom;
  else if (sType == sClock     )  eType = EDeviceType::Clock;
  else if (sType == sUsb       )  eType = EDeviceType::Usb;
  else if (bOk != nullptr) *bOk = false;
  return eType;
}
