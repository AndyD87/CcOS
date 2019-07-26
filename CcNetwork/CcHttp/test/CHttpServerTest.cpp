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
 * @brief     Implemtation of class CHttpServerTest
 */
#include "CHttpServerTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcString.h"
#include "CcHttpServer.h"
#include "CcHttpServerConfig.h"

class CHttpServerTest::CPrivate
{
public:
  CcString sTempDir;
  CcString sPrivateKeyFile;
  CcString sPublicKeyFile;
};

CHttpServerTest::CHttpServerTest() :
  CcTest("CHttpServerTest")
{
  appendTestMethod("Test for starting an http server", &CHttpServerTest::startHttpServer);
  appendTestMethod("Test for starting an https server", &CHttpServerTest::startHttpsServer);

  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->sTempDir = CcKernel::getTempDir();
  m_pPrivate->sPrivateKeyFile = m_pPrivate->sTempDir;
  m_pPrivate->sPublicKeyFile = m_pPrivate->sTempDir;
  m_pPrivate->sPrivateKeyFile.appendPath("Private.crt");
  m_pPrivate->sPublicKeyFile.appendPath("Public.crt");
}

CHttpServerTest::~CHttpServerTest()
{
  CCDELETE(m_pPrivate);
}

bool CHttpServerTest::startHttpServer()
{
  CcStatus oStatus;
  size_t uiTimeout = 50;
  CcHttpServerConfig oConfig(CcCommonPorts::CcTestBase + CcCommonPorts::HTTP);
  CCNEWTYPE(pServer, CcHttpServer, CcCommonPorts::CcTestBase + CcCommonPorts::HTTP);
  pServer->setConfig(&oConfig);
  pServer->start();
  pServer->waitForState(EThreadState::Running);
  while (uiTimeout > 0)
  {
    if (pServer->getState() == CcHttpServer::EState::Listening)
      break;
    CcKernel::delayMs(100); 
    uiTimeout--;
  }
  CcTestFramework::writeInfo("Stop server now.");
  pServer->stop();
  CcTestFramework::writeInfo("Stop server done.");
  pServer->start();
  CcKernel::delayMs(100);
  CcTestFramework::writeInfo("Stop server now.");
  pServer->stop();
  CcTestFramework::writeInfo("Stop server done.");
  pServer->start();
  CcKernel::delayMs(100);
  CcTestFramework::writeInfo("Stop server now.");
  pServer->stop();
  CcTestFramework::writeInfo("Stop server done.");
  pServer->start();
  CcKernel::delayMs(100);
  CcTestFramework::writeInfo("Stop server now.");
  pServer->stop();
  CcTestFramework::writeInfo("Stop server done.");
  CCDELETE(pServer);
  if (uiTimeout == 0)
  {
    oStatus = EStatus::TimeoutReached;
  }
  return oStatus;;
}

bool CHttpServerTest::startHttpsServer()
{
  CcStatus oStatus;
  CcHttpServerConfig oConfig(CcCommonPorts::CcTestBase + CcCommonPorts::HTTPS);
  CcTestFramework::writeInfo("Set ssl.");
  oConfig.setSslEnabled(true);
  CcTestFramework::writeInfo("Start server now.");
  CCNEWTYPE(pServer, CcHttpServer, CcCommonPorts::CcTestBase + CcCommonPorts::HTTPS);
  pServer->start();
  pServer->waitForState(EThreadState::Running);
  size_t uiTimeout = 50;
  while (uiTimeout > 0)
  {
    if (pServer->getState() == CcHttpServer::EState::Listening)
      break;
    CcKernel::delayMs(100);
    uiTimeout--;
  }
  CcTestFramework::writeInfo("Stop server now.");
  pServer->stop();
  CcTestFramework::writeInfo("Stop server done.");
  CCDELETE(pServer);
  if (uiTimeout == 0)
  {
    oStatus = EStatus::TimeoutReached;
  }
  return oStatus;;
}
