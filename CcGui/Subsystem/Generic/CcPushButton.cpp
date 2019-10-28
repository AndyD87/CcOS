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

class CcPushButton::CPrivate
{
public:
  CcText* m_pTextWidget = nullptr;
}; 

CcPushButton::CcPushButton(CcWidget* parent) :
  CcButton(parent)
{
  CCNEW(m_pPrivate, CPrivate);
  CCNEW(m_pPrivate->m_pTextWidget, CcText, this);
  setBorderColor(CcColor(
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_R,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_G,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_B));
  setBorderSize(CCPUSHBUTTON_DEFAULT_BORDERSIZE);
  m_pPrivate->m_pTextWidget->setTextOffset(CCPUSHBUTTON_DEFAULT_BORDERSIZE,CCPUSHBUTTON_DEFAULT_BORDERSIZE);
}

CcPushButton::~CcPushButton() 
{
  CCDELETE(m_pPrivate->m_pTextWidget);
  CCDELETE(m_pPrivate);
}

void CcPushButton::draw(bool bDoFlush)
{
  CCUNUSED(bDoFlush);
  drawButton();
}

void CcPushButton::drawButton()
{
  CcSize oTextSize = m_pPrivate->m_pTextWidget->getSize();
  uint16 uiTempHeight = static_cast<uint16>(oTextSize.getHeight());
  uint16 uiTempWidth  = static_cast<uint16>(oTextSize.getWidth());
  uiTempWidth  = static_cast<uint16>(getWidth()) - uiTempWidth;
  uiTempHeight = static_cast<uint16>(getHeight()) - uiTempHeight;
  uiTempWidth  = uiTempWidth  / 2;
  uiTempHeight = uiTempHeight / 2;
  m_pPrivate->m_pTextWidget->setTextOffset(static_cast<uint16>(getPos().getX()) + uiTempWidth, static_cast<uint16>(getPos().getY()) + uiTempHeight );
  m_pPrivate->m_pTextWidget->setBackgroundColor(getBackgroundColor());
  drawBackground(getBackgroundColor());
  drawBorder(getBorderColor(), getBorderSize());
  m_pPrivate->m_pTextWidget->drawString();
}

void CcPushButton::setText(const CcString& sString )
{
  m_pPrivate->m_pTextWidget->setText(sString);
}

const CcString& CcPushButton::getString()
{
  return m_pPrivate->m_pTextWidget->getText();
}
