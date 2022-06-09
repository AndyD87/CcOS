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
 * @brief     Class CcCrossButton
 */
#include "CcCrossButton.h"
#include "CcPainter.h"

CcCrossButton::CcCrossButton(CcWidget* rParent):
  CcButton(rParent)
{

}

CcCrossButton::~CcCrossButton()
{

}

void CcCrossButton::setCrossColor(uint8 uiR, uint8 uiG, uint8 uiB)
{
  m_uiR = uiR;
  m_uiG = uiG;
  m_uiB = uiB;
}

void CcCrossButton::setCrossSize(uint8 size)
{
  m_uiSize = size;
}

void CcCrossButton::drawCross()
{
  uint32 startX = (getWidth() - m_uiSize) / 2;
  uint32 startY = (getHeight() - m_uiSize) / 2;
  for(uint32 i=0; i < m_uiSize; i++)
  {
    CcPainter Painter(this);
    Painter.setColor(m_uiR, m_uiG, m_uiB);
    Painter.drawLine(CcPoint(startX + i, 0), CcPoint(startX + i, getWidth() - 1));
    Painter.drawLine(CcPoint(0, startY + i), CcPoint(getWidth() - 1, startX + i));
  }
}