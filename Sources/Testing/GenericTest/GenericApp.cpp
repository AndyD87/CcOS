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
#include "Shell/CcGenericConsole.h"
#include "CcKernel.h"
#include "Devices/CcDeviceUsb.h"
#include "Devices/CcDeviceI2C.h"
#include "Devices/IUsbDevice.h"
#include "Driver/Display/SDD1306.h"
#include "CcSize.h"
#include "IShell.h"
#include "GenericTestFont.h"
#include "CcConsole.h"

GenericApp::GenericApp()
{
}

GenericApp::~GenericApp()
{
  CCDELETE(m_pCdcDevice);
  CCDELETE(m_pConsole);
  CCDELETE(m_pDisplay);
}

void GenericApp::run()
{
  CcStatus oStatus(false);
  CcDeviceI2C oI2C = CcKernel::getDevice(EDeviceType::I2C);
  if(oI2C.isValid())
  { 
    oI2C->start();
    II2CClient* pClient = oI2C.createInterface(0x20);
    if(pClient)
    {
      uint8 uiByte = 0;
      if(pClient->writeByte(uiByte)) 
      {
        CCDEBUG("Read succeeded" + CcString::fromNumber(uiByte));
      }
      else
      {
        CCDEBUG("Read failed");
      }
      II2CClient* pClient = oI2C.createInterface(0x3c);
      m_pDisplay = CCNEW_INLINE(SDD1306, CcSize(128,64), *pClient, SDD1306::ETransportType::eI2C);
      m_pDisplay->start();
      m_pConsole = CCNEW_INLINE(CcGenericConsole, m_pDisplay, GenericTestFont_Used);
      if(m_pConsole->open(EOpenFlags::ReadWrite))
      {
        m_pConsole->writeLine("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789");
        m_pConsole->writeLine("IP: 192.168.100.178");
        m_pConsole->writeLine("DHCP: static");
        CcConsole::setOutputDevice(m_pConsole);
        CcKernel::sleep(2000);
        CcConsole::writeLine("Console set");
        CcKernel::sleep(2000);
        CcConsole::write("Active line", sizeof("Active line")-1);
      }
    }
  }

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
      oDeviceDescriptor.uiVendorId  	          = 0xaad;
      oDeviceDescriptor.uiProductId             = 0x15e;
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

#ifndef BULK_TRANSFER
      CCNEW(m_pBulkTransfer, CcUsbBulkTransfer, oUsbDevice);   
      oStatus = m_pBulkTransfer->open(EOpenFlags::ReadWrite);
      if(oStatus)
      {
        oStatus = oUsbDevice->start();
        //uchar pBuffer[64];
        //uchar uCnt = 1;
        //while(1)
        //{
        //  for(size_t uiPos = 0; uiPos < 53; uiPos++)
        //  {
        //    pBuffer[uiPos] = uCnt++;
        //  }
        //  // Resend until all data is written
        //  while(53 < m_pBulkTransfer->write(pBuffer, 53))
        //  {}
        //}
      }
#else
      CCNEW(m_pCdcDevice, CcUsbCdc, oUsbDevice);
      
      oStatus = m_pCdcDevice->open(EOpenFlags::ReadWrite);

      if(oStatus)
      {
        oStatus = oUsbDevice->start();
        if(oStatus)
        {
          CCNEW(m_pShell, IShell);   
          m_pShell->init(m_pCdcDevice);
          m_pShell->initDefaultCommands();
          m_pShell->registerOnStateChange(NewCcEvent(this, GenericApp::onShellStateChanged));
          oStatus = m_pShell->start();
          if(oStatus)
          {
          }
        }
      }
#endif
    }

    setExitCode(oStatus);
  }
}

void GenericApp::onShellStateChanged(void* pVoid)
{
  // Verify shell as sender and restart
  if( pVoid == m_pShell &&
      m_pShell->getThreadState() == EThreadState::Stopped)
  {
    m_pShell->start();
  }
}
