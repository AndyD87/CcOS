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
#include "CcStringUtil.h"
#include "CcHttpClient.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonNode.h"

//! Default timeout value for nginx operations
#define DEFAULT_TIMEOUT_TEST CcDateTimeFromSeconds(5)
const CcString CClientTest::c_sBaseUrl = "http://127.0.0.1:36580";

#ifndef HttpTestEnvironment_PATH
  //! Cmake injected value for nginx config and html path
  #define HttpTestEnvironment_PATH ""
#endif

#ifndef HttpTestEnvironment_nginx
  //! Cmake injected value for nginx executable
  #define HttpTestEnvironment_nginx ""
#endif

#ifndef HttpTestEnvironment_phpcgi
  //! Cmake injected value for php-cgi executable
  #define HttpTestEnvironment_phpcgi ""
#endif

#ifdef WINDOWS
  //! Startup params for nginx on windows
  #define NGINX_START_PARAMS ""
#else
  //! Startup params for nginx on linux
  #define NGINX_START_PARAMS "-c \"@HttpTestEnvironment_PATH@/conf/nginx.conf\" -p \"@HttpTestEnvironment_PATH@\" -g \"daemon off;\""
#endif

#ifdef WINDOWS
  //! Startup params for php-cgi on windows
  #define PHP_START_PARAMS "-b 127.0.0.1:36500"
#else
  //! Startup params for php-cgi on linux
  #define PHP_START_PARAMS "-b 127.0.0.1:36500"
#endif

CClientTest::CClientTest() :
  CcTest("CClientTest"),
  m_sTestEnvPath(HttpTestEnvironment_PATH),
  m_sNginxPath(HttpTestEnvironment_nginx),
  m_sPhpcgiPath(HttpTestEnvironment_phpcgi)
{
  if(downloadTestEnv())
  {
    appendTestMethod("Start test environment", &CClientTest::startTestEnv);

    appendTestMethod("Test cookie set on client", &CClientTest::testCookieSet);
    appendTestMethod("Test cookie get from client", &CClientTest::testCookieGet);

    appendTestMethod("Stop test environment", &CClientTest::stopTestEnv);
  }
}

CClientTest::~CClientTest()
{
  CCDELETE(m_pPhpFastCgi);
  if(m_pPhpFastCgi)
  {
    CCDELETE(m_pPhpFastCgi);
  }
  if(m_pWebserver)
  {
    stopTestEnv();
    CCDELETE(m_pWebserver);
  }
}

bool CClientTest::downloadTestEnv()
{
  bool bDownloadDone = false;
  if(CcDirectory::exists(m_sTestEnvPath))
  {
    if(CcFile::exists(m_sNginxPath))
    {
      if(CcFile::exists(m_sPhpcgiPath))
      {
        bDownloadDone = true;
      }
    }
  }
  return bDownloadDone;
}

bool CClientTest::startTestEnv()
{
  CCNEW(m_pWebserver, CcProcess, m_sNginxPath);
  CcString sCleaned = CcString(NGINX_START_PARAMS).replace("@HttpTestEnvironment_PATH@", m_sTestEnvPath);
  m_pWebserver->setWorkingDirectory(m_sTestEnvPath);
  m_pWebserver->setArguments(CcStringUtil::getArguments(sCleaned));
  m_pWebserver->start();

  CCNEW(m_pPhpFastCgi, CcProcess, m_sPhpcgiPath);
  sCleaned = CcString(PHP_START_PARAMS).replace("@HttpTestEnvironment_PATH@", m_sPhpcgiPath);
  m_pPhpFastCgi->setArguments(CcStringUtil::getArguments(sCleaned));
  m_pPhpFastCgi->start();

  if(m_pWebserver->waitForRunning(DEFAULT_TIMEOUT_TEST))
  {
    // Wait for http server started
    CcHttpClient oClient(c_sBaseUrl);
    size_t iTimeout = 10;
    while(!oClient.execGet() && iTimeout)
    {
      CcKernel::sleep(1000);
      iTimeout--;
    }
    if(iTimeout &&
       oClient.getByteArray().size())
    {
      return true;
    }
  }
  return false;
}

bool CClientTest::stopTestEnv()
{
  CCDELETE(m_pPhpFastCgi);

  CcString sCleaned = CcString(NGINX_START_PARAMS).replace("@HttpTestEnvironment_PATH@", m_sTestEnvPath);
  sCleaned +=  " -s quit";
  CcStatus oStatus = CcProcess::exec(m_sNginxPath, CcStringUtil::getArguments(sCleaned), m_sTestEnvPath, true, DEFAULT_TIMEOUT_TEST);
  if(oStatus)
  {
    return m_pWebserver->waitForExit(DEFAULT_TIMEOUT_TEST);
  }
  else
  {
    return oStatus;
  }
}

bool CClientTest::testCookieSet()
{
  bool bSuccess = false;
  CcHttpClient oClient;
  oClient.getCookies().append("TestCookie", "TestString");
  oClient.setUrl(c_sBaseUrl + "/cookie.php?do=echo");
  if(oClient.execGet())
  {
    CcString sData = oClient.getByteArray();
    CcJsonDocument oDoc;
    if(oDoc.parseDocument(sData) && oDoc.getJsonNode().isObject())
    {
      if(oDoc.getJsonNode().object().size() > 0 &&
         oDoc.getJsonNode().object()[0].isValue() &&
         oDoc.getJsonNode().object()[0].getName() == "TestCookie" &&
         oDoc.getJsonNode().object()[0].getValue() == "TestString")
      {
        bSuccess = true;
      }
    }
  }
  else
  {
    CcTestFramework::writeError("Failed to set cookie: " + oClient.getByteArray());
  }
  return bSuccess;
}

bool CClientTest::testCookieGet()
{
  bool bSuccess = false;
  CcHttpClient oClient;
  oClient.setUrl(c_sBaseUrl + "/cookie.php?do=set&name=TestCookie1&value=TestString2");
  if(oClient.execGet())
  {
    if(oClient.getCookies().size() > 0)
    {
      if(oClient.getCookies().exists("TestCookie1") &&
         oClient.getCookies().getValue("TestCookie1") == "TestString2")
      {
        bSuccess = true;
      }
    }
  }
  else
  {
    CcTestFramework::writeError("Failed to set cookie: " + oClient.getByteArray());
  }
  return bSuccess;
}
