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
 * @page      CcKernel
 * @subpage   CcGlobalStrings
 *
 * @page      CcGlobalStrings
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     namespace CcSystem
 */
#include "CcString.h"
#include "CcGlobalStrings.h"

namespace CcGlobalStrings
{
  const CcString Empty("");
  const CcString True("true");
  const CcString False("false");
  const CcString On("on");
  const CcString Off("off");
  const CcString Null("null");
  const CcString EolShort("\n");
  const CcString EolLong("\r\n");
#if WIN32
  const CcString& EolOs = EolLong;
#else
  const CcString& EolOs = EolShort;
#endif
  namespace Seperators
  {
    const CcString Dot(".");
    const CcString Pipe("|");
    const CcString Comma(",");
    const CcString Colon(":");
    const CcString Semicolon(";");
    const CcString& IpV4 = Dot;
    const CcString& MacAddress = Colon;
    const CcString MacAddress2("-");
  }
}
