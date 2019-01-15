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
 * @brief     Implementation of Class CcTaskbar
 */
#include "CcTaskbar.h"
#include "CcText.h"
#include "CcPushButton.h"
#include "CcMenu.h"

CcTaskbar::CcTaskbar(const CcWidgetHandle& rParent) :
  CcWidget(rParent),
  m_DummyCenter(0),
  m_Menu(0)
{
  if (rParent != 0)
  {
    setSize(CcSize(rParent->getWidth(), 20));
    setBackgroundColor(CcColor(0x2d, 0x2d, 0x30));
    setPos(CcPoint(0, 0));
  }
  m_Center=0;
}

CcTaskbar::~CcTaskbar() {
  hideDummy();
}

CcMenu* CcTaskbar::createMenu(void )
{
  m_Menu = new CcMenu(getParent());
  CCMONITORNEW(m_Menu);
  return m_Menu;
}

void CcTaskbar::drawMenu(void)
{
  for (uint16 i = 0; i < m_Menu->size(); i++)
  {
    CcTaskbarItem *temp = m_Menu->at(i);
    temp->createButton(100 * i, 0);
  }
}

void CcTaskbar::hideMenu(void)
{
  for (uint16 i = 0; i < m_Menu->size(); i++)
  {
    m_Menu->at(i)->hideMenuTree();
  }
  m_Center->draw();
}

void CcTaskbar::drawDummy(void)
{
  CCDELETE(m_DummyCenter);
  m_DummyCenter = new CcButton(getParent()); 
  CCMONITORNEW(m_DummyCenter);
  m_DummyCenter->setPos(m_Center->getPos());
  m_DummyCenter->setSize(m_Center->getSize());
}

void CcTaskbar::hideDummy(void)
{
  CCDELETE(m_DummyCenter);
}

void CcTaskbar::drawTray(void)
{

}

void CcTaskbar::setCenterWindow(const CcWidgetHandle& center)
{
  m_Center = center;
}

void CcTaskbar::DeleteDraws(void* pParam)
{
  CCUNUSED(pParam);
  if (m_Menu) m_Menu->getReverseList()->clear();
  hideDummy();
  hideMenu();
}
