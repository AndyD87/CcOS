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
 * @brief     Windows implementations for class CcPushButton
 */
#include "CcPushButton.h"
#include "CcWindowsGuiUtil.h"
#include <afxbutton.h>

class CcPushButtonPrivate
{
public:
  CcString sLabel;
};


CcPushButton::CcPushButton(const CcWidgetHandle& rParent) :
  CcButton(rParent),
  m_pPrivate(new CcPushButtonPrivate)
{
  getSubSysHandle().cast<CMFCButton>()->SetButtonStyle(BS_PUSHBUTTON);
  setBorderColor(CcColor(
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_R,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_G,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_B));
  setBorderSize(CCPUSHBUTTON_DEFAULT_BORDERSIZE);
}

CcPushButton::~CcPushButton() {
}

void CcPushButton::draw(bool bFlush)
{
  CCUNUSED(bFlush);
  drawButton();
}

void CcPushButton::drawButton()
{
  int32 uiTempHeight = 0;
  int32 uiTempWidth  = 0;

  uiTempWidth  = getWidth() - uiTempWidth;
  uiTempHeight = getHeight() - uiTempHeight;
  uiTempWidth  = uiTempWidth  / 2;
  uiTempHeight = uiTempHeight / 2;
  
  drawBackground(getStyle()->oBackgroundColor);
  drawBorder(getStyle()->oBorderColor, getStyle()->uBorderSize);
}

void CcPushButton::setText(const CcString& sString )
{
  m_pPrivate->sLabel = sString;
  getSubSysHandle().cast<CMFCButton>()->SetWindowTextW(TOLPCWSTR(m_pPrivate->sLabel));
}

const CcString& CcPushButton::getString()
{
  return m_pPrivate->sLabel;
}
