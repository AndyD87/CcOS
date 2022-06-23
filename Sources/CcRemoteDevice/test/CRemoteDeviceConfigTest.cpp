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
 * @brief     Implemtation of class CRemoteDeviceConfigTest
 */
#include "CRemoteDeviceConfigTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcString.h"
#include "Server/CConfig.h"
#include "Devices/Simulations/CcEepromSimulation.h"
#include "CcDevice.h"

CRemoteDeviceConfigTest::CRemoteDeviceConfigTest() :
  CcTest("CRemoteDeviceConfigTest")
{
  appendTestMethod("configuration from EEPROM", &CRemoteDeviceConfigTest::testEeprom);
  appendTestMethod("configuration of user settings", &CRemoteDeviceConfigTest::testUserSettings);
}

CRemoteDeviceConfigTest::~CRemoteDeviceConfigTest()
{
}

bool CRemoteDeviceConfigTest::testEeprom()
{
  bool bSuccess = false;
  CcEepromSimulation oSimulation(1024);
  oSimulation.start();
  CcKernel::addDevice(CcDevice(&oSimulation, EDeviceType::Eeprom));

  NRemoteDevice::Server::CConfig oConfig(true);
  if(!oConfig.isRead())
  {
    // Reset eeprom to beginning;
    NRemoteDevice::Server::CConfig oConfig2(true);
    bSuccess = oConfig2.isRead();
  }

  CcKernel::removeDevice(CcDevice(&oSimulation, EDeviceType::Eeprom));
  return bSuccess;
}

bool CRemoteDeviceConfigTest::testUserSettings()
{
  bool bSuccess = false;
  // Generate default config
  NRemoteDevice::Server::CConfig oConfig(true);
  oConfig.oUsers.oUsers.append(NRemoteDevice::Server::Config::CUsers::CCredentials("TestUser", "TestPassword"));
  oConfig.oUsers.oUsers.append(NRemoteDevice::Server::Config::CUsers::CCredentials("TestUser2", "TestPassword2"));
  oConfig.oUsers.sAdmin = "TestUser";

  CcByteArray oData;
  oConfig.writeData(NRemoteDevice::Server::CConfig::ESource::FileJson, oData);
  CcString sData = oData;
  CcString sDataToContain = "\"Users\":{\"Admin\":\"TestUser\",{\"Name\":\"TestUser\",\"Password\":\"TestPassword\"},{\"Name\":\"TestUser2\",\"Password\":\"TestPassword2\"}}";
  if (sData.contains(sDataToContain))
  {
    oConfig.writeData(NRemoteDevice::Server::CConfig::ESource::FileBinary, oData);
    if (oData.findString("TestUser") < oData.size() &&
        oData.findString("TestPassword") < oData.size() &&
        oData.findString("TestUser2") < oData.size() &&
        oData.findString("TestPassword2") < oData.size()
        )
    {
      bSuccess = true;
    }
  }

  return bSuccess;
}