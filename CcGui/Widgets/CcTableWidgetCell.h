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
 * @subpage   CcTableWidgetCell
 *
 * @page      CcTableWidgetCell
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTableWidgetCell
 **/
#ifndef H_CcTableWidgetCell_H_
#define H_CcTableWidgetCell_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "Style/CcStyleWidget.h"
#include "CcTable.h"

class CcTableWidgetRow;

/**
 * @brief Class implementation
 */
class CcGuiSHARED CcTableWidgetCell
{
public:
  /**
   * @brief Constructor
   */
  CcTableWidgetCell(CcTableWidgetRow *pParent);

  /**
   * @brief Destructor
   */
  virtual ~CcTableWidgetCell();

  CcTableWidgetRow* getParent() const
  {return m_pParent;}

  void setChild(CcWidget* pChild);
  CcWidget* getChild() const
  { return m_pChild; }

  bool operator==(const CcTableWidgetCell& rToCompare) const { CCUNUSED(rToCompare); return false; }
private:
  CcTableWidgetRow* m_pParent;
  CcWidget* m_pChild = nullptr;
};

#endif // H_CcTableWidgetCell_H_
