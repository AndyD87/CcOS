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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CProcessTest
 */
#include "CProcessTest.h"
#include "CcProcess.h"
#include "CcIODevice.h"
#include "CcByteArray.h"
#include "CcConsole.h"

CProcessTest::CProcessTest( void )
{
}

CProcessTest::~CProcessTest( void )
{
}

bool CProcessTest::test()
{
  bool bSuccess = true;
  bSuccess &= testStdConsoleCommand();
  if(!bSuccess)
  {
    CcConsole::writeLine("CProcessTest failed");
  }
  return bSuccess;
}

bool CProcessTest::testStdConsoleCommand()
{
  bool bRet = false;
#ifdef WIN32
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

  if (sAll.trim() == "hallo")
  {
    bRet = true;
  }

  return bRet;
}
