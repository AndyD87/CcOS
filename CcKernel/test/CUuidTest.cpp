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
 * @brief     Implemtation of class CUuidTest
 */
#include "CUuidTest.h"
#include "CcUuid.h"
#include "CcConsole.h"

CUuidTest::CUuidTest() :
  CcTest("CUuidTest")
{
  appendTestMethod("Test common conversions", &CUuidTest::test1);
}

bool CUuidTest::test1()
{
  bool bRet = false;
  const CcString sUuid = "{DEC724E5-E5E5-494B-8E55-26871E87D4AE}";
  CcUuid oUuid1(sUuid);
  CcUuid oUuid2;
  if (oUuid2.setUuid(sUuid))
  {
    CcString sUuidReturned = oUuid2.getUuidString();
    if (sUuidReturned.compare(sUuid, ESensitivity::CaseInsensitiv) &&
      oUuid1 == oUuid2)
    {
      bRet = true;
    }
  }
  return bRet;
}
