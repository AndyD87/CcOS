/*
 * This file is part of CcRemoteDeviceServerConfig.
 *
 * CcRemoteDeviceServerConfig is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceServerConfig is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceServerConfig.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcRemoteDeviceServerConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceServerConfig
 **/
#ifndef H_CcRemoteDeviceServerConfig_H_
#define H_CcRemoteDeviceServerConfig_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "ServerConfig/CWlan.h"
#include "ServerConfig/CSystem.h"
#include "ServerConfig/CEvents.h"
#include "ServerConfig/CStartup.h"

using namespace NsRemoteDeviceServerConfig;

/**
 * @brief CcRemoteDeviceServerConfig implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceServerConfig
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceServerConfig();

  /**
   * @brief Destructor
   */
  ~CcRemoteDeviceServerConfig();

  void init();

  CWlan               oWlan;
  CEvents             oEvents;
  CStartup            oStartup;
  CSystem             oSystem;
  CcHttpServerConfig  oHttpConfig;
};

#endif // H_CcRemoteDeviceServerConfig_H_
