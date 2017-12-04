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
 * @brief     Implemtation of class CTimeTest
 */
#include "CTimeTest.h"
#include "CcDateTime.h"
#include "CcConsole.h"
#include "CcString.h"

CTimeTest::CTimeTest( void )
{
}

CTimeTest::~CTimeTest( void )
{
}

bool CTimeTest::test()
{
  bool bSuccess = true;
  bSuccess &= test1();
  if(!bSuccess)
  {
    CcConsole::writeLine("CTimeTest failed");
  }
  return bSuccess;
}

bool CTimeTest::test1()
{
  bool bRet = true;
  CcDateTime oDateTime;
  oDateTime.setTimestampS(1481218997);
  int16 uiDayInYear   = oDateTime.getDaysSinceYear();
  int32 uiYear        = oDateTime.getYear();
  int16 uiMonth       = oDateTime.getMonth();
  int16 uiDay         = oDateTime.getDay();
  CcDateTime oDateTime2;
  oDateTime2.setTimestampS(0);
  oDateTime2.addUSeconds( 1481218997000000);
  oDateTime2.addMSeconds(-1481218997000);
  oDateTime2.addSeconds ( 1481218997);
  int16 ui2DayInYear  = oDateTime2.getDaysSinceYear();
  int32 ui2Year       = oDateTime2.getYear();
  int16 ui2Month      = oDateTime2.getMonth();
  int16 ui2Day        = oDateTime2.getDay();
  if (uiDayInYear != ui2DayInYear  &&
      uiYear      != ui2Year       &&
      uiMonth     != ui2Month      &&
      uiDay       != ui2Day        )
  {
    bRet = false;
  }
  return bRet;
}
