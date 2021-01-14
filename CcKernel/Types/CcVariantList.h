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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVariantList
 */
#ifndef H_CcVariantList_H_
#define H_CcVariantList_H_

#include "CcBase.h"
#include "CcList.h"
#include "CcVariant.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcVariant>;
#endif

class CcKernelSHARED CcVariantList : public CcList<CcVariant>
{
public:
  CcVariantList();
  CcVariantList(const CcVariantList &oToCopy) :
    CcList<CcVariant>(oToCopy)
  {}

  CcVariantList(CcVariantList&& oToMove) NOEXCEPT :
    CcList<CcVariant>(CCMOVE(oToMove))
  {}

  CcVariantList(const CcVariant& oItem) :
    CcList<CcVariant>(oItem)
  {}

  CcVariantList(const CcVariant* pItems, size_t uiCount) :
    CcList<CcVariant>(pItems, uiCount)
  {}

  CcVariantList(size_t uiCount) :
    CcList<CcVariant>(uiCount)
  {}

  void operator=(const CcVariantList& oToCopy)
  {
    CcList<CcVariant>::operator=(oToCopy);
  }

  void operator=(CcVariantList&& oToMove)
  {
    CcList<CcVariant>::operator=(CCMOVE(oToMove));
  }
};

#endif // H_CcVariantList_H_
