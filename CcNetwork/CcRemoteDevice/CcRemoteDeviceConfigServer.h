/*
 * This file is part of CcRemoteDeviceConfigServer.
 *
 * CcRemoteDeviceConfigServer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceConfigServer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceConfigServer.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcRemoteDeviceConfigServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceConfigServer
 **/
#ifndef CcRemoteDeviceConfigServer_H_
#define CcRemoteDeviceConfigServer_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcXml/CcXmlConfigFile.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"

/**
 * @brief CcRemoteDeviceConfigServer impelmentation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceConfigServer : public CcXmlConfigFile
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceConfigServer();

  /**
   * @brief Destructor
   */
  ~CcRemoteDeviceConfigServer();

  bool loadConfig(const CcString& sPath = "");
  
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
    class CcRemoteDeviceSHARED CEvent
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
  CEvents             oEvents;
  CStartup            oStartup;
  CcHttpServerConfig  oHttpConfig;
};

#endif /* CcRemoteDeviceConfigServer_H_ */
