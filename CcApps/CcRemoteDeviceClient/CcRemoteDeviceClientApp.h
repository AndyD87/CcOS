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
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceClientApp
 **/
#ifndef H_CcRemoteDeviceClientApp_H_
#define H_CcRemoteDeviceClientApp_H_

#include "CcGuiApplication.h"
#include "CcArguments.h"

/**
 * @brief CcRemoteDeviceClientApp implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceClientApp : public CcGuiApplication
{
public:
  /**
   * @brief Initialize app with arguments from command line
   * @param argc: Number of arguments
   * @param argv: Arguments as array
   */
  CcRemoteDeviceClientApp(int argc, char** argv);

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceClientApp();

  /**
   * @brief Disable gui, it has to be done befor start
   */
  void setNoGui()
  { m_bNoGui = true; }

  virtual void run() override;
  virtual void onWindowLoaded() override;

private:
  class CPrivate;

  void parseArgs();
  void printHelp();
private:
  CPrivate*             m_pPrivate = nullptr;  //!< Private data for applicaton.
  CcArguments           m_oArguments;
  bool                  m_bNoGui = false;
};

#endif // H_CcRemoteDeviceClientApp_H_
