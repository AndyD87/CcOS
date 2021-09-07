/*
 * This file is part of CcConfig.
 *
 * CcConfig is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConfig is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConfig.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConfig
 **/
#pragma once

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "Server/Config/CSystem.h"
#include "Server/Config/CEvents.h"
#include "Server/Config/CStartup.h"
#include "Server/Config/CInterfaces.h"
#include "Server/Config/CUsers.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcVersion.h"
#include "CcUuid.h"
#include "CcGlobalStrings.h"

namespace NRemoteDevice
{
namespace Server
{

/**
 * @brief CcConfig implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CConfig
{
public:
  /**
   * @brief Enum for config source and type
   */
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
   * @param bLoadConfig: Try to load from EEPROM, false for skip
   */
  CConfig(bool bLoadConfig = true);

  /**
   * @brief Destructor
   */
  virtual ~CConfig();

  /**
   * @brief Load default values from EEPROM
   * @param bLoadDefault: Try to load from EEPROM, false for skip
   * @return AllOk if no error
   */
  CcStatus init(bool bLoadDefault = true);

  /**
   * @brief Call this method to write configuration in same way as it was read, or
   *        define an other format by setting an other value than ESource::Unknown.
   */
  void write(ESource eSource = ESource::Unknown, const CcString& sPath = CcGlobalStrings::Empty);

  /**
   * @brief Write data out to defined target file or eeprom
   * @param eSource:  Target output file or eeprom
   * @param oData:    Data to write
   */
  void writeData(ESource eSource, CcByteArray& oData);

  /**
   * @brief Read configuration from file.
   *        Type will be parsed automaticaly
   * @param sPath: Path to file or eeprom
   */
  void read(const CcString& sPath);

  /**
   * @brief Read binary data and determine type automatic.
   * @param oData: Target buffer to parse
   */
  void readData(const CcByteArray& oData);

  /**
   * @brief Check if config was successfully read from any source
   * @return true if Config was parsed successfully from any source
   */
  bool isRead(){ return m_eSource != ESource::Unknown; }

  /**
   * @brief Get default configurationf from ressources
   * @return Path to \0 terminated char buffer.
   */
  static const char* getDefaultConfig();

  //! @return Get size of default config at getDefaultConfig()
  static size_t getDefaultConfigSize();

  /**
   * @brief Application can inherit this configuration
   *        and parse application config by implementing this method.
   * @param rNode: Application node from config
   */
  virtual void parseAppConfig(CcJsonNode &rNode);

  /**
   * @brief Application can inherit this configuration
   *        and add application config by implementing this method.
   * @param rNode: Application node add config.
   */
  virtual void writeAppConfig(CcJsonNode& rNode);

  /**
   * @brief Application can inherit this configuration
   *        and parse binary application config by implementing this method.
   * @param pItem:      Application node in binary config.
   * @param uiMaxSize:  Size left to read
   * @return Pointer to next item in config.
   */
  virtual const CcConfigBinary::CItem *parseAppConfigBinary(const void* pItem, size_t uiMaxSize);

  /**
   * @brief Write application config to binary stream
   * @param pStream:  Target io stream to write binary config to.
   * @return Number of bytes written.
   */
  virtual size_t writeAppConfigBinary(IIo &pStream);

private:
  void writeEeprom(ESource eSource);
  void writeFile(ESource eSource, const CcString& sPath);

  /**
   * @brief Parse json node to get client config
   * @param rJson: Json to get config from
   */
  void parseJson(CcJsonNode& rJson);

  /**
   * @brief Generate json document and write to storage.
   * @param pStream: Stream to write binary output of json data
   */
  void writeJson(IIo& pStream);

  /**
   * @brief Write credentials to binary config stream
   * @param pStream: Target stream
   * @return Number of bytes written
   */
  size_t writeBinary(IIo& pStream);

  /**
   * @brief Parse binary config file
   * @param pItem:      First item to search for this config
   * @param uiMaxSize:  Max size of binary left to read
   * @return True if parsing succeeded
   */
  bool parseBinary(const void* pItem, size_t uiMaxSize);

public:
  CcVersion           oVersion;       //!< Version of this application
  CcUuid              oVendorId;      //!< Vendor as uuid
  CcUuid              oDeviceId;      //!< Device as uuid
  CcString            sVariant;       //!< Variant as string
  uint32              uiSerialNr = 0; //!< Production number of device
  CcVersion           oSwVersion;     //!< Current Software version
  CcVersion           oHwVersion;     //!< Current Hardware version

  CcSocketAddressInfo oAddressInfo;   //!< Local bind address
  bool        bDetectable = true;     //!< True if udp broadcast will be setup
                                      //!< and answered.

  Config::CSystem             oSystem;      //!< System configuration
  Config::CEvents             oEvents;      //!< Events to react on
  Config::CStartup            oStartup;     //!< Startup commands wenn application starts
  Config::CInterfaces         oInterfaces;  //!< Interfaces used within application.
  Config::CUsers              oUsers;       //!< Users for accessing remote device

private:
  ESource  m_eSource = ESource::Unknown;
  CcString m_sFilePath;
  const static char c_aBinaryTag[8];
};

}
}
