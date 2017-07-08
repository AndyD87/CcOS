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
 * @brief     Implemtation of class CByteArrayTest
 */
#include "CByteArrayTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"

CByteArrayTest::CByteArrayTest( void )
{
}

CByteArrayTest::~CByteArrayTest( void )
{
}

bool CByteArrayTest::test()
{
  bool bSuccess = true;
  bSuccess &= testStringConversions();
  return bSuccess;
}

bool CByteArrayTest::testStringConversions()
{
  bool bRet = false;
  const char pcTestArray1[] = ("\17\103\103\0\1\2\3\16\27\100\377");
  CcByteArray oTestArray(pcTestArray1, sizeof(pcTestArray1));
  CcByteArray oTestHexString;
  oTestHexString.setHexString(oTestArray.getHexString());
  if (oTestArray == oTestHexString)
  {
    bRet = true;
  }
  return bRet;
}
