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
 * @brief     Class CcTableWidget
 **/
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "Style/CcStyleWidget.h"
#include "CcTable.h"
#include "CcTableWidgetRow.h"
#include "Private/CcTableWidgetColumnData.h"

#ifdef _MSC_VER
template class CcGuiSHARED CcList<CcTableWidgetRow>;
template class CcGuiSHARED CcList<CcTableWidgetColumnData>;
#endif

/**
 * @brief Class implementation
 */
class CcGuiSHARED CcTableWidget : public CcWidget, private CcList<CcTableWidgetRow>
{
public:
  /**
   * @brief Constructor
   */
  CcTableWidget(CcWidget* pParent );

  /**
   * @brief Destructor
   */
  virtual ~CcTableWidget();

  /**
   * @brief Get specific row
   * @param uiRow: Target inex of row
   * @return Handle to this
   */
  CcTableWidgetRow& operator[](size_t uiRow)
  { return at(uiRow); }

  //! @return Get number of row
  size_t getRowNumber()
  { return CcList<CcTableWidgetRow>::size(); }
  //! @return get number of columns
  size_t getColumnNumber()
  { return m_uiColumnsCount; }
  //! @return add row with default value to end
  CcTableWidgetRow& addRow();
  //! @brief Add new column at the end and update all containing values.
  void addColumn();

private:
  void updateSizes();

private:
  size_t                          m_uiColumnsCount = 0;
  CcList<CcTableWidgetColumnData> m_oColumns;
};
