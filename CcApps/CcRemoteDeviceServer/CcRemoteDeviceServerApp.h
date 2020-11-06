/*
 * This file is part of CcRemoteDeviceServerApp.
 *
 * CcRemoteDeviceServerApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceServerApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceServerApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcRemoteDeviceServerApp
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceServerApp
 **/
#ifndef H_CcRemoteDeviceServerApp_H_
#define H_CcRemoteDeviceServerApp_H_

#include "CcRemoteDeviceServer.h"
#include "CcArguments.h"

class IGpioPort;

/**
 * @brief CcRemoteDeviceServerApp implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceServerApp : public CcRemoteDeviceServer
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceServerApp(int iArgs, char** pArgv);

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceServerApp();

private:
  CcArguments           m_oArguments;
#if defined(LINUX) || defined(WINDOWS)
  IGpioPort* pSimulation = nullptr;
#endif
};

#endif // H_CcRemoteDeviceServerApp_H_
