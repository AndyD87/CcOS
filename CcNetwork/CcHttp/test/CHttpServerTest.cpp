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
  CcHttpServerConfig oConfig(CcCommonPorts::CcTestBase + CcCommonPorts::HTTP);
  CcHttpServer oServer(&oConfig);
  oServer.start();
  oServer.waitForState(EThreadState::Running);
  size_t uiTimeout = 50;
  while (uiTimeout > 0)
  {
    if (oServer.getState() == CcHttpServer::EState::Listening)
      break;
    CcKernel::delayMs(100); 
    uiTimeout--;
  }
  CcTestFramework::writeInfo("Stop server now.");
  oServer.stop();
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
  oConfig.setSslEnabled(true);
  CcHttpServer oServer(&oConfig);
  oServer.start();
  oServer.waitForState(EThreadState::Running);
  size_t uiTimeout = 50;
  while (uiTimeout > 0)
  {
    if (oServer.getState() == CcHttpServer::EState::Listening)
      break;
    CcKernel::delayMs(100);
    uiTimeout--;
  }
  CcTestFramework::writeInfo("Stop server now.");
  oServer.stop();
  if (uiTimeout == 0)
  {
    oStatus = EStatus::TimeoutReached;
  }
  return oStatus;;
}
