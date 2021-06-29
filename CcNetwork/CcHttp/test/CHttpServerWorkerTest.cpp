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
 * @brief     Implemtation of class CHttpServerWorkerTest
 */
#include "CHttpServerWorkerTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcString.h"
#include "CcTestSocket.h"
#include "CcHttpServer.h"
#include "CcHttpServerWorker.h"
#include "CcHttpServerConfig.h"
#include "HttpProvider/IHttpPathProvider.h"

class CTestProvider : public IHttpPathProvider
{
public:
  CTestProvider() :
    IHttpPathProvider("test.php")
  {}

  virtual CcStatus execGet(CcHttpWorkData& oData)
  {
    CcStatus oState;
    oData.readAllContent();
    oReceivedDate = oData.readAll();
    return oState;
  }

  CcByteArray oReceivedDate;
};

class CHttpServerWorkerTest::CPrivate
{
public:
  CPrivate() = default;
  CTestProvider oProvider;
  CcHttpServer  oServer;
};

CHttpServerWorkerTest::CHttpServerWorkerTest() :
  CcTest("CHttpServerWorkerTest")
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->oServer.registerProvider(&m_pPrivate->oProvider);
  appendTestMethod("Test worker for chunked transfer", &CHttpServerWorkerTest::testChunkedTransfer);
}

CHttpServerWorkerTest::~CHttpServerWorkerTest()
{
  CCDELETE(m_pPrivate);
}

bool CHttpServerWorkerTest::testChunkedTransfer()
{
  CcStatus oStatus = false;
  m_pPrivate->oServer.incWorker();
  CCNEWTYPE(pSocket, CcTestSocket);
  pSocket->writeString( "GET test.php HTTP/1.1\r\n"
                        "Host: localhost\r\n"
                        "Transfer-Encoding: chunked\r\n"
                        "\r\n"
                        "4\r\n"
                        "Wiki\r\n"
                        "6\r\n"
                        "pedia \r\n"
                        "E\r\n"
                        "in \r\n"
                        "\r\n"
                        "chunks.\r\n"
                        "0\r\n"
                        "\r\n");
  CCNEWTYPE(pWorker, CcHttpServerWorker, m_pPrivate->oServer, pSocket);
  if(pWorker->start())
    oStatus = pWorker->waitForExit(CcDateTimeFromSeconds(1));
  else
  {
    CcTestFramework::writeError("Worker not processed within timeout.");
  }
  if (oStatus)
  {
    CcByteArray oTargetValue("Wikipedia in \r\n\r\nchunks.");
    if (oTargetValue == m_pPrivate->oProvider.oReceivedDate)
    {
      oStatus = true;
    }
    else
    {
      CcTestFramework::writeError("Chunked data not successfully extrated.");
      oStatus = false;
    }
  }
  // Worker is deleting itself
  return oStatus;
}
