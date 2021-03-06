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
 * @brief     Implemtation of class CRemoteDeviceServerTest
 */
#include "CRemoteDeviceServerTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcString.h"
#include "Resources/CcRemoteDeviceGeneric.json.h"
#include "CcJson/CcJsonDocument.h"
#include "CcRemoteDeviceServer.h"
#include "Server/CConfig.h"

CRemoteDeviceServerTest::CRemoteDeviceServerTest() :
  CcTest("CRemoteDeviceServerTest")
{
  appendTestMethod("Start test server", &CRemoteDeviceServerTest::testStartServer);
  appendTestMethod("Test read write default config", &CRemoteDeviceServerTest::testDefaultConfig);
  appendTestMethod("Test read and write binary config", &CRemoteDeviceServerTest::testBinaryConfig);
}

CRemoteDeviceServerTest::~CRemoteDeviceServerTest()
{
}

bool CRemoteDeviceServerTest::testStartServer()
{
  CcStatus oStatus = false;
  CcRemoteDeviceServer oServer;
  oStatus = oServer.start();
  if(oStatus)
  {
    oServer.stop();
    oStatus = oServer.waitForExit(CcDateTimeFromSeconds(5));
  }
  return oStatus;
}

bool CRemoteDeviceServerTest::testDefaultConfig()
{
  CcStatus oStatus = false;
  NRemoteDevice::Server::CConfig oConfig(false);
  CcString sDefaultConfig(NRemoteDevice::Server::CConfig::getDefaultConfig(), NRemoteDevice::Server::CConfig::getDefaultConfigSize());
  sDefaultConfig.trim();
  oConfig.readData(sDefaultConfig);
  oConfig.write();
  CcByteArray oData;
  oConfig.writeData(NRemoteDevice::Server::CConfig::ESource::FileJson, oData);
  CcString sReadConfig = oData;
  if(sReadConfig == sDefaultConfig)
  {
    oStatus = true;
  }
  else
  {
    CcTestFramework::writeError("Comparing failed: ");
    CcTestFramework::writeError("");
    CcTestFramework::writeError(sReadConfig);
    CcTestFramework::writeError(sDefaultConfig);
    CcTestFramework::writeError("");
  }
  return oStatus;
}

bool CRemoteDeviceServerTest::testBinaryConfig()
{
  CcStatus oStatus = false;
  CcJsonDocument oDoc;
  NRemoteDevice::Server::CConfig oConfig(false);
  CcString sDefaultConfig(NRemoteDevice::Server::CConfig::getDefaultConfig(), NRemoteDevice::Server::CConfig::getDefaultConfigSize());
  oDoc.parseDocument(sDefaultConfig.trim());
  oConfig.readData(sDefaultConfig);

  CcByteArray oData;
  oConfig.writeData(NRemoteDevice::Server::CConfig::ESource::FileBinary, oData);

  NRemoteDevice::Server::CConfig oConfigBinary;
  oConfigBinary.readData(oData);

  CcByteArray oData2;
  oConfigBinary.writeData(NRemoteDevice::Server::CConfig::ESource::FileJson, oData2);
  CcString sConfig = oData2;
  if(sConfig == sDefaultConfig)
  {
    oStatus = true;
  }
  else
  {
    CcTestFramework::writeError("Wrong default config after binary parsed:");
    CcTestFramework::writeError("Original:");
    CcTestFramework::writeError("  " + sDefaultConfig);
    CcTestFramework::writeError("New:");
    CcTestFramework::writeError("  " + sConfig);
  }
  return oStatus;
}


