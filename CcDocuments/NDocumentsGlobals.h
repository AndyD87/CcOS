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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 */
#pragma once

#include "CcDocument.h"
#include "CcString.h"

namespace NDocumentsGlobals
{
  namespace NConfig
  {
    extern CcDocumentsSHARED CcConstString_H(Version);        //!< String for Version
    extern CcDocumentsSHARED CcConstString_H(VendorId);       //!< String for VendorId
    extern CcDocumentsSHARED CcConstString_H(DeviceId);       //!< String for DeviceId
    extern CcDocumentsSHARED CcConstString_H(Variant);        //!< String for Variant
    extern CcDocumentsSHARED CcConstString_H(SerialNr);       //!< String for SerialNr
    extern CcDocumentsSHARED CcConstString_H(HwVersion);      //!< String for HwVersion
    extern CcDocumentsSHARED CcConstString_H(SwVersion);      //!< String for SwVersion
    extern CcDocumentsSHARED CcConstString_H(Detectable);     //!< String for Detectable

    extern CcDocumentsSHARED CcConstString_H(System);         //!< String for System
    extern CcDocumentsSHARED CcConstString_H(Interfaces);     //!< String for Interfaces
    extern CcDocumentsSHARED CcConstString_H(Events);         //!< String for Events
    extern CcDocumentsSHARED CcConstString_H(Startup);        //!< String for Startup
    extern CcDocumentsSHARED CcConstString_H(Application);    //!< String for Application
    extern CcDocumentsSHARED CcConstString_H(User);           //!< String for User
    extern CcDocumentsSHARED CcConstString_H(Users);          //!< String for Users
    extern CcDocumentsSHARED CcConstString_H(Admin);          //!< String for Admin

    extern CcDocumentsSHARED CcConstString_H(KnownAccessPoints);  //!< String for KnownAccessPoints
    extern CcDocumentsSHARED CcConstString_H(DhcpEnable);         //!< String for DhcpEnable);
    extern CcDocumentsSHARED CcConstString_H(Enable);             //!< String for Enable);
    extern CcDocumentsSHARED CcConstString_H(RestApiEnabled);     //!< String for RestApiEnabled);
    extern CcDocumentsSHARED CcConstString_H(RestApi);            //!< String for RestApi);
    extern CcDocumentsSHARED CcConstString_H(HttpServer);         //!< String for HttpServer);
    extern CcDocumentsSHARED CcConstString_H(SSID);               //!< String for SSID);
    extern CcDocumentsSHARED CcConstString_H(Password);           //!< String for Password);
    extern CcDocumentsSHARED CcConstString_H(DhcpEnable);         //!< String for DhcpEnable);
    extern CcDocumentsSHARED CcConstString_H(Enable);             //!< String for Enable);
    extern CcDocumentsSHARED CcConstString_H(WlanAccessPoint);    //!< String for WlanAccessPoint);
    extern CcDocumentsSHARED CcConstString_H(WlanClient);         //!< String for WlanClient);
    extern CcDocumentsSHARED CcConstString_H(WlanCredential);     //!< String for WlanCredential);
    extern CcDocumentsSHARED CcConstString_H(Name);               //!< String for Name);

    extern CcDocumentsSHARED CcConstString_H(WorkingDirectory);   //!< String for WorkingDirectory)
    extern CcDocumentsSHARED CcConstString_H(DefaultEncoding);    //!< String for DefaultEncoding);
    extern CcDocumentsSHARED CcConstString_H(SslEnable);          //!< String for SslEnable);
    extern CcDocumentsSHARED CcConstString_H(SslPrivateKey);      //!< String for SslPrivateKey);
    extern CcDocumentsSHARED CcConstString_H(SslCertificate);     //!< String for SslCertificate);
    extern CcDocumentsSHARED CcConstString_H(SslPrivateKeyPath);  //!< String for SslPrivateKeyPath
    extern CcDocumentsSHARED CcConstString_H(SslCertificatePath); //!< String for SslCertificatePat
    extern CcDocumentsSHARED CcConstString_H(Timeout);            //!< String for Timeout);
    extern CcDocumentsSHARED CcConstString_H(BufferSize);         //!< String for BufferSize);
    extern CcDocumentsSHARED CcConstString_H(MaxThreads);         //!< String for MaxThreads);
    extern CcDocumentsSHARED CcConstString_H(DownloadDirectory);  //!< String for DownloadDirectory

    extern CcDocumentsSHARED CcConstString_H(Port);               //!< String for Port

    extern CcDocumentsSHARED CcConstString_H(IpV4);               //!< Ipv4 Value
    extern CcDocumentsSHARED CcConstString_H(IpV4Begin);          //!< Begin of an IpV4 Range
    extern CcDocumentsSHARED CcConstString_H(IpV4End);            //!< End of an IpV4 Range
  }
}
