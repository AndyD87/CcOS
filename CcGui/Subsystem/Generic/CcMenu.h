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
 * @page      CcGui
 * @subpage   CcMenu
 *
 * @page      CcMenu
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMenu
 */
#ifndef H_CCMENU_H_
#define H_CCMENU_H_

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
class CcGuiSHARED CcMenu {
public:
  /**
   * @brief Constructor
   */
  CcMenu(const CcWidgetHandle& rParentWidget);

  /**
   * @brief Destructor
   */
  virtual ~CcMenu();

  /**
   * @brief Add a Top-Level entry to menu list
   * @param entry: top node of sub tree
   */
  void addItem(CcTaskbarItem *entry);
  bool delItem(CcTaskbarItem *toDel);
  CcMenuReverse *getReverseList();
  CcTaskbarItem *createItem(const CcString& name = "");
  CcTaskbarItem *at(uint16 pos);

  size_t size();

private:
  CcWidgetHandle      m_parentWidget;     //!< Parent Window for followed items.
  CcVector<CcTaskbarItem*> m_MenuTree;    //!< Top-Level entry points to sub trees
  CcMenuReverse            m_RevList;     //!< Reverse List to keep the active tree in min
};

#endif /* H_CCMENU_H_ */
