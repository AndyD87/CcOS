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
 * @brief     Implemtation of class CProcessTest
 */
#include "CProcessTest.h"
#include "CcProcess.h"
#include "IIo.h"
#include "CcByteArray.h"
#include "CcConsole.h"

CProcessTest::CProcessTest() :
  CcTest("CProcessTest")
{
  appendTestMethod("Test echo console", &CProcessTest::testStdConsoleCommand);
}

CProcessTest::~CProcessTest()
{
}

bool CProcessTest::testStdConsoleCommand()
{
  bool bRet = false;
#ifdef WINDOWS
  CcProcess oProc("cmd.exe");
  oProc.addArgument("/C echo hallo");
#else
  CcProcess oProc("sh");
  //oProc.addArgument("-c \"echo hallo\"");
  oProc.addArgument("-c");
  oProc.addArgument("echo hallo");
#endif

  oProc.start();
  CcString sAll;
  while (oProc.hasExited() == false)
  {
    sAll += oProc.pipe().readAll();
  }
  oProc.waitForExit();
  sAll += oProc.pipe().readAll();

  // Use starts with, because windows echo will print " from command line arguments
  if (sAll.trim().startsWith("hallo"))
  {
    bRet = true;
  }
  else
  {
    CcConsole::writeLine("CProcessTest::testStdConsoleCommand failed, output not hallo: " + sAll);
  }

  return bRet;
}
