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
 * @brief     Implemtation of class CMutexTest
 */
#include "CMutexTest.h"
#include "IThread.h"
#include "CcMutex.h"

class CMutextTextThread : public IThread
{
public:
  CcMutex oMutex;
private:
  virtual void run() override
  {
    oMutex.lock();

    oMutex.unlock();
  }
};

CMutexTest::CMutexTest() :
  CcTest("CMutexTest")
{
  appendTestMethod("Test simple locking", &CMutexTest::testLock);
}

CMutexTest::~CMutexTest()
{
}

bool CMutexTest::testLock()
{
  bool bRet = false;
  CMutextTextThread oTestThread;
  oTestThread.oMutex.lock();
  oTestThread.start();
  CcStatus oStatus = oTestThread.waitForExit(CcDateTimeFromMSeconds(50));
  if (oStatus == EStatus::TimeoutReached)
  {
    oTestThread.oMutex.unlock();
    oStatus = oTestThread.waitForExit(CcDateTimeFromMSeconds(50));
    bRet = oStatus;
  }
  return bRet;
}
