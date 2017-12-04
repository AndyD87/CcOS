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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcHttpServer
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */

#include "CcHttpConstStrings.h"
#include "CcGlobalStrings.h"


const CcString& CcHttpConstStrings::EOL = CcGlobalStrings::EolLong;
const CcString& CcHttpConstStrings::EOLCompatible = CcGlobalStrings::EolShort;
const CcString CcHttpConstStrings::EOLSeperator("\r\n\r\n");
const CcString CcHttpConstStrings::EOLSeperatorCompatible("\n\n");
const CcString CcHttpConstStrings::Get("GET");
const CcString CcHttpConstStrings::Post("POST");
const CcString CcHttpConstStrings::Head("HEAD");