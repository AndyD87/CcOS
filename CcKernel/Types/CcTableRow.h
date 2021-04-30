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
 * @brief     Class CcTableRow
 */
#pragma once

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

  /**
   * @brief Create row with link to parent table.
   *        Row will be initialized with empty variants by number of columns from parent table.
   * @param pParentTable: Parent table
   */
  CcTableRow(CcTable& pParentTable);

  /**
   * @brief Copy all data from another row
   * @param oToCopy: Row to copy data from
   */
  CcTableRow(const CcTableRow& oToCopy);

  /**
   * @brief Move all data from another row
   * @param oToMove: Row to move data from
   */
  CcTableRow(CcTableRow&& oToMove);


  /**
   * @brief Destructor
   */
  ~CcTableRow() = default;

  /**
   * @brief Change parent tabl to another
   * @param parentTable: New target table
   */
  void setParentTable(CcTable* parentTable);

  /**
   * @brief Move all data from another row
   * @param oToMove: Row to move data from
   * @return Handle to this
   */
  CcTableRow& operator=(CcTableRow&& oToMove);

  /**
   * @brief Copy all data from another row
   * @param oToCopy: Row to copy data from
   * @return Handle to this
   */
  CcTableRow& operator=(const CcTableRow& oToCopy);

  /**
   * @brief Get value by column name.
   *        Name will be translated by parent table to target index.
   * @param sColumnName: Column to get data from
   * @return Value of target column or invalid variant if not found.
   */
  const CcVariant& operator[](const CcString& sColumnName) const;

  /**
   * @brief Get value by colum index.
   * @param uiPosition: Position in row
   * @return Value of target column or invalid variant if not found.
   */
  inline const CcVariant& operator[](size_t uiPosition) const
  { return CcVariantList::operator[](uiPosition); }

  /**
   * @brief Get value by column name.
   *        Name will be translated by parent table to target index.
   * @param sColumnName: Column to get data from
   * @return Value of target column or invalid variant if not found.
   */
  CcVariant& operator[](const CcString& sColumnName);

  /**
   * @brief Get value by colum index.
   * @param uiPosition: Position in row
   * @return Value of target column or invalid variant if not found.
   */
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
private:
  CcTable* m_pPartOfTable = nullptr;
};
