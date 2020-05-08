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
 * @brief     Implemtation of class CTestModuleTest
 */
#include "CTestModuleTest.h"
#include "CcTestFramework.h"
#include "CcKernel.h"
#include "CcString.h"
#include "CcProcess.h"
#include "CcFile.h"

CTestModuleTest::CTestModuleTest() :
  CcTest<CTestModuleTest>("CTestModuleTest")
{
  appendTestMethod("Check if external Application can fail", &CTestModuleTest::faileTest);
  appendTestMethod("Check if external Application will succeed", &CTestModuleTest::successTest);
}

CTestModuleTest::~CTestModuleTest()
{
}

bool CTestModuleTest::faileTest()
{
  bool bSuccess = false;
  CcString sBinDir = CcTestFramework::getBinaryDir();
#ifdef WINDOWS
  sBinDir.appendPath("CcOSModuleLoader.exe");
#else
  sBinDir.appendPath("CcOSModuleLoader");
#endif

  if (CcFile::exists(sBinDir))
  {
    CcProcess oProcess(sBinDir);
    oProcess.addArgument("WrongFile");
    CcStatus oState = oProcess.exec(CcDateTimeFromSeconds(4));
    if (oState != true && oState != EStatus::TimeoutReached)
      bSuccess = true;
  }
  return bSuccess;
}

bool CTestModuleTest::successTest()
{
  bool bSuccess = false;
  CcString sBinDir = CcTestFramework::getBinaryDir();
  CcString sLibDir = CcTestFramework::getBinaryDir();
#ifdef WINDOWS
  sBinDir.appendPath("CcOSModuleLoader.exe");
  sLibDir.appendPath("CcTesting.dll");
#else
  sBinDir.appendPath("CcOSModuleLoader");
  sLibDir.appendPath("../lib/CcTesting.so");
#endif

  if (CcFile::exists(sBinDir) &&
      CcFile::exists(sLibDir))
  {
    CcProcess oProcess(sBinDir);
    oProcess.addArgument(sLibDir);
    bSuccess = oProcess.exec(CcDateTimeFromSeconds(4));
  }
  return bSuccess;
}
