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
 * @brief     Class CcMenuItem
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcString.h"
#include "CcTree.h"
#include "CcButton.h"
#include "CcMenuReverse.h"

/**
 * @brief Item within the Menue tree
 */
class CcGuiSHARED CcMenuItem : public CcObject, public CcTree
{
public:
  /**
   * @brief Constructor
   */
  CcMenuItem(CcWidget* rParent, CcString name = "");

  /**
   * @brief Destructor
   */
  virtual ~CcMenuItem();

  /**
   * @brief Create a new menue item and add it to subtree.
   * @param name: Value of Item
   */
  CcMenuItem* createItem(const CcString& name = "");

  /**
   * @brief Add a Item to Menu-Tree
   * @param toAdd: Item to Add
   */
  void addItem(CcMenuItem *toAdd);

  /**
   * @brief Remove a item from Menu-Tree
   * @param toDel: Item to Delete
   */
  void delItem(CcMenuItem *toDel);

  /**
   * @brief Return Item on Position
   * @param pos: Requested Position
   * @return stored Item
   */
  CcMenuItem* at(size_t pos);

  //! @return Get value name of item
  CcString* getValue();
  //! @return Get reverse list of parents
  CcMenuReverse* getReverseList();
  //! @return Get current button of list item
  CcButton* getButton();

  //! @param toSet: Set name of item
  void setValue(const CcString& toSet);
  //! @param list: Upate reverse list
  void setReverseList(CcMenuReverse *list);

  /**
   * @brief Create button for this Item on specific location
   * @param startX: X Position of button
   * @param startY: Y Position of button
   * @return Created button
   */
  CcButton* createButton(uint32 startX, uint32 startY);

  /**
   * @brief Draw Next Stage of Menu, and create Buttons for Input.
   */
  virtual void drawMenuTree();

  /**
   * @brief Hide all Displayed Elements of next Stages
   */
  virtual void hideMenuTree();

  /**
   * @brief Incomming on click event
   * @param pos: Point of click on button
   */
  void onClick(CcPoint *pos);

private:
  CcWidget* m_parentWidget;
  CcButton  *m_Button;
  CcString       m_Name;
  CcMenuReverse *m_ReverseList;
};
