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
 * @brief    Development default CLI-Application for testing new Implementations
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "Types/CcArguments.h"
#include "Types/CcStatus.h"
#include "Types/CcByteArray.h"
#include "Hash/CcCrc32.h"
#include "CcFile.h"
#include "CcVersion.h"

/**
 * @brief Print help ouput for this application
 */
void printHelp()
{
  CcConsole::writeLine("CcHash is a application wich is part of CcOS framework.");
  CcConsole::writeLine("It can be used to create Hash values of different Input types like strings, files and pipe.");
  CcConsole::writeLine("Version: " + CcKernel::getVersion().getVersionString(3));
  CcConsole::writeLine(CcGlobalStrings::Empty);
  CcConsole::writeLine("Usage: CcHash [--algorithm=<algorithm>] file path_to_file");
  CcConsole::writeLine("       CcHash [--algorithm=<algorithm>] string \"String to parse\"");
  CcConsole::writeLine(CcGlobalStrings::Empty);
  CcConsole::writeLine("Algorithms: ");
  CcConsole::writeLine("  Select on of the following algorithms wich should be used for generating hash value. Default Algorithm is CRC");
  CcConsole::writeLine("  This paramater is case insensitive.");
  CcConsole::writeLine("    CRC");
  CcConsole::writeLine("    SHA256");
  CcConsole::writeLine("    MD5");

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

  if (oArguments.size() > 2)
  {
    if (oArguments[1].compareInsensitve("file"))
    {
      CcFile oFile(oArguments[2]);
      if (oFile.exists())
      {
        CcString sCrcString = CcString::fromNumber(oFile.getCrc32().getValueUint32(), 16);
        sCrcString.fillBeginUpToLength('0', 8);
      }
      else
      {
        CcConsole::writeLine("File not found");
        iRet = (int)EStatus::FSFileNotFound;
      }
    }
    else if (oArguments[1].compareInsensitve("string"))
    {
      CcCrc32 oCrc;
      oCrc.generate(oArguments[2]);
      CcString sCrcString = CcString::fromNumber(oCrc.getValueUint32(), 16);
      sCrcString.fillBeginUpToLength('0', 8);
    }
    else
    {
      printHelp();
    }
  }
  else if (oArguments.size() == 2)
  {
    CcFile oFile(oArguments[1]);
    if (oFile.exists())
    {
      CcString sCrcString = CcString::fromNumber(oFile.getCrc32().getValueUint32(), 16);
      sCrcString.fillBeginUpToLength('0', 8);
    }
    else
    {
      CcConsole::writeLine("File not found");
      iRet = (int) EStatus::FSFileNotFound;
    }
  }
  else
  {
    // @todo print help
  }

  return iRet;
}
