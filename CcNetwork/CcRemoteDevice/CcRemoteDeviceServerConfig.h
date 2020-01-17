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
#include "ServerConfig/CSystem.h"
#include "ServerConfig/CEvents.h"
#include "ServerConfig/CStartup.h"
#include "ServerConfig/CInterfaces.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcVersion.h"
#include "CcUuid.h"

using namespace NRemoteDeviceServerConfig;

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
  CcRemoteDeviceServerConfig(bool bLoadConfig = true);

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceServerConfig();

  void parseJson(CcJsonNode &rJson);
  CcString writeJson();

  void parseBinary(const void* pItem, size_t uiMaxSize);
  size_t writeBinary(void* pItem, size_t uiMaxSize);

  static const char* getDefaultConfig();
  static size_t getDefaultConfigSize();

  virtual void parseAppConfig(CcJsonNode &rNode);
  virtual void writeAppConfig(CcJsonNode& rNode);

  virtual void parseAppConfigBinary(const void* pItem, size_t uiMaxSize);
  virtual void writeAppConfigBinary(void* pItem, size_t uiMaxSize);

  CcVersion   oVersion;
  CcUuid      oVendorId;
  CcUuid      oDeviceId;
  CcString    sVariant;
  uint32      uiSerialNr = 0;
  CcVersion   oSwVersion;
  CcVersion   oHwVersion;

  bool        bDetectable = true;

  CSystem             oSystem;
  CEvents             oEvents;
  CStartup            oStartup;
  CInterfaces         oInterfaces;

  CcJsonDocument      oJsonDocument;
};

#endif // H_CcRemoteDeviceServerConfig_H_
