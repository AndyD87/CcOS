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
 * @brief     Class CcWidget
 */

#include "CcWidget.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcInputEvent.h"

CcWidget::CcWidget(CcWidget* parent) :
  m_Parent(parent)
{
  setSize(parent->getSize());
  if (m_Parent != nullptr)
  {
    m_Parent->registerChild(this);
  }
}

CcWidget::CcWidget(int16 iPosX, int16 iPosY, uint16 uiWidth, uint16 uiHeight, CcWidget* parent) :
  m_oWindowRect(iPosX, iPosY, uiWidth, uiHeight),
  m_Parent(parent)
{
  if (m_Parent != nullptr)
  {
    m_Parent->registerChild(this);
  }
}

CcWidget::~CcWidget()
{
  if (m_Parent != nullptr)
    m_Parent->removeChild(this);
  for (CcWidget* pWidget : getChildList())
  {
    pWidget->setParent(nullptr);
  }
}

void CcWidget::setPos(const CcPoint& oPoint)
{
  m_oWindowRect = oPoint;
  onRectangleChanged();
}

void CcWidget::setSize(const CcSize& oSize)
{
  m_oWindowRect = oSize;
  onRectangleChanged();
}

void CcWidget::setWindowRect(const CcRectangle& oRect)
{
  m_oWindowRect = oRect;
  onRectangleChanged();
}

CcRectangle CcWidget::getInnerRect()
{
  CcRectangle oRectangle;
  oRectangle.setWidth(getWindowRect().getWidth() - (2 * m_uiBorderSize));
  oRectangle.setHeight(getWindowRect().getHeight() - (2 * m_uiBorderSize));
  return oRectangle;
}

void CcWidget::drawBorder(void )
{
  CcPainter Painter(this);
  Painter.setColor(m_BorderColor);
  CcRectangle oRectangle = getWindowRect();
  oRectangle.setPoint(CcPoint(0, 0));
  Painter.drawRectangle(oRectangle, m_uiBorderSize, false);
}

void CcWidget::draw( void )
{
  drawAllChilds();
}

void CcWidget::drawBackground( void )
{
  CcRectangle oInnerRect = getInnerRect();
  uint32 uiTemp = oInnerRect.getAreaSize();
  if (setPixelArea(oInnerRect))
  {
    for (uint32 i = 0; i < uiTemp; i++)
      drawPixel(m_BackgroundColor);
  }
}

void CcWidget::drawPixel(const CcColor& oColor)
{
  getWindow()->drawPixel(oColor);
}

void CcWidget::drawAllChilds(void)
{
  // draw childs
  for (CcWidget* pWidget : getChildList())
  {
    pWidget->draw();
  }
}

bool CcWidget::setPixelArea(const CcRectangle& oRectangle)
{
  if (m_Parent != nullptr)
  {
    CcRectangle oForwardRec(oRectangle);
    oForwardRec += getWindowRect();
    return m_Parent->setPixelArea(oForwardRec);
  }
  else
  {
    return getWindow()->setPixelArea(oRectangle);
  }
}

void CcWidget::registerChild(CcWidget *oChildWidget)
{
  m_oChildList.append(oChildWidget);
}

CcWidget* CcWidget::getHitTest(const CcPoint& oPointToFind)
{
  for (size_t i = getChildList().size(); i > 0; i--)
  {
    CcWidget* oTempWidget = getChildList()[i - 1];
    if (oTempWidget->getWindowRect().checkPoint(oPointToFind))
    {
      CcPoint oNewPoint = oPointToFind - oTempWidget->getPos();
      return oTempWidget->getHitTest(oNewPoint);
    }
  }
  return this;
}

void CcWidget::removeChild(CcWidget *oChildWidget)
{
  m_oChildList.removeItem(oChildWidget);
}

void CcWidget::onRectangleChanged(void)
{

}

void CcWidget::onBackgroundChanged(void)
{

}
