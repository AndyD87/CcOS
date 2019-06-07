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
 * @brief    Development default CLI-Application for testing new Implementations
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "Types/CcArguments.h"

void printHelp ()
{
  CcConsole::writeLine("Usage: CcOSRessource");
}

void run(const CcString& sOutputFile, const CcString& sInputFile, const CcString& sRessourceName)
{
  CCUNUSED(sOutputFile);
  CCUNUSED(sInputFile);
  CCUNUSED(sRessourceName);
}

int main(int argc, char **argv)
{
  int iRet = 0;
  CcArguments oArguments(argc, argv);
  CcKernel::initCLI();

  CcString sOutputFile;
  CcString sInputFile;
  CcString sRessourceName;
  if (oArguments.size() > 1)
  {
    for (size_t uiArgument = 0; uiArgument < oArguments.size(); uiArgument++)
    {
      if (oArguments[uiArgument] == "-h")
      {
        printHelp();
      }
      else if (oArguments[uiArgument] == "-o")
      {
        if (oArguments.size() >= uiArgument + 1)
        {
          sOutputFile = oArguments[uiArgument+1];
          uiArgument++;
        }
      }
      else if (oArguments[uiArgument] == "-i")
      {
        if (oArguments.size() >= uiArgument + 1)
        {
          sInputFile = oArguments[uiArgument + 1];
          uiArgument++;
        }
      }
      else if (oArguments[uiArgument] == "-n")
      {
        if (oArguments.size() >= uiArgument + 1)
        {
          sRessourceName = oArguments[uiArgument + 1];
          uiArgument++;
        }
      }
    }
  }
  else
  {
    printHelp();
  }

  return iRet;
}
