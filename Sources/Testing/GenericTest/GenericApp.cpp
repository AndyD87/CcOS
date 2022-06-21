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
#include "CcConsole.h"

GenericApp::GenericApp()
{
}

GenericApp::~GenericApp()
{
  CCDELETE(m_pCdcDevice);
}

void GenericApp::run()
{
  CcDeviceUsb oUsbDevice = CcKernel::getDevice(EDeviceType::Usb);
  if(oUsbDevice.isValid())
  {            
    CCNEW(m_pCdcDevice, CcUsbCdc, oUsbDevice);   
    CcStatus oStatus = m_pCdcDevice->start();

    CcConsole::setInputDevice(m_pCdcDevice);
    CcConsole::setOutputDevice(m_pCdcDevice);

    while(1)
    {
      CcKernel::sleep(1000);
      CcConsole::writeString("Test\r\n");
    }

    setExitCode(oStatus);
  }
}
