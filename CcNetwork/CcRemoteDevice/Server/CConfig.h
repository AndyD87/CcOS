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
 * @page      CcConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConfig
 **/
#ifndef H_CcConfig_H_
#define H_CcConfig_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "Server/Config/CSystem.h"
#include "Server/Config/CEvents.h"
#include "Server/Config/CStartup.h"
#include "Server/Config/CInterfaces.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcVersion.h"
#include "CcUuid.h"
#include "CcGlobalStrings.h"

namespace NRemoteDeviceServer
{
/**
 * @brief CcConfig implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CConfig
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
  void writeFile(ESource eSource, const CcString& sPath);
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

  Config::CSystem             oSystem;
  Config::CEvents             oEvents;
  Config::CStartup            oStartup;
  Config::CInterfaces         oInterfaces;

private:
  ESource  m_eSource = ESource::Unknown;
  CcString m_sFilePath;
  const static char c_aBinaryTag[8];
};

}
#endif // H_CcConfig_H_
