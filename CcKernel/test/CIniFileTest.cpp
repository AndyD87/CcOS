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
 * @brief     Implemtation of class CIniFileTest
 */
#include "CIniFileTest.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcKernel.h"
#include "CcStringStream.h"
#include "Documents/CcIniFile.h"

const char pExampleIni[] =
    "#  Global Comment 0\r\n"
    " # Global Comment 1\r\n"
    " ; Global Comment 2\r\n"
    "[Unit]\r\n"
    "Description=TestService\r\n"
    "\r\n"
    "# Comment 1\r\n"
    "[Service]\r\n"
    "Type=simple\r\n"
    "ExecStart=/opt/bin/TestService\r\n"
    "WorkingDirectory=/opt/bin\r\n"
    "\r\n"
    "Restart=on-failure\r\n"
    "RestartSec=10\r\n"
    "KillMode=process\r\n"
    "\r\n"
    " [Install] \r\n"
    "WantedBy=multi-user.target\r\n";

CIniFileTest::CIniFileTest() :
  CcTest("CIniFileTest")
{
  appendTestMethod("Test read test string", &CIniFileTest::testFileExample);
}

CIniFileTest::~CIniFileTest()
{
}

bool CIniFileTest::testFileExample()
{
  bool bRet = false;
  CcIniFile oFile;
  CcString oStrig(pExampleIni);
  CcStringStream oSteam(oStrig);
  bRet = oFile.readStream(oSteam);
  return bRet;
}
