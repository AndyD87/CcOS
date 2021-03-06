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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcMenu
 */
#include "CcMenu.h"
#include "CcMenuItem.h"

CcMenu::CcMenu(CcWidget* rParentWidget) :
  m_parentWidget(rParentWidget)
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

CcMenuReverse *CcMenu::getReverseList()
{
  return &m_RevList;
}

CcTaskbarItem *CcMenu::createItem(const CcString& name)
{
  CCNEWTYPE(newItem, CcTaskbarItem, m_parentWidget, name);
  newItem->setReverseList(&m_RevList);
  m_MenuTree.append(newItem);
  return newItem;
}

size_t CcMenu::size()
{
  return m_MenuTree.size();
}

CcTaskbarItem* CcMenu::at(uint16 pos)
{
  return m_MenuTree.at(pos);
}
