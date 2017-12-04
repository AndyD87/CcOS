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
 * @brief     Class CcPushButton
 */
#include "CcPushButton.h"

CcPushButton::CcPushButton(CcWidgetHandle parent) :
  CcButton(parent),
  m_TextWidget(parent)
{
  setBorderColor(CcColor(
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_R,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_G,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_B));
  setBorderSize(CCPUSHBUTTON_DEFAULT_BORDERSIZE);
  m_TextWidget.setTextOffset(CCPUSHBUTTON_DEFAULT_BORDERSIZE,CCPUSHBUTTON_DEFAULT_BORDERSIZE);
}

CcPushButton::~CcPushButton() {
}

void CcPushButton::draw( void )
{
  drawButton();
}

void CcPushButton::drawButton( void )
{
  uint16 uiTempHeight;
  uint16 uiTempWidth;
  m_TextWidget.getTextSize(&uiTempWidth, &uiTempHeight);
  uiTempWidth  = getWidth() - uiTempWidth;
  uiTempHeight = getHeight() - uiTempHeight;
  uiTempWidth  = uiTempWidth  / 2;
  uiTempHeight = uiTempHeight / 2;
  m_TextWidget.setTextOffset( getPosX() + uiTempWidth, getPosY() + uiTempHeight );
  m_TextWidget.setBackgroundColor(getBackgroundColor());
  drawBackground();
  drawBorder();
  m_TextWidget.drawString();
}

void CcPushButton::setText(const CcString& sString )
{
  m_TextWidget.setString(sString);
}

const CcString& CcPushButton::getString(void)
{
  return m_TextWidget.getString();
}
