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
    IUsbDevice::CDeviceDescriptor oDeviceDescriptor;
    oDeviceDescriptor.uiBcd                   = 0x0200;
    oDeviceDescriptor.uiDeviceClass           = 2;
    oDeviceDescriptor.uiDeviceSubClass        = 2;
    oDeviceDescriptor.uiDeviceProtocol        = 0;
    oDeviceDescriptor.uiMaxPacketSize         = 64;
    oDeviceDescriptor.uiVendorId  	          = 0x1234;
    oDeviceDescriptor.uiProductId             = 0x1234;
    oDeviceDescriptor.uiBcdDevice             = 0x0200;
    oDeviceDescriptor.uiManufacturerStringIdx = 1;
    oDeviceDescriptor.uiProductStringIdx      = 2;
    oDeviceDescriptor.uiSerialNumberStringIdx = 3;
    oDeviceDescriptor.uiNumConfigurations     = 1;

    IUsbDevice::CConfigDescriptor& oConfig = oDeviceDescriptor.createConfig();
    oConfig.getConfig()->uiNumInterfaces = 2;
    oConfig.getConfig()->uiConfigurationValue = 1;
    oConfig.getConfig()->uiConfiguration = 0;
    oConfig.getConfig()->uiAttributes = 0xC0;
    oConfig.getConfig()->uiMaxPower   = 0x32; 

    IUsbDevice::SInterfaceDescriptor* oInterface0 = oConfig.createInterface();
    oInterface0->uiInterfaceNumber = 0;
    oInterface0->uiAlternateSetting = 0;
    oInterface0->uiNumEndpoints = 1;
    oInterface0->uiInterfaceClass = 0x02;
    oInterface0->uiInterfaceSubClass = 0x02;
    oInterface0->uiInterfaceProtocol = 0x01;
    oInterface0->uiInterfaceIdx = 0;

    IUsbDevice::SFunctionalDescriptor* pFuntional0 = oConfig.createFunctional(5);
    pFuntional0->Data.Raw.uiRaw[1]= 0x00;
    pFuntional0->Data.Raw.uiRaw[2]= 0x10;
    pFuntional0->Data.Raw.uiRaw[3]= 0x01;
    
    IUsbDevice::SFunctionalDescriptor* pFuntional1 = oConfig.createFunctional(5);
    pFuntional1->Data.Raw.uiRaw[1]= 0x01;
    pFuntional1->Data.Raw.uiRaw[2]= 0x00;
    pFuntional1->Data.Raw.uiRaw[3]= 0x01;
    
    IUsbDevice::SFunctionalDescriptor* pFuntional2 = oConfig.createFunctional(4);
    pFuntional2->Data.Raw.uiRaw[1]= 0x02;
    pFuntional2->Data.Raw.uiRaw[2]= 0x02;

    IUsbDevice::SFunctionalDescriptor* pFuntional3 = oConfig.createFunctional(5);
    pFuntional3->Data.Raw.uiRaw[1]= 0x06;
    pFuntional3->Data.Raw.uiRaw[2]= 0x00;
    pFuntional3->Data.Raw.uiRaw[3]= 0x01;

    IUsbDevice::SEndpointDescriptor* pEp82 = oConfig.createEndpoint();
    pEp82->uiEndpointAddress = 0x82;
    pEp82->uiAttributes = 0x03;
    pEp82->wMaxPacketSize = 0x08;
    pEp82->uInterval = 0x10;

    IUsbDevice::SInterfaceDescriptor* pInterface1 = oConfig.createInterface();
    pInterface1->uiInterfaceNumber = 1;
    pInterface1->uiAlternateSetting = 0;
    pInterface1->uiNumEndpoints = 2;
    pInterface1->uiInterfaceClass = 0x0A;
    pInterface1->uiInterfaceSubClass = 0x00;
    pInterface1->uiInterfaceProtocol = 0x00;
    pInterface1->uiInterfaceIdx = 0;

    IUsbDevice::SEndpointDescriptor* pEp01 = oConfig.createEndpoint();
    pEp01->uiEndpointAddress = 0x01;
    pEp01->uiAttributes      = 0x02;
    pEp01->wMaxPacketSize    = 512;
    pEp01->uInterval         = 0x00;

    IUsbDevice::SEndpointDescriptor* pEp81 = oConfig.createEndpoint();
    pEp81->uiEndpointAddress = 0x81;
    pEp81->uiAttributes      = 0x02;
    pEp81->wMaxPacketSize    = 512;
    pEp81->uInterval         = 0x00;

    oDeviceDescriptor.createString("GER");
    oDeviceDescriptor.createString("CcOS Manu");
    oDeviceDescriptor.createString("CcOS Prod");
    oDeviceDescriptor.createString("CcOS Seri");
    oDeviceDescriptor.createString("CcOS Numm");

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
