/*
 * This file is part of CcRemoteDeviceClientApp.
 *
 * CcRemoteDeviceClientApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceClientApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceClientApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceClientApp
 */
#include "CcRemoteDeviceClientApp.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcKernel.h"
#include "CcConsole.h"

class CcRemoteDeviceClientApp::CPrivate
{
public:
  CPrivate()
  {
  }

  ~CPrivate()
  {
  }
};

CcRemoteDeviceClientApp::CcRemoteDeviceClientApp(int argc, char** argv) :
  CcGuiApplication(CcRemoteDeviceGlobals::Names::ClientAppName),
  m_oArguments({}, argc, argv)
{
  CCNEW(m_pPrivate, CPrivate);
  parseArgs();
}

CcRemoteDeviceClientApp::~CcRemoteDeviceClientApp()
{
  CCDELETE(m_pPrivate);
}


void CcRemoteDeviceClientApp::run()
{
  if (m_bNoGui)
  {
    // Start cli
    CcKernel::initCLI();
    CcConsole::writeLine(CcRemoteDeviceGlobals::Names::ClientAppName + " started");
  }
  else
  {
    // Start gui
    CcKernel::deinitCLI();
    CcGuiApplication::run();
  }
}

void CcRemoteDeviceClientApp::onWindowLoaded()
{
  // Gui started, setup window.
}

void CcRemoteDeviceClientApp::parseArgs()
{
  if (m_oArguments.size() > 1)
  {
    for (size_t uiArgument = 1; uiArgument < m_oArguments.size(); uiArgument++)
    {
      if (m_oArguments[uiArgument] == "-h")
      {
        printHelp();
      }
      else if (m_oArguments[uiArgument] == "--console")
      {
        m_bNoGui = true;
      }
      else
      {
        printHelp();
      }
    }
  }
}

void CcRemoteDeviceClientApp::printHelp()
{
  CcKernel::initCLI();
  CcConsole::writeLine("Usage: " + CcRemoteDeviceGlobals::Names::ClientAppName);
}
