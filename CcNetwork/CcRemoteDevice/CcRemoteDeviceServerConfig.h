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
#include "CcXml/CcXmlConfigFile.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"

class CcRemoteDeviceServerConfig;

/**
 * @brief CcRemoteDeviceServerConfig implementation
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

  class CcRemoteDeviceSHARED CStartup
  {
  public:
    CCDEFINE_EQUAL_OPERATORS(CStartup)
    class CcRemoteDeviceSHARED CCommand
    {
    public:
      CCDEFINE_EQUAL_OPERATORS(CCommand)
      bool bTest;
    };
    class CcRemoteDeviceSHARED CCommands : public CcList<CCommand>
    {
    public:
      CCDEFINE_EQUAL_OPERATORS(CCommands)
    };
    bool bStopOnError = true;
    CCommands oCommands;
  };
  class CEvents
  {
  public:
    class CEvent
    {
    public:
      enum class EType
      {

      };
    inline bool operator==(const CEvent&) { return false; }
    inline bool operator!=(const CEvent&) { return true; }

      EType eType;
      CcString sData;
    };
    CcList<CEvent> oTimeEvents;
  };
  class CSystem
  {
  public:
    class CDevice
    {
    public:
      enum class EType
      {
        Gpio
      };

      EType eType;
      union
      {
        struct
        {
          uint8 uiPort;
          uint8 uiPin;
        } Gpio;
      } Data;
    };
    CcList<CDevice> oDevices;
  };
  class  CWlan
  {
  public:
    CcString sClientSsid;
    CcPassword oClientPassword;
    bool bClientEnabled = true;
    CcString sServerSsid;
    CcPassword oServerPassword;
    bool bServerEnabled = true;
  };

  void init();

  CWlan               oWlan;
  CEvents             oEvents;
  CStartup            oStartup;
  CSystem             oSystem;
  CcHttpServerConfig  oHttpConfig;
};

#endif // H_CcRemoteDeviceServerConfig_H_
