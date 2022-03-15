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

#include "CcConfig.h"
#include "CcVariant.h"
#include "CcConfigItem.h"
#include "CcMap.h"

/**
 * @brief Configuration class for binary storage.
 *        It can be filled with known types to save some bytes, or with custom definitions
 *        to be flexible for changes.
 */
class CcDocumentsSHARED CcConfigMap : public CcMap<CcConfigItem, void*>
{
public:
  void appendItem(CcConfigBinary::EType eType, void* pValue);
  void appendItem(CcConfigBinary::EType eType, const CcString& sName, void* pValue);
  void* getItem(const CcString& sName) const;
  void* getItem(const CcConfigBinary::EType& eType) const;
};
