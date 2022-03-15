/*
 * This file is part of CcOSModuleLoaderApp.
 *
 * CcOSModuleLoaderApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOSModuleLoaderApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOSModuleLoaderApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcOSModuleLoaderApp
 */
#include "CcOSModuleLoaderApp.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "CcFile.h"

class CcOSModuleLoaderApp::CPrivate
{
public:
  CPrivate()
  {
  }

  ~CPrivate()
  {
  }
};

CcOSModuleLoaderApp::CcOSModuleLoaderApp(int argc, char** argv) :
  CcApp("CcOSModuleLoaderApp"),
  m_oArguments(argc, argv)
{
  CCNEW(m_pPrivate, CPrivate);
  parseArgs();
}

CcOSModuleLoaderApp::~CcOSModuleLoaderApp()
{
  CCDELETE(m_pPrivate);
}


void CcOSModuleLoaderApp::run()
{
  if (m_sFileToLoad.length() > 0)
  {
    CcFile cFile(m_sFileToLoad);
    CcConsole::writeLine("Start loading");
    CcString sPath = cFile.getAbsolutePath(m_sFileToLoad);
    if (cFile.isFile())
    {
      this->setExitCode(CcKernel::loadModule(sPath));
    }
    else
    {
      CcConsole::writeLine("File not found");
      this->setExitCode(EStatus::FSFileNotFound);
    }
    
  }
  else
  {
    CcConsole::writeLine("No file set");
    this->setExitCode(EStatus::FSFileNotFound);
  }
}

void CcOSModuleLoaderApp::parseArgs()
{
  if (m_oArguments.size() > 1)
  {
    for (size_t uiArgument = 1; uiArgument < m_oArguments.size(); uiArgument++)
    {
      if (m_oArguments[uiArgument] == "-h")
      {
        printHelp();
      }
      else
      {
        m_sFileToLoad = m_oArguments[uiArgument];
      }
    }
  }
}

void CcOSModuleLoaderApp::printHelp()
{
  CcConsole::writeLine("Usage: " + getName() + " FileToLoad");
}
