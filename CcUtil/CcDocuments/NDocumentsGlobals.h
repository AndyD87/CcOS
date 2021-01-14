/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 */

#ifndef H_NDocumentsGlobals_H_
#define H_NDocumentsGlobals_H_

#include "CcDocument.h"
#include "CcString.h"

namespace NDocumentsGlobals
{
  namespace NConfig
  {
    extern CcDocumentsSHARED CcConstString_H(Version);
    extern CcDocumentsSHARED CcConstString_H(VendorId);
    extern CcDocumentsSHARED CcConstString_H(DeviceId);
    extern CcDocumentsSHARED CcConstString_H(Variant);
    extern CcDocumentsSHARED CcConstString_H(SerialNr);
    extern CcDocumentsSHARED CcConstString_H(HwVersion);
    extern CcDocumentsSHARED CcConstString_H(SwVersion);
    extern CcDocumentsSHARED CcConstString_H(Detectable);

    extern CcDocumentsSHARED CcConstString_H(System);
    extern CcDocumentsSHARED CcConstString_H(Interfaces);
    extern CcDocumentsSHARED CcConstString_H(Events);
    extern CcDocumentsSHARED CcConstString_H(Startup);
    extern CcDocumentsSHARED CcConstString_H(Application);

    extern CcDocumentsSHARED CcConstString_H(KnownAccessPoints);
    extern CcDocumentsSHARED CcConstString_H(DhcpEnable);
    extern CcDocumentsSHARED CcConstString_H(Enable);
    extern CcDocumentsSHARED CcConstString_H(RestApiEnabled);
    extern CcDocumentsSHARED CcConstString_H(RestApi);
    extern CcDocumentsSHARED CcConstString_H(HttpServer);
    extern CcDocumentsSHARED CcConstString_H(SSID);
    extern CcDocumentsSHARED CcConstString_H(Password);
    extern CcDocumentsSHARED CcConstString_H(DhcpEnable);
    extern CcDocumentsSHARED CcConstString_H(Enable);
    extern CcDocumentsSHARED CcConstString_H(WlanAccessPoint);
    extern CcDocumentsSHARED CcConstString_H(WlanClient);
    extern CcDocumentsSHARED CcConstString_H(WlanCredential);
    extern CcDocumentsSHARED CcConstString_H(Name);

    extern CcDocumentsSHARED CcConstString_H(WorkingDirectory);
    extern CcDocumentsSHARED CcConstString_H(DefaultEncoding);
    extern CcDocumentsSHARED CcConstString_H(SslEnable);
    extern CcDocumentsSHARED CcConstString_H(SslPrivateKey);
    extern CcDocumentsSHARED CcConstString_H(SslCertificate);
    extern CcDocumentsSHARED CcConstString_H(SslPrivateKeyPath);
    extern CcDocumentsSHARED CcConstString_H(SslCertificatePath);
    extern CcDocumentsSHARED CcConstString_H(Timeout);
    extern CcDocumentsSHARED CcConstString_H(BufferSize);
    extern CcDocumentsSHARED CcConstString_H(MaxThreads);
    extern CcDocumentsSHARED CcConstString_H(DownloadDirectory);

    extern CcDocumentsSHARED CcConstString_H(Port);
  }
}

#endif // H_NDocumentsGlobals_H_
