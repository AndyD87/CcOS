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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceGlobals
 */
#include "CcRemoteDeviceGlobals.h"

namespace CcRemoteDeviceGlobals
{
  const CcString ProjectName("CcRemoteDevice");
  namespace Defaults
  {
    const CcString SslKeyFilename("Key.crt");
    const CcString SslCertificateFilename("Certificate.crt");
  }
  namespace Config
  {
    const CcString Version("Version");
    const CcString VendorId("VendorId");
    const CcString DeviceId("DeviceId");
    const CcString Variant("Variant");
    const CcString SerialNr("SerialNr");
    const CcString HwVersion("HwVersion");
    const CcString SwVersion("SwVersion");
    const CcString Detectable("Detectable");

    const CcString System("System");
    const CcString Interfaces("Interfaces");
    const CcString Software("Software");
    const CcString Events("Events");
    const CcString Startup("Startup");

    namespace SystemNs
    {
      const CcString WlanAccessPoint("WlanAccessPoint");
      const CcString WlanClient("WlanClient");
      const CcString Name("Name");

      namespace WlanAccessPointNs
      {
        const CcString SSID("SSID");
        const CcString Password("Password");
        const CcString Dhcp("Dhcp");
        const CcString Enable("Enable");
      }

      namespace WlanClientNs
      {
        const CcString KnownAccessPoints("KnownAccessPoints");
        const CcString Dhcp("Dhcp");
        const CcString Enable("Enable");
      }
    }
    namespace InterfacesNs
    {
      const CcString RestApiEnabled("RestApiEnabled");
      const CcString RestApi("RestApi");
    }
  }
}
