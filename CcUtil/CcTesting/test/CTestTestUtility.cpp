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
#include "IProgressReceiver.h"

class CTestTestUtility::CProgress : public IProgressReceiver
{
public:
  virtual ~CProgress(){}
  virtual void update(uint64 uiValue, uint64 uiFrom) override
  {
    CcTestFramework::writeInfo("Progress: " + CcString::fromNumber(uiValue) + " from " + CcString::fromNumber(uiFrom));
  }
};

CTestTestUtility::CTestTestUtility() :
  CcTest<CTestTestUtility>("CTestTestUtility")
{
  appendTestMethod("Test and verify file", &CTestTestUtility::fileGenerationTest);
  appendTestMethod("Test and verify file with executable", &CTestTestUtility::fileGenerationTestExecutable);
  appendTestMethod("Test exit code with instant return", &CTestTestUtility::testExitCodeInstant);
  appendTestMethod("Test exit code with delayed return", &CTestTestUtility::testExitCodeTimed);
}

CTestTestUtility::~CTestTestUtility()
{
}

bool CTestTestUtility::fileGenerationTest()
{
  bool bSuccess = false;
  CcString sTempDir = CcTestFramework::getTemporaryDir();
  sTempDir.appendPath("Test.file");
  CProgress oProgress;
  CcStatus oStatus = CcTestUtility::generateAndVerifyFile(sTempDir, 1024*1024 + 77, 77, &oProgress);
  bSuccess = oStatus;
  CcFile::remove(sTempDir);
  return bSuccess;
}

bool CTestTestUtility::fileGenerationTestExecutable()
{
  CcString sTempDir = CcTestFramework::getTemporaryDir();
  sTempDir.appendPath("Test Spaced.file");
  CcString sBinDir = CcTestFramework::getBinaryDir();
  sBinDir.appendPath("CcTestingTest");
#ifdef WINDOWS
  sBinDir.append(".exe");
#endif
  CcStringList oParams;
  oParams.append("run").append("generateAndVerifyFile").append(sTempDir).append(CcString::fromInt(1024*1024 + 77)).append("keep");
  CcStatus oState = CcProcess::exec(sBinDir, oParams, CcGlobalStrings::Empty, true, CcDateTimeFromSeconds(100));
  if(oState)
  {
    if(CcFile::exists(sTempDir))
    {
      oParams.append("run").append("generateAndVerifyFile").append(sTempDir).append(CcString::fromInt(1024*1024 + 77));
      oState = CcProcess::exec(sBinDir, oParams, CcGlobalStrings::Empty, true, CcDateTimeFromSeconds(100));
      if(oState)
      {
        if(!CcFile::exists(sTempDir))
        {
          oState = false;
        }
        else
        {
          CcTestFramework::writeError("File not deleted");
        }
      }
      else
      {
        CcTestFramework::writeError("2nd execute generateAndVerifyFile failed");
      }
    }
    else
    {
      CcTestFramework::writeError("File delete, but hast to stay");
    }
  }
  else
  {
    CcTestFramework::writeError("Execute generateAndVerifyFile failed");
  }
  return oState;
}

bool CTestTestUtility::testExitCodeInstant()
{
  CcString sTempDir = CcTestFramework::getTemporaryDir();
  sTempDir.appendPath("Test.file");
  CcString sBinDir = CcTestFramework::getBinaryDir();
  sBinDir.appendPath("CcTestingTest");
#ifdef WINDOWS
  sBinDir.append(".exe");
#endif
  CcStringList oParams;
  int iEstimatedErrorCode = 100;
  oParams.append("run").append("exitInstant").append(CcString::fromInt(iEstimatedErrorCode));
  CcStatus oState = CcProcess::exec(sBinDir, oParams, CcGlobalStrings::Empty, true, CcDateTimeFromSeconds(100));
  if(oState.getErrorInt() == iEstimatedErrorCode)
  {
    oState = true;
  }
  else
  {
    CcTestFramework::writeError("Execute testExitCodeInstant failed");
  }
  return oState;
}

bool CTestTestUtility::testExitCodeTimed()
{
  CcString sTempDir = CcTestFramework::getTemporaryDir();
  sTempDir.appendPath("Test.file");
  CcString sBinDir = CcTestFramework::getBinaryDir();
  sBinDir.appendPath("CcTestingTest");
#ifdef WINDOWS
  sBinDir.append(".exe");
#endif
  CcStringList oParams;
  int iEstimatedErrorCode = 256;
  uint32 uiTimeout = 2;
  oParams.append("run").append("exitTimed").append(CcString::fromInt(iEstimatedErrorCode)).append(CcString::fromNumber(uiTimeout));
  CcStatus oState = CcProcess::exec(sBinDir, oParams, CcGlobalStrings:: Empty, true, CcDateTimeFromMSeconds(100));
  if(oState == EStatus::TimeoutReached)
  {
    oState = true;
  }
  else
  {
    CcTestFramework::writeError("Execute testExitCodeTimed failed");
  }
  return oState;
}
