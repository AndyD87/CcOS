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
#include "CcRemoteDeviceServerConfig.h"

CRemoteDeviceServerTest::CRemoteDeviceServerTest() :
  CcTest("CRemoteDeviceServerTest")
{
  appendTestMethod("Start test server", &CRemoteDeviceServerTest::testStartServer);
  appendTestMethod("Test read write default config", &CRemoteDeviceServerTest::testDefaultConfig);
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
  CcJsonDocument oDoc;
  CcRemoteDeviceServerConfig oConfig(false);
  CcString sDefaultConfig(CcRemoteDeviceGeneric_json, CcRemoteDeviceGeneric_json_Length);
  oDoc.parseDocument(sDefaultConfig.trim());
  oConfig.parseJson(oDoc.getJsonData());
  CcString sReadConfig = oConfig.writeJson().trim();
  if(sReadConfig == sDefaultConfig)
  {
    oStatus = true;
  }
  else
  {
    CCDEBUG("Comparing failed: ");
    CCDEBUG("");
    CCDEBUG(sReadConfig);
    CCDEBUG("");
    CCDEBUG(sDefaultConfig);
  }
  return oStatus;
}

