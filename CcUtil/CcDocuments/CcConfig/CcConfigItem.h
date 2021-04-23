/*
 * This file is part of CcConfigItem.
 *
 * CcConfigItem is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConfigItem is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConfigItem.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConfigItem
 **/
#pragma once

#include "CcVariant.h"
#include "CcConfigBinary.h"
#include "CcString.h"
#include "CcGlobalStrings.h"

/**
 * @brief Configuration class for binary storage.
 *        It can be filled with known types to save some bytes, or with custom definitions
 *        to be flexible for changes.
 */
class CcDocumentsSHARED CcConfigItem
{
public:
  CcConfigItem(CcConfigBinary::EType eType = CcConfigBinary::EType::Unknown, const CcString& sName = CcGlobalStrings::Empty) :
    eType(eType),
    sName(sName)
  {}

  CcConfigBinary::EType eType;  //!< Type of item
  CcString              sName;  //!< Name of item to identify

  /**
   * @brief Compare an items with this item if they are equal
   * @param oToCompare: Object to compare with
   * @return true if items are equal
   */
  bool operator==(const CcConfigItem& oToCompare) const
  { return eType == oToCompare.eType && sName == oToCompare.sName; }
};
