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
 * @brief    Entry point for Application
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CBaseTest.h"
#include "CKernelTest.h"
#include "CThreadTest.h"
#include "CUuidTest.h"
#include "CFileTest.h"
#include "CStringTest.h"
#include "CStringUtilTest.h"
#include "CTimeTest.h"
#include "CHashTest.h"
#include "CByteArrayTest.h"
#include "CProcessTest.h"
#include "CStatusTest.h"
#include "CSharedMemoryTest.h"
#include "CcConsole.h"
#include "CNetworkTest.h"
#include "CUrlTest.h"
#include "CVersionTest.h"
#include "CPasswordTest.h"
#include "CListTest.h"
#include "CVectorTest.h"
#include "CEventTest.h"
#include "CBufferTest.h"
#include "CMutexTest.h"
#include "CStringListTest.h"
#include "CSystemTest.h"
#include "CGenericMallocTest.h"
#include "CWorkerTest.h"

#include "CcTestFramework.h"
#include "CcMemoryMonitor.h"

// Application entry point. 
int main(int argc, char **argv)
{
  CcTestFramework::init(argc, argv);
  CcConsole::writeLine("Start: CcKernelTest");
#ifdef DEBUG
  CcKernel::initCLI();
#endif
  if(CcKernel::getPlatform() == EPlatform::Generic)
  {
    CcTestFramework::SetInstantStart();
  }

  CcTestFramework_addTest(CBaseTest);
  CcTestFramework_addTest(CKernelTest);
  CcTestFramework_addTest(CSystemTest);
  CcTestFramework_addTest(CVectorTest);
  CcTestFramework_addTest(CListTest);
  CcTestFramework_addTest(CThreadTest);
  CcTestFramework_addTest(CWorkerTest);
  CcTestFramework_addTest(CUuidTest);
  CcTestFramework_addTest(CHashTest);
  CcTestFramework_addTest(CStringTest);
  CcTestFramework_addTest(CStringUtilTest);
  CcTestFramework_addTest(CPasswordTest);
  CcTestFramework_addTest(CFileTest);
  CcTestFramework_addTest(CTimeTest);
  CcTestFramework_addTest(CByteArrayTest);
  CcTestFramework_addTest(CProcessTest);
  CcTestFramework_addTest(CStatusTest);
  CcTestFramework_addTest(CSharedMemoryTest);
  CcTestFramework_addTest(CNetworkTest);
  CcTestFramework_addTest(CUrlTest);
  CcTestFramework_addTest(CVersionTest);
  CcTestFramework_addTest(CEventTest);
  CcTestFramework_addTest(CBufferTest);
  CcTestFramework_addTest(CMutexTest);
  CcTestFramework_addTest(CStringListTest);
  CcTestFramework_addTest(CGenericMallocTest);
  CcTestFramework::runTests();
  return CcTestFramework::deinit();
}
