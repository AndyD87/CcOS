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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVariantList
 */
#pragma once

#include "CcBase.h"
#include "CcList.h"
#include "CcVariant.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcVariant>;
#endif

/**
 * @brief Variant list
 *        It is prepared to add some further methods like filter etc.
 */
class CcKernelSHARED CcVariantList : public CcList<CcVariant>
{
public:
  CcVariantList();

  /**
   * @brief Copy content from an other variant list
   * @param oToCopy: List to copy from
   */
  CcVariantList(const CcVariantList &oToCopy) :
    CcList<CcVariant>(oToCopy)
  {}

  /**
   * @brief CcVariantList
   * @param oToMove
   */
  CcVariantList(CcVariantList&& oToMove) NOEXCEPT :
    CcList<CcVariant>(CCMOVE(oToMove))
  {}

  /**
   * @brief Initialize list with first item.
   * @param oItem: Varian to add on first position.
   */
  CcVariantList(const CcVariant& oItem) :
    CcList<CcVariant>(oItem)
  {}

  /**
   * @brief Initialize this list with an array of variants.
   * @param pItems:   Initial variants
   * @param uiCount:  Number of variants in pItems
   */
  CcVariantList(const CcVariant* pItems, size_t uiCount) :
    CcList<CcVariant>(pItems, uiCount)
  {}

  /**
   * @brief Create a list with predefined number of empty variants.
   * @param uiCount: Number of variants initialy add.
   */
  CcVariantList(size_t uiCount) :
    CcList<CcVariant>(uiCount)
  {}

  /**
   * @brief Copy data from variant list to this
   * @param oToCopy: List to copy from
   */
  void operator=(const CcVariantList& oToCopy)
  {
    CcList<CcVariant>::operator=(oToCopy);
  }

  /**
   * @brief Move content of a variant list to this.
   * @param oToMove: List to move from
   */
  void operator=(CcVariantList&& oToMove) NOEXCEPT
  {
    CcList<CcVariant>::operator=(CCMOVE(oToMove));
  }
};
