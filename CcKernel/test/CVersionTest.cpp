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
 * @brief     Implemtation of class CVersionTest
 */
#include "CVersionTest.h"
#include "CcVersion.h"
#include "CcConsole.h"

CVersionTest::CVersionTest( void )
{
}

CVersionTest::~CVersionTest( void )
{
}

bool CVersionTest::test()
{
  bool bSuccess = true;
  bSuccess &= testVersionSetting();
  bSuccess &= testVersionComparingGreater();
  bSuccess &= testVersionComparingLower();
  if(!bSuccess)
  {
    CcConsole::writeLine("CVersionTest failed");
  }
  return bSuccess;
}

bool CVersionTest::testVersionSetting()
{
  bool bRet = false;
  CcVersion oVersion0(1, 0, 0, 0);
  CcVersion oVersion1(1);
  CcVersion oVersion2("1.0");
  CcVersion oVersion3("1,0,0", ",");
  if (oVersion0 == oVersion1 &&
      oVersion0 == oVersion2 && 
      oVersion0 == oVersion3 )
  {
    bRet = true;
  }
  return bRet;
}

bool CVersionTest::testVersionComparingGreater()
{
  bool bRet = false;
  CcVersion oVersion0(1, 0, 0, 2);
  CcVersion oVersion1(1, 0, 0, 1);
  CcVersion oVersion2("0.9");
  CcVersion oVersion3("1,0,0", ",");
  if (oVersion0 > oVersion1 &&
      oVersion0 > oVersion2 &&
      oVersion0 > oVersion3)
  {
    bRet = true;
  }
  return bRet;
}

bool CVersionTest::testVersionComparingLower()
{
  bool bRet = false;
  CcVersion oVersion0(0, 9, 0, 2);
  CcVersion oVersion1(1, 0, 0, 1);
  CcVersion oVersion2("0.9.1.0");
  CcVersion oVersion3("1,0,0", ",");
  if (oVersion0 < oVersion1 &&
      oVersion0 < oVersion2 &&
      oVersion0 < oVersion3)
  {
    bRet = true;
  }
  return bRet;
}