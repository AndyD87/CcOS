/*
 * This file is part of CcRemoteDeviceGlobals.
 *
 * CcRemoteDeviceGlobals is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceGlobals is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceGlobals.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcRemoteDeviceGlobals
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceGlobals
 **/
#ifndef H_CcRemoteDeviceGlobals_H_
#define H_CcRemoteDeviceGlobals_H_

#include "CcRemoteDevice.h"
#include "CcString.h"
#include "Network/CcCommonPorts.h"

namespace CcRemoteDeviceGlobals
{
  extern const CcRemoteDeviceSHARED CcString ProjectName;
  namespace Defaults
  {
    extern const CcRemoteDeviceSHARED CcString SslKeyFilename;
    extern const CcRemoteDeviceSHARED CcString SslCertificateFilename;
    static const uint16& Port = CcCommonPorts::CcRemoteDevice;
  }
  namespace Config
  {
    extern const CcRemoteDeviceSHARED CcString& RootNode;
    extern const CcRemoteDeviceSHARED CcString ServerNode;
    extern const CcRemoteDeviceSHARED CcString ClientNode;
    namespace Server
    {
      extern const CcRemoteDeviceSHARED CcString StartupNode;
      namespace Startup
      {
        extern const CcRemoteDeviceSHARED CcString StopOnErrorNode;
        extern const CcRemoteDeviceSHARED CcString CommandsNode;
        namespace Commands
        {
          extern const CcRemoteDeviceSHARED CcString CommandNode;
        }
      }
      extern const CcRemoteDeviceSHARED CcString TimeEventsNode;
      namespace Startup
      {
        extern const CcRemoteDeviceSHARED CcString EventNode;
        namespace Event
        {
          extern const CcRemoteDeviceSHARED CcString TypeNode;
          extern const CcRemoteDeviceSHARED CcString DataNode;
        }
      }
      extern const CcRemoteDeviceSHARED CcString RemoteControlNode;
      namespace RemoteControl
      {
        extern const CcRemoteDeviceSHARED CcString PortNode;
        extern const CcRemoteDeviceSHARED CcString SslCertificateNode;
      }
    }
  }
}

#endif /* H_CcRemoteDeviceGlobals_H_ */
