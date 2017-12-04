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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Definitions for Http Types like MIME
 *
 *            MIME-Types are related to http://wiki.selfhtml.org/wiki/Referenz:MIME-Typen
 */
#ifndef CcHttpGlobals_H_
#define CcHttpGlobals_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"

namespace CcHttpGlobals
{
  extern const CcHttpSHARED CcString ServerConfigDirName;  //!< Default Name of subdirectory in Global ConfigDir
  extern const CcHttpSHARED CcString ServerConfigFileName;     //!< Default Name configuration-File

  extern const CcHttpSHARED CcString MIME_APPLICATION;  //!< default value for Application-Data
  extern const CcHttpSHARED CcString MIME_APPLICATION_PDF;           //!< Application Data is in PDF-Format
  extern const CcHttpSHARED CcString MIME_APPLICATION_GZIP;          //!< Application Data is gzip compressed
  extern const CcHttpSHARED CcString MIME_APPLICATION_JSON;          //!< Output/Input is in JSON format

  extern const CcHttpSHARED CcString MIME_IMAGE;   //!< default value for image types
  extern const CcHttpSHARED CcString MIME_IMAGE_JPEG;   //!< Image is an JPEG
  extern const CcHttpSHARED CcString MIME_IMAGE_PNG;    //!< Image is an PNG
  extern const CcHttpSHARED CcString MIME_IMAGE_BMP;    //!< Image is an BMP
  extern const CcHttpSHARED CcString MIME_IMAGE_GIF;    //!< Image is an GIF

  extern const CcHttpSHARED CcString MIME_TEXT;        //!< Default Text-Format is Plain
  extern const CcHttpSHARED CcString MIME_TEXT_XML;          //!< Text Format is XML
  extern const CcHttpSHARED CcString MIME_TEXT_CSS;          //!< Text Format is CSS
  extern const CcHttpSHARED CcString MIME_TEXT_HTML;         //!< Text Format is HTML
  extern const CcHttpSHARED CcString MIME_TEXT_JS;   //!< Text Format is JavaScript
  extern const CcHttpSHARED CcString MIME_TEXT_PLAIN;        //!< Text Format is Plain, no formatation

}
#endif /* CcHttpGlobals_H_ */
