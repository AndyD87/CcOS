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
 * @brief     Class CcMenu
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcList.h"
#include "CcMenuItem.h"
#include "CcMenuReverse.h"
#include "CcTaskbar.h"
#include "CcTaskbarItem.h"

#ifdef _MSC_VER
template class CcGuiSHARED CcVector<CcTaskbarItem*>;
#endif

/**
 * @brief Menue-Tree start point with settings for display
 */
class CcGuiSHARED CcMenu
{
public:
  /**
   * @brief Constructor
   */
  CcMenu(CcWidget* rParentWidget);

  /**
   * @brief Destructor
   */
  virtual ~CcMenu();

  /**
   * @brief Add a Top-Level entry to menu list
   * @param entry: top node of sub tree
   */
  void addItem(CcTaskbarItem *entry);

  /**
   * @brief Delete item from taskbar
   * @param toDel:  Item to delete
   * @return True if found and deleted
   */
  bool delItem(CcTaskbarItem *toDel);


  /**
   * @brief Create ne taskbar item with name
   * @param name: Name for new item
   * @return Pointer to created item or null on error
   */
  CcTaskbarItem* createItem(const CcString& name = "");

  /**
   * @brief Get taskbar item at specific location
   * @param pos:  Index of taskbar to query for
   * @return Item at index or nullptr if pos not valid.
   */
  CcTaskbarItem* at(uint16 pos);

  //! @return Get number of entries in list
  size_t size();
  //! @return Get reverse list for navigation
  CcMenuReverse* getReverseList();

private:
  CcWidget*      m_parentWidget;     //!< Parent Window for followed items.
  CcVector<CcTaskbarItem*> m_MenuTree;    //!< Top-Level entry points to sub trees
  CcMenuReverse            m_RevList;     //!< Reverse List to keep the active tree in min
};
