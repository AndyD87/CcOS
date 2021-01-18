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
 * @brief    Entry point for Application
 */

#include "CcBase.h"
#include "CcTestFramework.h"
#include "CTestModuleTest.h"
#include "CTestTestUtility.h"
#include "CcTestUtility.h"
#include "CcConsole.h"
#include "CcFile.h"
#include "IProgressReceiver.h"
#include "CcKernel.h"

class CProgress : public IProgressReceiver
{
public:
  virtual ~CProgress()
  {
  }
  virtual void update(uint64 uiValue, uint64 uiFrom) override
  {
    CcConsole::writeLine("Progress: " + CcString::fromNumber(uiValue) + " from " + CcString::fromNumber(uiFrom));
    if(uiValue >= uiFrom)
      CcConsole::writeLine(" finished");
  }
};

/**
 * @brief Default application entry point
 * @param iArgc:  Argument count in ppArgv
 * @param ppArgv: Passed arguments from callup
 * @return Exitcode, default 0 if no error occured
 */
int main(int iArgc, char** ppArgv)
{
  int iReturn = 0;
  bool bDoTesting = true;
  if(iArgc > 1)
  {
    bDoTesting = false;
    if(CcString(ppArgv[1]) == "run")
    {
      if(iArgc > 2)
      {
        if(CcString(ppArgv[2]) == "generateAndVerifyFile")
        {
          if(iArgc > 4)
          {
            CcString sFile(ppArgv[3]);
            CcString sSize(ppArgv[4]);
            bool bSizeOk = false;
            uint64 uiSize = sSize.toUint64(&bSizeOk);
            if(bSizeOk)
            {
              CProgress oProgress;
              CcStatus oStatus = CcTestUtility::generateAndVerifyFile(sFile, uiSize, 0, &oProgress);
              if(iArgc > 5 && CcString(ppArgv[5]) == "keep")
              {
                CcConsole::writeLine("Keep file and do not delete");
              }
              else
              {
                oStatus = CcFile::remove(sFile);
              }
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
        else if(CcString(ppArgv[2]) == "exitInstant")
        {
          if(iArgc > 3)
          {
            CcString sExitCode(ppArgv[3]);
            bool bSizeOk;
            iReturn = sExitCode.toInt32(&bSizeOk);
            if(!bSizeOk)
            {
              CcConsole::writeLine("Invalid Paramter <ExitCode>");
              iReturn = -1;
            }
          }
          else
          {
            CcConsole::writeLine("Invalid Paramter count");
            iReturn = -1;
          }
        }
        else if(CcString(ppArgv[2]) == "exitTimed")
        {
          if(iArgc > 4)
          {
            CcString sExitCode(ppArgv[3]);
            CcString sExitTimer(ppArgv[4]);
            bool bSizeOk;
            iReturn = sExitCode.toInt32(&bSizeOk);
            if(!bSizeOk)
            {
              CcConsole::writeLine("Invalid Paramter <ExitCode>");
              iReturn = -1;
            }
            else
            {
              uint32 iTimer = sExitTimer.toUint32(&bSizeOk);
              if(!bSizeOk)
              {
                CcConsole::writeLine("Invalid Paramter <Seconds>");
                iReturn = -1;
              }
              else
              {
                CcKernel::sleep(1000 * iTimer);
              }
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
    else if(CcString(ppArgv[1]) == "-h" ||
            CcString(ppArgv[1]) == "/h")
    {
      CcConsole::writeLine(" run generateAndVerifyFile <File> <Size> [keep]");
      CcConsole::writeLine(" run exitInstant           <ExitCode>");
      CcConsole::writeLine(" run exitTimed             <ExitCode> <Seconds>");
    }
    else
    {
      CcConsole::writeLine("Unknown");
      iReturn = -1;
    }
  }
  if(bDoTesting)
  {
    CcTestFramework::init(iArgc, ppArgv);

    CcTestFramework_addTest(CTestModuleTest);
    CcTestFramework_addTest(CTestTestUtility);

    CcTestFramework::runTests();
    iReturn = CcTestFramework::deinit();
  }
  return iReturn;
}
