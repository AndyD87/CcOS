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
 * @page      CcUtil
 * @subpage   CcDocuments
 *
 * @page      CcDocuments
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 */

#ifndef H_CcDocumentsGlobals_H_
#define H_CcDocumentsGlobals_H_

#include "CcDocument.h"
#include "CcString.h"

namespace CcDocumentsGlobals
{
  namespace Config
  {
    extern const CcDocumentsSHARED CcString Version;
    extern const CcDocumentsSHARED CcString VendorId;
    extern const CcDocumentsSHARED CcString DeviceId;
    extern const CcDocumentsSHARED CcString Variant;
    extern const CcDocumentsSHARED CcString SerialNr;
    extern const CcDocumentsSHARED CcString HwVersion;
    extern const CcDocumentsSHARED CcString SwVersion;
    extern const CcDocumentsSHARED CcString Detectable;

    extern const CcDocumentsSHARED CcString System;
    extern const CcDocumentsSHARED CcString Interfaces;
    extern const CcDocumentsSHARED CcString Events;
    extern const CcDocumentsSHARED CcString Startup;
    extern const CcDocumentsSHARED CcString Application;

    namespace SystemNs
    {
      extern const CcDocumentsSHARED CcString WlanAccessPoint;
      extern const CcDocumentsSHARED CcString WlanClient;
      extern const CcDocumentsSHARED CcString WlanCredential;
      extern const CcDocumentsSHARED CcString Name;

      namespace WlanAccessPointNs
      {
        extern const CcDocumentsSHARED CcString SSID;
        extern const CcDocumentsSHARED CcString Password;
        extern const CcDocumentsSHARED CcString DhcpEnable;
        extern const CcDocumentsSHARED CcString Enable;
      }

      namespace WlanClientNs
      {
        extern const CcDocumentsSHARED CcString KnownAccessPoints;
        extern const CcDocumentsSHARED CcString DhcpEnable;
        extern const CcDocumentsSHARED CcString Enable;
      }
    }
    namespace InterfacesNs
    {
      extern const CcDocumentsSHARED CcString RestApiEnabled;
      extern const CcDocumentsSHARED CcString RestApi;
    }
  }
}

#endif // H_CcDocumentsGlobals_H_
