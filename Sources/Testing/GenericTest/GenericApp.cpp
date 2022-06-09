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
 * @brief     Class GenericApp
 *
 *  Implementation of Main Application
 */

#include "GenericApp.h"
#include "CcKernel.h"
#include "Devices/CcDeviceUsb.h"
#include "Devices/IUsbDevice.h"

GenericApp::GenericApp()
{
}

GenericApp::~GenericApp()
{
}

void GenericApp::run()
{
  CcDeviceUsb oUsbDevice = CcKernel::getDevice(EDeviceType::Usb);
  if(oUsbDevice.isValid())
  {
    IUsbDevice::CDeviceDescriptor oDeviceDescriptor(0x1234, 0x1234);
    oDeviceDescriptor.uiBcd = 0x0200;
    oDeviceDescriptor.uiDeviceClass     = 0;
    oDeviceDescriptor.uiDeviceSubClass  = 0;
    oDeviceDescriptor.uiDeviceProtocol  = 0;
    oDeviceDescriptor.uiMaxPacketSize   = 64;
    oDeviceDescriptor.uiBcdDevice             = 0x0200;
    oDeviceDescriptor.uiManufacturerStringIdx = 0;
    oDeviceDescriptor.uiProductStringIdx      = 0;
    oDeviceDescriptor.uiSerialNumberStringIdx = 0;
    oDeviceDescriptor.uiNumConfigurations     = 0;

    IUsbDevice* pUsbDevice = oUsbDevice.getDevice()->createDevice(oDeviceDescriptor);
    if(pUsbDevice)
    {
      pUsbDevice->start();
    }
    else
    {
      setExitCode(EStatus::InvalidHandle);
    }                     
  }
}
