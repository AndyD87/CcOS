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
 * @brief     Implemtation of class CTestTestUtility
 */
#include "CTestTestUtility.h"
#include "CcTestFramework.h"
#include "CcTestUtility.h"
#include "CcProcess.h"
#include "CcFile.h"

CTestTestUtility::CTestTestUtility() :
  CcTest<CTestTestUtility>("CTestTestUtility")
{
  appendTestMethod("Test and verify file", &CTestTestUtility::fileGenerationTest);
  appendTestMethod("Test and verify file with executable", &CTestTestUtility::fileGenerationTestExecutable);
}

CTestTestUtility::~CTestTestUtility()
{
}

bool CTestTestUtility::fileGenerationTest()
{
  bool bSuccess = false;
  CcString sTempDir = CcTestFramework::getTemporaryDir();
  sTempDir.appendPath("Test.file");
  CcStatus oStatus = CcTestUtility::generateAndVerifyFile(sTempDir, 1024*1024 + 77);
  bSuccess = oStatus;
  CcFile::remove(sTempDir);
  return bSuccess;
}

bool CTestTestUtility::fileGenerationTestExecutable()
{
  CcString sTempDir = CcTestFramework::getTemporaryDir();
  sTempDir.appendPath("Test.file");
  CcString sBinDir = CcTestFramework::getBinaryDir();
#ifdef WINDOWS
  sBinDir.append(".exe");
#endif
  sBinDir.appendPath("CcTestingTest");
  CcStringList oParams;
  oParams.append("run").append("generateAndVerifyFile").append(sTempDir).append(CcString::fromInt(1024*1024 + 77));
  CcStatus oState = CcProcess::exec(sBinDir, oParams);
  return oState;
}
