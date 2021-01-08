/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY); without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcHttp
 * @subpage   CcHttpGlobals
 *
 * @page      CcHttpGlobals
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Definitions for Http Types like MIME
 *
 *            MIME-Types are related to http://wiki.selfhtml.org/wiki/Referenz:MIME-Typen
 */

#include "CcBase.h"
#include "CcHttpGlobals.h"

namespace CcHttpGlobals
{
  const CcString ServerConfigDirName  ("/CcHttpServer");  //!< Default Name of subdirectory in Global ConfigDir
  const CcString ServerConfigFileName ("http.conf");     //!< Default Name configuration-File

  const CcString MIME_APPLICATION       ("application/octet-stream");  //!< default value for Application-Data
  const CcString MIME_APPLICATION_PDF   ("application/pdf");           //!< Application Data is in PDF-Format
  const CcString MIME_APPLICATION_GZIP  ("application/gzip");          //!< Application Data is gzip compressed
  const CcString MIME_APPLICATION_JSON  ("application/json");          //!< Output/Input is in JSON format

  const CcString MIME_IMAGE        ("image/jpeg");   //!< default value for image types
  const CcString MIME_IMAGE_JPEG   ("image/jpeg");   //!< Image is an JPEG
  const CcString MIME_IMAGE_PNG    ("image/png");    //!< Image is an PNG
  const CcString MIME_IMAGE_BMP    ("image/bmp");    //!< Image is an BMP
  const CcString MIME_IMAGE_GIF    ("image/gif");    //!< Image is an GIF

  const CcString MIME_TEXT       ("text/plain");        //!< Default Text-Format is Plain
  const CcString MIME_TEXT_XML   ("text/xml");          //!< Text Format is XML
  const CcString MIME_TEXT_CSS   ("text/css");          //!< Text Format is CSS
  const CcString MIME_TEXT_HTML  ("text/html");         //!< Text Format is HTML
  const CcString MIME_TEXT_JS    ("text/javascript");   //!< Text Format is JavaScript
  const CcString MIME_TEXT_PLAIN ("text/plain");        //!< Text Format is Plain, no formatation

}

