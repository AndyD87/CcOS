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
 * @brief     Class CcCrossButton
 */
#include "CcCrossButton.h"
#include "CcPainter.h"

CcCrossButton::CcCrossButton(CcWidgetHandle parent):
  CcButton(parent)
{

}

CcCrossButton::~CcCrossButton()
{

}

void CcCrossButton::setCrossColor(uint8 R, uint8 G, uint8 B)
{
  m_R = R;
  m_G = G;
  m_B = B;
}

void CcCrossButton::setCrossSize(uint8 size)
{
  m_CrossSize = size;
}

void CcCrossButton::drawCross( void )
{
  uint16 startX = (getWidth() - m_CrossSize) / 2;
  uint16 startY = (getHeight() - m_CrossSize) / 2;
  for(uint16 i=0; i < m_CrossSize; i++)
  {
    CcPainter Painter(getWindowWidget());
    Painter.setColor(m_R, m_G, m_B);
    Painter.drawLine(CcPoint(startX + i, 0), CcPoint(startX + i, getWidth() - 1));
    Painter.drawLine(CcPoint(0, startY + i), CcPoint(getWidth() - 1, startX + i));
  }
}
