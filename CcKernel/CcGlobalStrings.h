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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     namespace CcSystem
 */
#include "CcString.h"
#include "CcKernelBase.h"

#ifndef CcGlobalStrings_H_
#define CcGlobalStrings_H_

namespace CcGlobalStrings
{
  static const CcString Empty("");
  static const CcString True("true");
  static const CcString False("false");
  static const CcString Null("null");
  static const CcString EolShort("\n");
  static const CcString EolLong("\r\n");
#if WIN32
  static const CcString& EolOs = EolLong;
#else
  static const CcString& EolOs = EolShort;
#endif
}

#endif