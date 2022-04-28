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
 * @brief     Implemtation of class CRemoteDeviceComTest
 */
#include "CRemoteDeviceComTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcString.h"
#include "CcRemoteDeviceServer.h"
#include "CcRemoteDeviceDiscovery.h"
#include "Server/CConfig.h"
#include "Network/CcCommonPorts.h"
#include "CcIp.h"

//! Test port to bind and connection on running unit test
static const uint16 CRemoteDeviceComTest_uiTestPort     = CcCommonPorts::CcRemoteDevice + CcCommonPorts::CcTestBase;
static const uint16 CRemoteDeviceComTest_uiHttpTestPort = CcCommonPorts::HTTP           + CcCommonPorts::CcTestBase;

CRemoteDeviceComTest::CRemoteDeviceComTest() :
  CcTest("CRemoteDeviceComTest")
{
  CCNEWTYPE(pConfig, NRemoteDevice::Server::CConfig);
  CCNEW(m_pDevice, CcRemoteDeviceServer, pConfig, false);
  appendTestMethod("Start test server", &CRemoteDeviceComTest::testStartServer);
  appendTestMethod("Test discovery", &CRemoteDeviceComTest::testDiscover);
  appendTestMethod("Stop test server", &CRemoteDeviceComTest::testStopServer);
}

CRemoteDeviceComTest::~CRemoteDeviceComTest()
{
  NRemoteDevice::Server::CConfig* pConfig = &m_pDevice->getConfig();
  CCDELETE(m_pDevice);
  CCDELETE(pConfig);
}

bool CRemoteDeviceComTest::testStartServer()
{
  CcStatus oStatus = false;
  if(m_pDevice)
  {
    m_pDevice->getConfig().oInterfaces.oHttpServer.getAddressInfo().setPort(CRemoteDeviceComTest_uiHttpTestPort);
    m_pDevice->getConfig().oAddressInfo.setPort(CRemoteDeviceComTest_uiTestPort);
    m_pDevice->getConfig().oAddressInfo.setIp(CcIp("127.0.0.1"));
    if (m_pDevice->start())
    {
      oStatus = m_pDevice->waitForState(EThreadState::Running);
    }
  }
  return oStatus;
}

bool CRemoteDeviceComTest::testDiscover()
{
  CcStatus oStatus = EStatus::NotFound;
  CcRemoteDeviceDiscovery oDiscvover;
  oDiscvover.getAddressInfo().setIp(CcIp("127.0.0.1"));
  size_t uiTimeout = 5;
  while (uiTimeout-- > 0)
  {
    if (0 != oDiscvover.findAllDevices(CRemoteDeviceComTest_uiTestPort))
    {
      oStatus = true;
      break;
    }
    else
    {
      CcKernel::delayS(1);
    }
  }
  
  return oStatus;
}

bool CRemoteDeviceComTest::testStopServer()
{
  CcStatus oStatus = false;
  if (m_pDevice)
  {
    m_pDevice->stop();
    if (m_pDevice->waitForExit())
    {
      oStatus = true;
    }
  }
  return oStatus;
}


