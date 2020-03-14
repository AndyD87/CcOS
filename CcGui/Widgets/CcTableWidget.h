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
 * @page      Widgets
 * @subpage   CcTableWidget
 *
 * @page      CcTableWidget
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTableWidget
 **/
#ifndef H_CcTableWidget_H_
#define H_CcTableWidget_H_

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

  CcTableWidgetRow& operator[](size_t uiRow)
  { return at(uiRow); }

  size_t getRowNumber()
  { return CcList<CcTableWidgetRow>::size(); }
  size_t getColumnNumber()
  { return m_uiColumnsCount; }

  CcTableWidgetRow& addRow();
  void addColumn();

private:
  size_t                          m_uiColumnsCount = 0;
  CcList<CcTableWidgetColumnData> m_oColumns;
};

#endif // H_CcTableWidget_H_
