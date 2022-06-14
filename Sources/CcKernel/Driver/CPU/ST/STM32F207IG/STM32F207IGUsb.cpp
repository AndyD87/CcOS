
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

#include "STM32F207IGUsb.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "usbh_conf.h"
#include <stm32f2xx_hal_hcd.h>
#include <stm32f2xx_hal_pcd.h>

STM32F207IGUsb::STM32F207IGUsb()
{
}

STM32F207IGUsb::~STM32F207IGUsb()
{
}

CcStatus STM32F207IGUsb::onState(EState eState)
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
      break;
  }
  return bSuccess;
}

IUsbDevice* STM32F207IGUsb::createDevice(const IUsbDevice::CDeviceDescriptor& oConfig)
{
  if(m_pType.pBasicDevice)
  {
    return nullptr;
  }
  else
  {
    CCNEW(m_pType.pDevice, STM32F207IGUsbDevice);
    if(m_pType.pDevice->loadDeviceDescriptor(oConfig))
    {
      return m_pType.pDevice;
    }
    else
    {
      CCDELETE(m_pType.pDevice);
      return nullptr;
    }
  }
}