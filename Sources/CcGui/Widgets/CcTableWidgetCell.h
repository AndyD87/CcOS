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
 * @brief     Class CcTableWidgetCell
 **/
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "Style/CcStyleWidget.h"
#include "CcTable.h"

class CcTableWidget;
class CcTableWidgetRow;

/**
 * @brief Class implementation
 */
class CcGuiSHARED CcTableWidgetCell
{
public:
  /**
   * @brief Constructor
   * @param pParent: Parent widget
   */
  CcTableWidgetCell(CcTableWidgetRow *pParent = nullptr);

  /**
   * @brief Copy content from another cell
   * @param oToCopy: Cell to copy from
   */
  CcTableWidgetCell(const CcTableWidgetCell& oToCopy)
  {operator=(oToCopy);}

  /**
   * @brief Destructor
   */
  virtual ~CcTableWidgetCell();

  /**
   * @brief Copy content from another cell to this
   * @param oToCopy: Cell to copy from
   * @return Handle to this
   */
  CcTableWidgetCell& operator=(const CcTableWidgetCell& oToCopy);

  //! @return Get parent row
  CcTableWidgetRow* getRow() const
  {return m_pParent;}
  //! @return Get parent table
  CcTableWidget* getTable() const;
  //! @return Get widget of this cell
  CcWidget* getChild() const
  { return m_pChild; }

  //! @param pChild: Replace child
  void setChild(CcWidget* pChild)
  { m_pChild = pChild; }

  /**
   * @brief Compare with another cell is not implemented yet.
   *        It will return false.
   * @param rToCompare: Cell to compare with
   * @return False
   */
  bool operator==(const CcTableWidgetCell& rToCompare) const { CCUNUSED(rToCompare); return false; }
private:
  CcTableWidgetRow* m_pParent;
  CcWidget* m_pChild = nullptr;
};
