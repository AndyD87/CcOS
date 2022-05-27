
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

#include "STM32F407Usb.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "usbh_conf.h"
#include <stm32f4xx_hal_hcd.h>
#include <stm32f4xx_hal_pcd.h>

STM32F407Usb::STM32F407Usb()
{
}

STM32F407Usb::~STM32F407Usb()
{
}

CcStatus STM32F407Usb::onState(EState eState)
{
  CcStatus bSuccess = false;
  switch(eState)
  {
    case EState::Start:
      if(m_pType.pBasicDevice)
      {
        m_pType.pBasicDevice->start();
      }
      break;
    case EState::Stop:
      if(m_pType.pBasicDevice)
      {
        m_pType.pBasicDevice->stop();
      }
      break;
    default:
      bSuccess = EStatus::NotSupported;
  }
  return bSuccess;
}

IUsbDevice* STM32F407Usb::createDevice()
{
  if(m_pType.pBasicDevice)
  {
    return nullptr;
  }
  else
  {
    return m_pType.pDevice = new STM32F407UsbDevice();
  }
}