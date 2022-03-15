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
#include "CcOSBuildConfig.h"

/**
 * @brief Print help ouput for this application
 */
void printHelp ()
{
  CcConsole::writeLine("Usage: CcOSBuildConfig config2cmake [Configuration] [TargetCmake]");
  CcConsole::writeLine("Example:");
  CcConsole::writeLine("  - Generation CcOS build configuration like this");
  CcConsole::writeLine("    ./CcOSBuildConfigApp config2cmake ../../../CMakeConfig/CcOSBuildConfig.xml ../../../CMakeConfig/CcOSBuildConfig.cmake");
  
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
    if (oArguments[1] == "config2cmake")
    {
      if (oArguments.size() > 3)
      {
        CcString sFileSource = oArguments[2];
        CcString sFileTarget = oArguments[3];
        CcOSBuildConfig m_oConfig;
        if (m_oConfig.loadConfigFile(sFileSource))
        {
          m_oConfig.writeAllProjects();
          if (!m_oConfig.writeCmakeDefines(sFileTarget))
          {
            CcConsole::writeLine("Failed to write CMakeFile.");
          }
          CcConsole::writeLine("Configuration file loaded.");
        }
        else
        {
          CcConsole::writeLine("Configuration file loading failed.");
        }
      }
      else
      {
        CcConsole::writeLine("Not enough parameters for config2cmake");
      }
    }
  }
  else
  {
    printHelp();
  }

  return iRet;
}
