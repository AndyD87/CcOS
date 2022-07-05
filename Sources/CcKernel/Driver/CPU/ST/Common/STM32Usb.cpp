
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

#include "STM32Usb.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "usbh_conf.h"

STM32Usb::STM32Usb()
{
}

STM32Usb::~STM32Usb()
{
}

CcStatus STM32Usb::onState(EState eState)
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

IUsbDevice* STM32Usb::createDevice()
{
  if(m_pType.pBasicDevice)
  {
    return nullptr;
  }
  else
  {
    CCNEW(m_pType.pDevice, STM32UsbDevice);
    return m_pType.pDevice;
  }
}

void STM32Usb::removeDevice()
{
  CCDELETE(m_pType.pDevice);
}
