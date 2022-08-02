/*
 * This file is part of CcDhcpServerApp.
 *
 * CcDhcpServerApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcDhcpServerApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcDhcpServerApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcDhcpServerApp
 */
#include "CcDhcpServerApp.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "CcDhcpGlobals.h"

CcDhcpServerApp::CcDhcpServerApp(int argc, char** argv) :
  CcService(CcDhcpGlobals::Strings::ServerName, CCNEW_INLINE(CcDhcpServer)),
  m_oArguments({}, argc, argv)
{
  parseArgs();
}

CcDhcpServerApp::~CcDhcpServerApp()
{
}

void CcDhcpServerApp::parseArgs()
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
        m_bCli = true;
      }
      else
      {
        printHelp();
      }
    }
  }
}

void CcDhcpServerApp::printHelp()
{
  CcKernel::initCLI();
  CcConsole::writeLine("Usage: ");
  CcConsole::writeLine("Options:");
  CcConsole::writeLine("  -h          Print this help message");
  CcConsole::writeLine("  --console   Run not in service context, keep cli open");
}
