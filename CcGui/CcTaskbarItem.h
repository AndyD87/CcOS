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
 * @brief     Class CcTaskbarItem
 */
#ifndef H_CCTASKBARITEM_H_
#define H_CCTASKBARITEM_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcString.h"
#include "CcButton.h"
#include "CcMenuReverse.h"
#include "CcMenuItem.h"

/**
 * @brief Item for Placing a Symbol inside the Taskbar
 */
class CcGuiSHARED CcTaskbarItem : public CcMenuItem {
public:
  /**
   * @brief Constructor
   */
  CcTaskbarItem(CcWidget* rParent, const CcString& name = "");

  /**
   * @brief Destructor
   */
  virtual ~CcTaskbarItem();

  /**
   * @brief Draw Next Stage of Menu, and create Buttons for Input.
   */
  virtual void drawMenuTree();

  /**
   * @brief Hide all Displayed Elements of next Stages
   */
  virtual void hideMenuTree();
};

#endif // H_CcTASKBARITEM_H_
