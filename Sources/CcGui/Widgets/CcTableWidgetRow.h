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
 * @brief     Class CcTableWidgetRow
 **/
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "Style/CcStyleWidget.h"
#include "CcTable.h"
#include "CcTableWidgetCell.h"

#ifdef _MSC_VER
template class CcGuiSHARED CcList<CcTableWidgetCell*>;
#endif

class CcTableWidget;

/**
 * @brief Table with private list of row widgets
 */
class CcGuiSHARED CcTableWidgetRow : private CcList<CcTableWidgetCell>
{
public:
  //! @brief Iterator forwarded from basic list
  typedef CcList<CcTableWidgetCell>::iterator iterator;

  /**
   * @brief Constructor
   * @param pParent: parent widget to dispaly this table in
   * @param uiSize:  Number of colums to init with
   */
  CcTableWidgetRow(CcTableWidget* pParent = nullptr, size_t uiSize = 0);

  /**
   * @brief Destructor
   */
  virtual ~CcTableWidgetRow();

  /**
   * @brief Compare with another row.
   *        Fake implementation for list, it will always retur false.
   * @param rToCompare: Row to compare with
   * @return false
   */
  bool operator==(const CcTableWidgetRow& rToCompare) const { CCUNUSED(rToCompare); return false; }
  /**
   * @brief Get cell at specific index
   * @param uiCell: Index of queried cell
   * @return Cell at target index
   */
  CcTableWidgetCell& operator[](size_t uiCell)
  { return at(uiCell); }

  //! @return Begin iterator for walk throug list
  iterator begin()
  {return CcList<CcTableWidgetCell>::begin();}
  //! @return End iterator to compare for end of list
  iterator end()
  {return CcList<CcTableWidgetCell>::end();}
  //! @return Get height of row
  uint32 getHeight()
  { return m_uiHeight; }
  //! @return Get height of row relative
  float getHeightRelative()
  { return m_fHeightRelative; }
  //! @return Get parent table
  CcTableWidget* getTable() const
  {return m_pParent;}

  /**
   * @brief Add columen to row init with default valu
   */
  void addColumn();

  //! @param uiHeight: Set fixed height
  void setHeight(uint32 uiHeight)
  { m_uiHeight = uiHeight; m_fHeightRelative = 0.0; }
  //! @param fHeight: Set relative height
  void setHeight(float fHeight)
  { m_fHeightRelative = fHeight; m_uiHeight = 0; }

private:
  CcTableWidget* m_pParent;
  uint32 m_uiHeight = 0;
  float m_fHeightRelative = 1.0;
};
