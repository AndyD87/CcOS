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
 * @brief    Entry point for Application
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CBaseTest.h"
#include "CArgumentTest.h"
#include "CKernelTest.h"
#include "CIniFileTest.h"
#include "CThreadTest.h"
#include "CUuidTest.h"
#include "CFileTest.h"
#include "CStringTest.h"
#include "CStringUtilTest.h"
#include "CTimeTest.h"
#include "CHashTest.h"
#include "CHmacTest.h"
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
#include "CMapTest.h"
#include "CVectorTest.h"
#include "CEventTest.h"
#include "CBufferTest.h"
#include "CMutexTest.h"
#include "CStringListTest.h"
#include "CSystemTest.h"
#include "CSystemServiceTest.h"
#include "CGenericMallocTest.h"
#include "CWorkerTest.h"
#include "CBoardSupportTest.h"

#include "CcTestFramework.h"
#include "CcMemoryMonitor.h"

/**
 * @brief Default application entry point
 * @param iArgc:  Argument count in ppArgv
 * @param ppArgv: Passed arguments from callup
 * @return Exitcode, default 0 if no error occured
 */
int main(int iArgc, char** ppArgv)
{
  CcTestFramework::init(iArgc, ppArgv);
  CcConsole::writeLine("Start: CcKernelTest");
#ifdef DEBUG
  CcKernel::initCLI();
#endif
  if(CcKernel::getPlatform() == EPlatform::Generic)
  {
    CcTestFramework::setInstantStart();
  }

  CcTestFramework_addTest(CBaseTest);
  CcTestFramework_addTest(CKernelTest);
  CcTestFramework_addTest(CArgumentTest);
  CcTestFramework_addTest(CGenericMallocTest);
  CcTestFramework_addTest(CSystemTest);
  CcTestFramework_addTest(CSystemServiceTest);
  CcTestFramework_addTest(CVectorTest);
  CcTestFramework_addTest(CListTest);
  CcTestFramework_addTest(CMapTest);
  CcTestFramework_addTest(CIniFileTest);
  CcTestFramework_addTest(CBoardSupportTest);
  CcTestFramework_addTest(CThreadTest);
  CcTestFramework_addTest(CWorkerTest);
  CcTestFramework_addTest(CUuidTest);
  CcTestFramework_addTest(CHashTest);
  CcTestFramework_addTest(CHmacTest);
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

  CcTestFramework::runTests();

  // Shutdown kernel in Testframework deinit to check if all running threads
  // will be closed safely.
  return CcTestFramework::deinit(true);
}
