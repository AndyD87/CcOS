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
    static const uint16& HttpPort = CcCommonPorts::HTTP;
  }
  namespace Config
  {
    extern const CcRemoteDeviceSHARED CcString Version;
    extern const CcRemoteDeviceSHARED CcString VendorId;
    extern const CcRemoteDeviceSHARED CcString DeviceId;
    extern const CcRemoteDeviceSHARED CcString Variant;
    extern const CcRemoteDeviceSHARED CcString SerialNr;
    extern const CcRemoteDeviceSHARED CcString HwVersion;
    extern const CcRemoteDeviceSHARED CcString SwVersion;
    extern const CcRemoteDeviceSHARED CcString Detectable;

    extern const CcRemoteDeviceSHARED CcString System;
    extern const CcRemoteDeviceSHARED CcString Interfaces;
    extern const CcRemoteDeviceSHARED CcString Events;
    extern const CcRemoteDeviceSHARED CcString Startup;
    extern const CcRemoteDeviceSHARED CcString Application;

    namespace SystemNs
    {
      extern const CcRemoteDeviceSHARED CcString WlanAccessPoint;
      extern const CcRemoteDeviceSHARED CcString WlanClient;
      extern const CcRemoteDeviceSHARED CcString Name;

      namespace WlanAccessPointNs
      {
        extern const CcRemoteDeviceSHARED CcString SSID;
        extern const CcRemoteDeviceSHARED CcString Password;
        extern const CcRemoteDeviceSHARED CcString Dhcp;
        extern const CcRemoteDeviceSHARED CcString Enable;
      }

      namespace WlanClientNs
      {
        extern const CcRemoteDeviceSHARED CcString KnownAccessPoints;
        extern const CcRemoteDeviceSHARED CcString Dhcp;
        extern const CcRemoteDeviceSHARED CcString Enable;
      }
    }
    namespace InterfacesNs
    {
      extern const CcRemoteDeviceSHARED CcString RestApiEnabled;
      extern const CcRemoteDeviceSHARED CcString RestApi;
    }
  }
}

#endif // H_CcRemoteDeviceGlobals_H_
