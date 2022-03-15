/*
 * This file is part of CcConfigBinary.
 *
 * CcConfigBinary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConfigBinary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConfigBinary.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcConfigBinary
 */
#include "CcConfigMap.h"

void CcConfigMap::appendItem(CcConfigBinary::EType eType, void* pValue)
{
  appendItem(eType, CcConfigBinary::getString(eType), pValue);
}

void CcConfigMap::appendItem(CcConfigBinary::EType eType, const CcString& sName, void* pValue)
{
  append(CcConfigItem(eType, sName), pValue);
}

void* CcConfigMap::getItem(const CcString& sName) const
{
  for (const CcPair<CcConfigItem, void*>& oItem : *this)
  {
    if (oItem.getKey().sName == sName)
    {
      return oItem.getValue();
    }
  }
  return nullptr;
}

void* CcConfigMap::getItem(const CcConfigBinary::EType& eType) const
{
  for (const CcPair<CcConfigItem, void*>& oItem : *this)
  {
    if (oItem.getKey().eType == eType)
    {
      return oItem.getValue();
    }
  }
  return nullptr;
}
