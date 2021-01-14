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
 * @file
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Definitions for Http Types like MIME
 *
 *            MIME-Types are related to http://wiki.selfhtml.org/wiki/Referenz:MIME-Typen
 */
#ifndef H_CcHttpDefaults_H_
#define H_CcHttpDefaults_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"

namespace CcHttpDefaults
{
  extern CcHttpSHARED CcString Charset;   //!< Default charset for response and request
  extern CcHttpSHARED CcString Agent;     //!< Default agent for request
  extern CcHttpSHARED CcString ServerName;//!< Default name of server in response header
}
#endif // H_CcHttpDefaults_H_
