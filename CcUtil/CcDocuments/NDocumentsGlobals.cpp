/*
 * This file is part of NDocumentsGlobals.
 *
 * NDocumentsGlobals is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * NDocumentsGlobals is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with NDocumentsGlobals.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class NDocumentsGlobals
 */
#include "NDocumentsGlobals.h"

namespace NDocumentsGlobals
{
  namespace NConfig
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
    const CcString Events("Events");
    const CcString Startup("Startup");
    const CcString Application("App");

    const CcString WlanAccessPoint("WlanAccessPoint");
    const CcString WlanClient("WlanClient");
    const CcString WlanCredential("WlanCredential");
    const CcString Name("Name");
    const CcString SSID("SSID");
    const CcString Password("Password");
    const CcString DhcpEnable("DhcpEnable");
    const CcString Enable("Enable");
    const CcString KnownAccessPoints("KnownAccessPoints");
    const CcString RestApiEnabled("RestApiEnabled");
    const CcString RestApi("RestApi");

    const CcString WorkingDirectory("WorkingDirectory");
    const CcString DefaultEncoding("DefaultEncoding");
    const CcString SslEnable("SslEnable");
    const CcString SslPrivateKey("SslPrivateKey");
    const CcString SslCertificate("SslCertificate");
    const CcString SslPrivateKeyPath("SslPrivateKeyPath");
    const CcString SslCertificatePath("SslCertificatePath");
    const CcString Timeout("Timeout");
    const CcString BufferSize("BufferSize");
    const CcString MaxThreads("MaxThreads");
  }
}
