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
#include "CcGlobalStrings.h"

using namespace NRemoteDeviceServerConfig;

/**
 * @brief CcRemoteDeviceServerConfig implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceServerConfig
{
public:
  enum class ESource
  {
    Unknown = 0,
    FileJson,
    FileBinary,
    EepromJson,
    EepromBinary,
  };

  /**
   * @brief Constructor
   */
  CcRemoteDeviceServerConfig(bool bLoadConfig = true);

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceServerConfig();

  /**
   * @brief Call this method to write configuration in same way as it was read, or
   *        define an other format by setting an other value than ESource::Unknown.
   */
  void write(ESource eSource = ESource::Unknown, const CcString &sPath = CcGlobalStrings::Empty);
  void writeData(ESource eSource, CcByteArray& oData);

  void read(const CcString& sPath);
  void readData(const CcByteArray& oData);

  /**
   * @brief Check if config was successfully read from any source
   * @return true if Config was parsed successfully from any source
   */
  bool isRead(){ return m_eSource != ESource::Unknown; }

  static const char* getDefaultConfig();
  static size_t getDefaultConfigSize();

  virtual void parseAppConfig(CcJsonNode &rNode);
  virtual void writeAppConfig(CcJsonNode& rNode);

  virtual const CcConfigBinary::CItem *parseAppConfigBinary(const void* pItem, size_t uiMaxSize);
  virtual size_t writeAppConfigBinary(IIo &pStream);

private:
  void loadJsonFile(const CcString& sPath);
  void loadBinaryFile(const CcString& sPath);
  void writeEeprom(ESource eSource);
  void writeFile(ESource eSource, const CcString &sPath);
  void parseJson(CcJsonNode& rJson);
  void writeJson(IIo& pStream);
  size_t writeBinary(IIo& pStream);

  bool parseBinary(const void* pItem, size_t uiMaxSize);

public:
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

private:
  ESource  m_eSource = ESource::Unknown;
  CcString m_sFilePath;
  const static char c_aBinaryTag[8];
};

#endif // H_CcRemoteDeviceServerConfig_H_
