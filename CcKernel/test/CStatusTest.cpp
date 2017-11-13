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
 * @brief     Implemtation of class CStatusTest
 */
#include "CStatusTest.h"
#include "CcString.h"
#include "CcConsole.h"
#include "CcStatus.h"

CStatusTest::CStatusTest( void )
{
}

CStatusTest::~CStatusTest( void )
{
}

bool CStatusTest::test()
{
  bool bSuccess = true;
  bSuccess &= testBoolValues();
  if(!bSuccess)
  {
    CcConsole::writeLine("CStatusTest failed");
  }
  return bSuccess;
}

bool CStatusTest::testBoolValues()
{
  bool bRet = false;
  CcStatus oStatus;
  // Status must be default true;
  if (oStatus)
  {
    oStatus.setError(EStatus::Error);
    if (!oStatus)
    {
      oStatus = true;
      if (oStatus)
      {
        oStatus = false;
        if (!oStatus)
        {
          bool bTest = true;
          bTest = oStatus;
          if (!bTest)
          {
            oStatus.setError(EStatus::AllOk);
            bTest = false;
            bTest = oStatus;
            if (bTest)
            {
              bRet = true;
            }
          }
        }
      }
    }
  }
  return bRet;
}
