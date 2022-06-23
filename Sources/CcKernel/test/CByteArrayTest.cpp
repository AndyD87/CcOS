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
 * @brief     Implemtation of class CByteArrayTest
 */
#include "CByteArrayTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"
#include "CcConsole.h"

CByteArrayTest::CByteArrayTest() :
  CcTest("CByteArrayTest")
{
  appendTestMethod("Test converting of Hex strings to binary array", &CByteArrayTest::testStringConversions);
  appendTestMethod("Test find methods", &CByteArrayTest::testFind);
}

CByteArrayTest::~CByteArrayTest()
{
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

bool CByteArrayTest::testFind()
{
  bool bRet = false;
  const char pcTestArray1[] = ("Hallo Welt");
  CcByteArray oTestArray(pcTestArray1, sizeof(pcTestArray1)-1);
  if (oTestArray.find("all", 3) == 1)
  {
    if (oTestArray.find("alle", 4) == SIZE_MAX)
    {
      if (oTestArray.find("Hallo Welt2", 11) == SIZE_MAX)
      {
        if (oTestArray.find("all", 3, 2) == SIZE_MAX)
        {
          if (oTestArray.find("alle", 3, 0) == 1)
          {
            if (oTestArray.find("Welt", 4, 0) == 6 &&
                oTestArray.find("Welt", 4, 0, 9) == SIZE_MAX)
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
