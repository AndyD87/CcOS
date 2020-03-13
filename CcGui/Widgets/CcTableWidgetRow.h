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
 * @subpage   CcTableWidgetRow
 *
 * @page      CcTableWidgetRow
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTableWidgetRow
 **/
#ifndef H_CcTableWidgetRow_H_
#define H_CcTableWidgetRow_H_

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
 * @brief Class implementation
 */
class CcGuiSHARED CcTableWidgetRow : private CcList<CcTableWidgetCell>
{
public:
  /**
   * @brief Constructor
   */
  CcTableWidgetRow(CcTableWidget* pParent = nullptr, size_t uiSize = 0);

  /**
   * @brief Destructor
   */
  virtual ~CcTableWidgetRow();

  bool operator==(const CcTableWidgetRow& rToCompare) const { CCUNUSED(rToCompare); return false; }

  void addColumn();
  CcTableWidget* getParent() const
  {return m_pParent;}

private:
  CcTableWidget* m_pParent;
};

#endif // H_CcTableWidgetRow_H_
