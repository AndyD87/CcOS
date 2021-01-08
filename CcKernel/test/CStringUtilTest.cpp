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
 * @brief     Implemtation of class CStringUtilTest
 */
#include "CStringUtilTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"
#include "CcConsole.h"

CStringUtilTest::CStringUtilTest() :
  CcTest("CStringUtilTest")
{
  appendTestMethod("Test for argument searching.", &CStringUtilTest::testFindArgument);
}

CStringUtilTest::~CStringUtilTest()
{
}

bool CStringUtilTest::testFindArgument()
{
  bool bSuccess = false;
  CcString sTestArg1(" arg1 ");
  CcString sTestArg2(" arg 2 ");
  CcString sTestArg3(" \"arg 3 ");
  CcString sTestArg4(" \"arg 4\" ");
  CcString sTestArg5(" \"arg\\\" 5\" ");
  CcString sTestArg6(" \"arg\\\" \" \"6\" ");
  size_t uiPos = 0;
  CcString sResult = CcStringUtil::findArgument(sTestArg1, uiPos);
  if (sResult == "arg1" && uiPos == 5)
  {
    uiPos = 0;
    sResult = CcStringUtil::findArgument(sTestArg2, uiPos);
    if (sResult == "arg" && uiPos == 4)
    {
      sResult = CcStringUtil::findArgument(sTestArg2, uiPos);
      if (sResult == "2" && uiPos == 6)
      {
        uiPos = 0;
        sResult = CcStringUtil::findArgument(sTestArg3, uiPos);
        if (sResult == "arg 3 " && uiPos == sTestArg3.size())
        {
          uiPos = 0;
          sResult = CcStringUtil::findArgument(sTestArg4, uiPos);
          if (sResult == "arg 4" && uiPos == 8)
          {
            uiPos = 0;
            sResult = CcStringUtil::findArgument(sTestArg5, uiPos);
            if (sResult == "arg\" 5" && uiPos == 10)
            {
              uiPos = 0;
              sResult = CcStringUtil::findArgument(sTestArg6, uiPos);
              if (sResult == "arg\" " && uiPos == 9)
              {
                sResult = CcStringUtil::findArgument(sTestArg6, uiPos);
                if (sResult == "6" && uiPos == 13)
                {
                  bSuccess = true;
                }
              }
            }
          }
        }
      }
    }
  }
  return bSuccess;
}
