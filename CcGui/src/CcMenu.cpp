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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcMenu
 */
#include "CcMenu.h"
#include "CcMenuItem.h"

CcMenu::CcMenu(CcWidget* parentWidget, CcTaskbar* parentTaskbar) :
  m_parentWidget(parentWidget),
  m_Taskbar(parentTaskbar)
{

}

CcMenu::~CcMenu() {
}

void CcMenu::addItem(CcTaskbarItem *entry)
{
  m_MenuTree.append(entry);
}

bool CcMenu::delItem(CcTaskbarItem *toDel)
{
  m_MenuTree.removeItem(toDel);
  return true;
}

CcMenuReverse *CcMenu::getReverseList(void)
{
  return &m_RevList;
}

CcTaskbarItem *CcMenu::createItem(const CcString& name)
{
  CcTaskbarItem *newItem = new CcTaskbarItem(m_parentWidget, name); CCMONITORNEW(newItem);
  newItem->setReverseList(&m_RevList);
  m_MenuTree.append(newItem);
  return newItem;
}

size_t CcMenu::size(void)
{
  return m_MenuTree.size();
}

CcTaskbarItem* CcMenu::at(uint16 pos)
{
  return m_MenuTree.at(pos);
}
