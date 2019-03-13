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
 * @brief     Implemtation of class CThreadTest
 */
#include "CThreadTest.h"
#include "IThread.h"

class CSimpleRunThread : public IThread
{
  virtual void run() override
  {
    CcTestFramework::writeInfo("CSimpleRunThread");
    bRunningReset = true;
  }
public:
  volatile bool bRunningReset = false;
};

CThreadTest::CThreadTest()
{
  appendTestMethod("Test thread execution in current context", &CThreadTest::testThreadExec);
  appendTestMethod("Test thread execution in new context", &CThreadTest::testThreadExec);
}

CThreadTest::~CThreadTest()
{
}

bool CThreadTest::testThreadExec()
{
  CSimpleRunThread oSimpleThread;
  oSimpleThread.startOnCurrent();
  return oSimpleThread.bRunningReset;
}

bool CThreadTest::testThreadStartWait()
{
  CSimpleRunThread oSimpleThread;
  oSimpleThread.start();
  CcStatus oState = oSimpleThread.waitForExit(CcDateTimeFromSeconds(1));
  return oState;
}
