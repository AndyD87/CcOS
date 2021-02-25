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
 * @brief     Implemtation of class CClientTest
 */
#include "CClientTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcDirectory.h"
#include "CcProcess.h"

#ifndef HttpTestEnvironment_PATH
  #define HttpTestEnvironment_PATH ""
#endif

CClientTest::CClientTest() :
  CcTest("CClientTest"),
  m_sTestEnvPath(HttpTestEnvironment_PATH)
{
  if(downloadTestEnv())
  {
    appendTestMethod("Start test environment", &CClientTest::startTestEnv);
  }
}

CClientTest::~CClientTest()
{
  CCDELETE(m_pPhpFastCgi);
  CCDELETE(m_pWebserver);
}

bool CClientTest::downloadTestEnv()
{
  bool bDownloadDone = true;
  return bDownloadDone;
}

bool CClientTest::startTestEnv()
{
  bool bSuccess = false;
  if(CcDirectory::exists(m_sTestEnvPath))
  {
    CcString sWebserver = m_sTestEnvPath;
    sWebserver.appendPath("nginx");
    sWebserver.appendPath("nginx.exe");
    CcString sPhpCgi = m_sTestEnvPath;
    sPhpCgi.appendPath("php");
    sPhpCgi.appendPath("php-cgi.exe");
    if(CcDirectory::exists(sWebserver) &&
       CcDirectory::exists(sPhpCgi)
    )
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}
