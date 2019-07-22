/*
 * This file is part of CcRemoteDeviceConfig.
 *
 * CcRemoteDeviceConfig is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceConfig is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceConfig.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcRemoteDeviceConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceConfig
 **/
#ifndef CcRemoteDeviceConfig_H_
#define CcRemoteDeviceConfig_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcXml/CcXmlConfigFile.h"
#include "CcString.h"
#include "CcList.h"

/**
 * @brief CcRemoteDeviceConfig impelmentation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceConfig : public CcXmlConfigFile
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceConfig();

  /**
   * @brief Destructor
   */
  ~CcRemoteDeviceConfig();

  bool loadConfig(const CcString& sPath = "");
  
  class CServer
  {
  public:
    class CStartup
    {
    public:
      class CCommand
      {
        
      };
      bool bStopOnError = true;
      CcList<CCommand> oCommands;
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

        EType eType;
        CcString sData;
      };
      CcList<CEvent> oTimeEvents;
    };
    class CRemoteControl
    {
    public:
      uint16_t uiPort = CcRemoteDeviceGlobals::Defaults::Port;
      CcString sSslCertificatePath;
      CcString sSslKeyPath;
    };
    CEvents        oEvents;
    CStartup       oStartup;
    CRemoteControl oRemoteControl;
  };
  class CClient
  {
  public:
  };

public:
  CServer* getServerConfig();
  CClient* getClienConfig();

private:
  CServer* m_pServer = nullptr;
  CClient* m_pClient = nullptr;
};

#endif /* CcRemoteDeviceConfig_H_ */
