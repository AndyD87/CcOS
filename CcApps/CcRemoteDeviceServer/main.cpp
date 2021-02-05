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
#include "CcKernel.h"
#include "CcConsole.h"
#include "Types/CcArguments.h"
#include "CcFile.h"
#include "CcGlobalStrings.h"
#include "CcRemoteDeviceServerApp.h"

/**
 * @brief Print help ouput for this application
 */
void printHelp ()
{
  CcConsole::writeLine("Usage: CcRemoteDeviceServer");
}

/**
 * @brief Default application entry point
 * @param iArgc:  Argument count in ppArgv
 * @param ppArgv: Passed arguments from callup
 * @return Exitcode, default 0 if no error occured
 */
int main(int iArgc, char** ppArgv)
{
  int iRet = 0;
  CcArguments oArguments(iArgc, ppArgv);
  CcKernel::initCLI();

  CcString sOutputFile;
  CcString sInputFile;
  CcString sResourceName;
  if (oArguments.size() > 1)
  {
    for (size_t uiArgument = 1; uiArgument < oArguments.size(); uiArgument++)
    {
      if (oArguments[uiArgument] == "-h")
      {
        printHelp();
      }
      else
      {
        printHelp();
      }
    }
  }
  else
  {
    printHelp();
  }
  if (iRet == 0)
  {
    CcRemoteDeviceServerApp oServer(iArgc, ppArgv);
    iRet = oServer.exec().getErrorInt();
  }
  return iRet;
}
