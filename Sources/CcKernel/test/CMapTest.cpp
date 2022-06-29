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
 * @brief     Implemtation of class CMapTest
 */
#include "CMapTest.h"
#include "CcMapCommon.h"

CMapTest::CMapTest() : 
  CcTest("CMapTest")
{
  appendTestMethod("Test Set existing value", &CMapTest::testSet);
}

CMapTest::~CMapTest()
{
}

bool CMapTest::testSet()
{
  bool bRet = false;
  CcStringMap oMap;
  oMap.set("test", "1");
  if (oMap.size() == 1)
  {
    oMap.set("test", "2");
    if (oMap.size() == 1 &&
        oMap.getValue("test") == "2")
    {
      bRet = true;
    }
  }
  return bRet;
}
