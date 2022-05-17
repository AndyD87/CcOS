/*
 * This file is part of CcRemoteDeviceClientApp.
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
 * @par       Language   C++ ANSI V3
 * @brief     Class CcDhcpServerApp
 **/
#pragma once

#include "CcArguments.h"
#include "CcDhcpServer.h"
#include "CcService.h"

/**
 * @brief CcDhcpServerApp implementation
 *        Main class wich is loaded to start Application.
 */
class CcDhcpServerApp : public CcService
{
public:
  /**
   * @brief Initialize app with arguments from command line
   * @param argc: Number of arguments
   * @param argv: Arguments as array
   */
  CcDhcpServerApp(int argc, char** argv);

  /**
   * @brief Destructor
   */
  virtual ~CcDhcpServerApp();

private:
  void parseArgs();
  void printHelp();
private:
  CcArguments   m_oArguments;
  bool          m_bCli = false;
};
