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
 * @brief     Implementation of Class CcMenuItem
 */
#include "CcMenuItem.h"

CcMenuItem::CcMenuItem(CcWidgetHandle parent, CcString name) :
  m_parentWidget(parent),
  m_Button(0),
  m_Name(name),
  m_ReverseList(0)
{
}

CcMenuItem::~CcMenuItem() {
  hideMenuTree();
  clear();
}

CcMenuItem* CcMenuItem::createItem(const CcString& name)
{
  CcMenuItem *ret = new CcMenuItem( m_parentWidget, name); 
  CCMONITORNEW(ret);
  ret->setReverseList(m_ReverseList);
  addSubTree(ret);
  return ret;
}

void CcMenuItem::addItem(CcMenuItem *toAdd)
{
  addSubTree(toAdd);
}

void CcMenuItem::delItem(CcMenuItem *toDel
  ){
  delSubTree(toDel);
}

void CcMenuItem::setValue(const CcString& toSet)
{
  m_Name = toSet;
}

CcMenuItem* CcMenuItem::at(uint16 pos)
{
  return (CcMenuItem*)getAt(pos);
}

CcString* CcMenuItem::getValue(void)
{
  return &m_Name;
}

void CcMenuItem::setReverseList(CcMenuReverse *list)
{
  m_ReverseList = list;
}

CcMenuReverse* CcMenuItem::getReverseList(void)
{
  return m_ReverseList;
}

CcPushButton* CcMenuItem::createButton(uint16 startX, uint16 startY)
{
  m_Button = new CcPushButton(m_parentWidget); 
  CCMONITORNEW(m_Button);
  m_Button->setText(m_Name);
  m_Button->setPos(CcPoint(startX, startY));
  m_Button->setBackgroundColor(CcColor(0x33, 0x33, 0x33));
  m_Button->setBorderColor(CcColor(0x2d, 0x2d, 0x30));
  m_Button->setBorderSize(0);
  m_Button->setSize(CcSize(100, 20));
  m_Button->draw();
  return m_Button;
}

CcPushButton* CcMenuItem::getButton(void)
{
  return m_Button;
}

void CcMenuItem::drawMenuTree(void* pParam)
{
  CCUNUSED(pParam);
  for (uint16 i = 0; i < size(); i++)
  {
    at(i)->createButton(100 + m_Button->getPosX(), m_Button->getPosY() + m_Button->getHeight()*i);
  }
}

void CcMenuItem::hideMenuTree(void)
{
  for (uint16 i = 0; i < size(); i++)
  {
    at(i)->hideMenuTree();
  }
  CCDELETE(m_Button);
}
