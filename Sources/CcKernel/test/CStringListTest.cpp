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
 * @brief     Implemtation of class CStringListTest
 */
#include "CStringListTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"
#include "CcConsole.h"
#include "CcGlobalStrings.h"

CStringListTest::CStringListTest() :
  CcTest("CStringListTest")
{
  appendTestMethod("Test remove empty lines", &CStringListTest::removeEmpty);
  appendTestMethod("Test split functions", &CStringListTest::splitTest);
}

CStringListTest::~CStringListTest()
{
}

bool CStringListTest::removeEmpty()
{
  bool bSuccess = false;
  CcStringList oList;
  oList.append("Hallo");
  oList.append(CcGlobalStrings::Empty);
  oList.append(CcGlobalStrings::Space);
  oList.append(CcGlobalStrings::Empty);
  oList.append("du");
  oList.append(CcGlobalStrings::Empty);
  oList.append(CcGlobalStrings::Space);
  oList.append(CcGlobalStrings::Empty);
  oList.append("da");
  oList.append("");
  if (oList.size() == 10)
  {
    oList.removeEmpty();
    if (oList.size() == 5)
    {
      CcString sLine = oList.collapse(CcGlobalStrings::Empty);
      if (sLine == "Hallo du da")
      {
        bSuccess = true;
      }
    }
  }
  return bSuccess;
}

bool CStringListTest::splitTest()
{
  bool bSuccess = false;
  CcString sSplittingString = "Hallo  du da  ";
  CcStringList sSplitWEmpty = sSplittingString.split(CcGlobalStrings::Seperators::Space);
  CcStringList sSplitWOEmpty = sSplittingString.split(CcGlobalStrings::Seperators::Space, false);
  if (sSplitWEmpty.size() == 5 &&
      sSplitWOEmpty.size() == 3 &&
      sSplitWEmpty.removeEmpty().size() == 3)
  {
    bSuccess = true;
  }
  return bSuccess;
}
