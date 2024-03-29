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
 * @brief     Implemtation of class CMqttClientTest
 */
#include "CMqttClientTest.h"
#include "CcKernel.h"
#include "CcString.h"
#include "CcMqttClient.h"
#include "CcDateTime.h"

CMqttClientTest::CMqttClientTest() :
  CcTest("CMqttClientTest")
{
  appendTestMethod("Test basic CcMqttClient", &CMqttClientTest::testStartStop);
  appendTestMethod("Test basic CcMqttClient multiple times", &CMqttClientTest::testStartStopLoop);
}

CMqttClientTest::~CMqttClientTest()
{
}

bool CMqttClientTest::testStartStop()
{
  bool bSuccess = true;
  CcMqttClient oClient;
  oClient.start();
  oClient.stop();
  return bSuccess;
}

bool CMqttClientTest::testStartStopLoop()
{
  bool bSuccess = true;
  for (int i = 0; i < 200; i++)
  {
    CcMqttClient oClient;
    oClient.start();
    bSuccess = oClient.waitForRunning(CcDateTime(CcDateTimeFromSeconds(1)));
    if (bSuccess)
    {
      oClient.stop();
      bSuccess = oClient.waitForExit(CcDateTime(CcDateTimeFromSeconds(1)));
    }
  }
    return bSuccess;
}
