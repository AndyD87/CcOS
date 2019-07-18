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
#ifndef CcRemoteDeviceServerConfig_H_
#define CcRemoteDeviceServerConfig_H_

#include "CcRemoteDevice.h"
#include "CcXml/CcXmlConfigFile.h"

/**
 * @brief CcRemoteDeviceServerConfig impelmentation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceServerConfig : public CcXmlConfigFile
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

  bool loadConfig(const CcString& sPath = "");

};

#endif /* CcRemoteDeviceServerConfig_H_ */
