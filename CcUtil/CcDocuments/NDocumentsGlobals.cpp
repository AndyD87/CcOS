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
    CcConstString_C(Version,"Version");
    CcConstString_C(VendorId,"VendorId");
    CcConstString_C(DeviceId,"DeviceId");
    CcConstString_C(Variant,"Variant");
    CcConstString_C(SerialNr,"SerialNr");
    CcConstString_C(HwVersion,"HwVersion");
    CcConstString_C(SwVersion,"SwVersion");
    CcConstString_C(Detectable,"Detectable");

    CcConstString_C(System,"System");
    CcConstString_C(Interfaces,"Interfaces");
    CcConstString_C(Events,"Events");
    CcConstString_C(Startup,"Startup");
    CcConstString_C(Application,"App");

    CcConstString_C(WlanAccessPoint,"WlanAccessPoint");
    CcConstString_C(WlanClient,"WlanClient");
    CcConstString_C(WlanCredential,"WlanCredential");
    CcConstString_C(Name,"Name");
    CcConstString_C(SSID,"SSID");
    CcConstString_C(Password,"Password");
    CcConstString_C(DhcpEnable,"DhcpEnable");
    CcConstString_C(Enable,"Enable");
    CcConstString_C(KnownAccessPoints,"KnownAccessPoints");
    CcConstString_C(RestApiEnabled,"RestApiEnabled");
    CcConstString_C(RestApi,"RestApi");
    CcConstString_C(HttpServer,"HttpServer");

    CcConstString_C(WorkingDirectory,"WorkingDirectory");
    CcConstString_C(DefaultEncoding,"DefaultEncoding");
    CcConstString_C(SslEnable,"SslEnable");
    CcConstString_C(SslPrivateKey,"SslPrivateKey");
    CcConstString_C(SslCertificate,"SslCertificate");
    CcConstString_C(SslPrivateKeyPath,"SslPrivateKeyPath");
    CcConstString_C(SslCertificatePath,"SslCertificatePath");
    CcConstString_C(Timeout,"Timeout");
    CcConstString_C(BufferSize,"BufferSize");
    CcConstString_C(MaxThreads,"MaxThreads");
    CcConstString_C(DownloadDirectory,"DownloadDirectory");
  }
}
