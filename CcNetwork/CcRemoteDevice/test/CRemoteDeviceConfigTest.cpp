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
 * @brief     Implemtation of class CRemoteDeviceConfigTest
 */
#include "CRemoteDeviceConfigTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcString.h"
#include "Server/CConfig.h"
#include "Devices/Simulations/CcEepromSimulation.h"

CRemoteDeviceConfigTest::CRemoteDeviceConfigTest() :
  CcTest("CRemoteDeviceConfigTest")
{
  appendTestMethod("configuration from EEPROM", &CRemoteDeviceConfigTest::testEeprom);
}

CRemoteDeviceConfigTest::~CRemoteDeviceConfigTest()
{
}

bool CRemoteDeviceConfigTest::testEeprom()
{
  bool bSuccess = false;
  CcEepromSimulation oSimulation(1024);
  oSimulation.start();
  CcKernel::addDevice(CcDeviceHandle(&oSimulation, EDeviceType::Eeprom));

  NRemoteDevice::Server::CConfig oConfig(true);
  if(!oConfig.isRead())
  {
    // Reset eeprom to beginning;
    NRemoteDevice::Server::CConfig oConfig2(true);
    bSuccess = oConfig2.isRead();
  }

  CcKernel::removeDevice(CcDeviceHandle(&oSimulation, EDeviceType::Eeprom));
  return bSuccess;
}
