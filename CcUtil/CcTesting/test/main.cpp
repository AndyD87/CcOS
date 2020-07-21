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
 * @brief    Entry point for Application
 */

#include "CcBase.h"
#include "CcTestFramework.h"
#include "CTestModuleTest.h"
#include "CTestTestUtility.h"
#include "CcTestUtility.h"
#include "CcConsole.h"

int main(int argc, char **argv)
{
  int iReturn = 0;
  bool bDoTesting = true;
  if(argc > 1)
  {
    bDoTesting = false;
    if(CcString(argv[1]) == "run")
    {
      if(argc > 2)
      {
        if(CcString(argv[2]) == "generateAndVerifyFile")
        {
          if(argc > 4)
          {
            CcString sFile(argv[3]);
            CcString sSize(argv[4]);
            bool bSizeOk = false;
            uint64 uiSize = sSize.toUint64(&bSizeOk);
            if(bSizeOk)
            {
              CcStatus oStatus = CcTestUtility::generateAndVerifyFile(sFile, uiSize);
              if(!oStatus)
              {
                CcConsole::writeLine("generateAndVerifyFile failed with:" +
                                     CcString::fromInt(oStatus.getErrorInt()));
              }
              iReturn = oStatus.getErrorInt();
            }
          }
          else
          {
            CcConsole::writeLine("Invalid Paramter count");
            iReturn = -1;
          }
        }
        else
        {
          CcConsole::writeLine("Unknown");
          iReturn = -1;
        }
      }
      else
      {
        CcConsole::writeLine("Invalid Paramter count");
        iReturn = -1;
      }
    }
    else if(CcString(argv[1]) == "-h" ||
            CcString(argv[1]) == "/h")
    {
      CcConsole::writeLine("generateAndVerifyFile <File> <Size>");
    }
    else
    {
      CcConsole::writeLine("Unknown");
      iReturn = -1;
    }
  }
  if(bDoTesting)
  {
    CcTestFramework::init(argc, argv);

    CcTestFramework_addTest(CTestModuleTest);
    CcTestFramework_addTest(CTestTestUtility);

    CcTestFramework::runTests();
    iReturn = CcTestFramework::deinit();
  }
  return iReturn;
}
