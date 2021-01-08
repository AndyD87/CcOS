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
 * @subpage   CcHttpDefaults
 *
 * @page      CcHttpDefaults
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Definitions for Http Types like MIME
 *
 *            MIME-Types are related to http://wiki.selfhtml.org/wiki/Referenz:MIME-Typen
 */

#include "CcBase.h"
#include "CcHttpDefaults.h"

namespace CcHttpDefaults
{
  CcString CcHttpSHARED Charset      ("utf-8");
  CcString CcHttpSHARED Agent        ("CcOS Http-Client");
  CcString CcHttpSHARED ServerName   ("CcOS Http-Server");
}

