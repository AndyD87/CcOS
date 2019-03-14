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
 * @brief     Implemtation of class CKernelTest
 */
#include "CKernelTest.h"
#include "CcString.h"
#include "CcConsole.h"
#include "CcKernel.h"
#include "CcFileSystem.h"
#include "CcGlobalStrings.h"

CKernelTest::CKernelTest()
{
  appendTestMethod("Test environment variables", &CKernelTest::testEnvironmentVariables);
}

bool CKernelTest::testEnvironmentVariables()
{
  // Generic System does not have PATH in env vars
#ifdef GENERIC
  CcKernel::setEnvironmentVariable("PATH", "/bin");
#endif
  bool bRet = false;
  // test PATH, it is always available
  if (CcKernel::getEnvironmentVariableExists("PATH"))
  {
    CcString sInitPath = CcKernel::getEnvironmentVariable("PATH");
    CcString sPathAppended;
    sPathAppended = sInitPath + CcGlobalStrings::Seperators::Semicolon + CcKernel::getWorkingDir();
    if (CcKernel::setEnvironmentVariable("PATH", sPathAppended))
    {
      CcString sTestPath = CcKernel::getEnvironmentVariable("PATH");
      if (sTestPath == sPathAppended)
      {
        if (CcKernel::setEnvironmentVariable("PATH", sInitPath))
        {
          sTestPath = CcKernel::getEnvironmentVariable("PATH");
          if (sTestPath == sInitPath)
          {
            bRet = true;
          }
        }
      }
      else
      {
        CCERROR("testEnvironmentVariable failed:");
        CCERROR("Init:  " + sInitPath);
        CCERROR("New:   " + sPathAppended);
        CCERROR("Test:  " + sTestPath);
      }
    }
  }
  if(bRet == true)
  {
    CcStringMap oInitAllEnv = CcKernel::getEnvironmentVariables();
    CcString sInitPath = CcKernel::getEnvironmentVariable("PATH");
    bRet = false;
    if( sInitPath.length() > 0 &&
        CcKernel::removeEnvironmentVariable("PATH"))
    {
      CcStringMap oTestAllEnv = CcKernel::getEnvironmentVariables();
      oTestAllEnv.clear();
      if(oTestAllEnv.size() +1 == oInitAllEnv.size() &&
          oTestAllEnv.containsKey("PATH") == false)
      {
        if(CcKernel::setEnvironmentVariable("PATH", sInitPath))
        {
          bRet = true;
        }
      }
      oTestAllEnv.clear();
    }
  }
  return bRet;
}
