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
#include "IShell.h"

GenericApp::GenericApp()
{
}

GenericApp::~GenericApp()
{
  CCDELETE(m_pCdcDevice);
}

void GenericApp::run()
{
  CcStatus oStatus(false);
  CcDeviceUsb oUsb = CcKernel::getDevice(EDeviceType::Usb);
  if(oUsb.isValid())
  {
    CcDeviceUsbDevice oUsbDevice = oUsb->createDevice();
    if(oUsbDevice.isValid())
    {
      IUsbDevice::CDeviceDescriptor& oDeviceDescriptor = oUsbDevice.getDevice()->getDeviceDescriptor();
      oDeviceDescriptor.uiDeviceClass           = 0xef;
      oDeviceDescriptor.uiDeviceSubClass        = 0x02;
      oDeviceDescriptor.uiDeviceProtocol        = 0x01;
      oDeviceDescriptor.uiBcd                   = 0x0200;
      oDeviceDescriptor.uiMaxPacketSize         = 64;
      oDeviceDescriptor.uiVendorId  	          = 0x1234;
      oDeviceDescriptor.uiProductId             = 0x1234;
      oDeviceDescriptor.uiBcdDevice             = 0x0200;
      oDeviceDescriptor.uiManufacturerStringIdx = 1;
      oDeviceDescriptor.uiProductStringIdx      = 2;
      oDeviceDescriptor.uiSerialNumberStringIdx = 3;
      IUsbDevice::SStringDescriptor& pString = oDeviceDescriptor.createString("123");
      pString.uiLength = sizeof(IUsbDevice::SStringDescriptor);
      pString.pString[0] = 0x0409;
      
      oDeviceDescriptor.createString("CcOS Manu");
      oDeviceDescriptor.createString("CcOS Prod");
      oDeviceDescriptor.createString("CcOS Seri");
      
      IUsbDevice::CConfigDescriptor& oConfig = oDeviceDescriptor.createConfig();
      oConfig.getConfig()->uiConfigurationValue = 1;
      oConfig.getConfig()->uiConfiguration = 0;
      oConfig.getConfig()->uiAttributes = 0xC0;
      oConfig.getConfig()->uiMaxPower   = 0x32;

      CCNEW(m_pCdcDevice, CcUsbCdc, oUsbDevice);
      CCNEW(m_pBulkTransfer, CcUsbBulkTransfer, oUsbDevice);   
      
      oStatus = m_pCdcDevice->open(EOpenFlags::ReadWrite);

      oStatus = m_pBulkTransfer->open(EOpenFlags::ReadWrite);

      if(oStatus)
      {
        oStatus = oUsbDevice->start();
        if(oStatus)
        {
          CCNEW(m_pShell, IShell);   
          m_pShell->init(m_pCdcDevice);
          m_pShell->initDefaultCommands();
          oStatus = m_pShell->start();
          if(oStatus)
          {
          }
        }
      }
    }

    setExitCode(oStatus);
  }
}
