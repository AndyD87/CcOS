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
 * @brief     Class CcTableRow
 */
#ifndef H_CcTableRow_H_
#define H_CcTableRow_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcListCommon.h"
#include "CcHandle.h"
#include "CcVariantList.h"

class CcTable;

#ifdef _MSC_VER
template class CcKernelSHARED CcHandle<CcTable>;
#endif

/**
 * @brief Storage Class for Tables
 *        it operates with a fixed size of Columns, and varios rows.
 */
class CcKernelSHARED  CcTableRow : public CcVariantList
{
public:
  CcTableRow() = default;

  CcTableRow(CcTable& pParentTable);

  /**
   * @brief CopyConstructor
   */
  CcTableRow(const CcTableRow& oToCopy);

  /**
   * @brief CopyConstructor
   */
  CcTableRow(CcTableRow&& oToMove);


  /**
   * @brief Destructor
   */
  ~CcTableRow() = default;

  void setParentTable(CcTable* parentTable);

  CcTableRow& operator=(CcTableRow&& oToMove);
  CcTableRow& operator=(const CcTableRow& oToCopy);
  const CcVariant& operator[](const CcString& sColumnName) const;
  inline const CcVariant& operator[](size_t uiPosition) const
    { return CcVariantList::operator[](uiPosition); }
  CcVariant& operator[](const CcString& sColumnName);
  inline CcVariant& operator[](size_t uiPosition)
    { return CcVariantList::operator[](uiPosition); }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcTableRow& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcTableRow& oToCompare) const
    { return !operator==(oToCompare); }

  uint32 getHeight() const
  { return m_uiHeight; }
  float getHeightRelative() const
  { return m_fHeightRelative; }
private:
  CcTable* m_pPartOfTable = nullptr;
  uint32   m_uiHeight = 0;
  float    m_fHeightRelative = 1.0;
};

#endif // H_CcTableRow_H_
