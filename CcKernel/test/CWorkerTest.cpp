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
 * @brief     Implemtation of class CWorkerTest
 */
#include "CWorkerTest.h"
#include "IWorker.h"
#include "CcTestFramework.h"
#include "CcVector.h"
#include "CcMutex.h"
#include "CcDateTime.h"
#include "CcKernel.h"

class CWorkerTestSimpleWorker : public IWorker
{
public:

  CWorkerTestSimpleWorker() :
    IWorker("CWorkerTestSimpleWorker")
  {
    m_uiId = s_uiId++;
  }

  virtual ~CWorkerTestSimpleWorker()
  {
    CWorkerTestSimpleWorker::remove(this);
    CWorkerTestSimpleWorker::s_uiIdClosed++;
  }

  virtual void run() override
  {
    CcTestFramework::writeInfo("Worker with id: " + CcString::fromSize(m_uiId));
  }

  static void append(IWorker* pWorker)
  {
    CWorkerTestSimpleWorker::s_oWorkersMutex.lock();
    CWorkerTestSimpleWorker::s_oWorkers.append(pWorker);
    CWorkerTestSimpleWorker::s_oWorkersMutex.unlock();
  }

  static void remove(IWorker* pWorker)
  {
    CWorkerTestSimpleWorker::s_oWorkersMutex.lock();
    CWorkerTestSimpleWorker::s_oWorkers.removeItem(pWorker);
    CWorkerTestSimpleWorker::s_oWorkersMutex.unlock();
  }

public:
  size_t                    m_uiId;
  static size_t             s_uiId;
  static size_t             s_uiIdClosed;
  static CcVector<IWorker*> s_oWorkers;
  static CcMutex            s_oWorkersMutex;
};

size_t CWorkerTestSimpleWorker::s_uiId = 0;
size_t CWorkerTestSimpleWorker::s_uiIdClosed = 0;
CcVector<IWorker*> CWorkerTestSimpleWorker::s_oWorkers;
CcMutex CWorkerTestSimpleWorker::s_oWorkersMutex;

CWorkerTest::CWorkerTest() :
  CcTest("CWorkerTest")
{
  appendTestMethod("Test if multiple workers can be started", &CWorkerTest::testMultipleWorkers);
}

CWorkerTest::~CWorkerTest()
{
}

bool CWorkerTest::testMultipleWorkers()
{
  bool bRet = false;
  for (int i = 0; i < 100; i++)
  {
    CCNEWTYPE(pWorker, CWorkerTestSimpleWorker);
    CWorkerTestSimpleWorker::append(pWorker);
    pWorker->start();
  }

  CcDateTime oMaxTime(CcKernel::getDateTime());
  // wait maximum 10 seconds until timeout
  oMaxTime.addSeconds(10);
  while(CWorkerTestSimpleWorker::s_uiId != CWorkerTestSimpleWorker::s_uiIdClosed &&
        oMaxTime > CcKernel::getDateTime()
    )
  { }
  if (CWorkerTestSimpleWorker::s_uiId == CWorkerTestSimpleWorker::s_uiIdClosed &&
      CWorkerTestSimpleWorker::s_oWorkers.size() == 0)
  {
    bRet = true;
  }
  return bRet;
}
