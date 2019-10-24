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
 * @brief     Class CcWidget
 */

#include "CcWidget.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcInputEvent.h"

class CcWidget::CPrivate
{
public:
  CcWidgetHandle m_pThis;
  CcWidgetHandle m_Parent;
  CcSubSysHandle m_hSubSys;
  CcList<CcWidgetHandle> m_oChildList;
  bool m_bCustomPaint = false;
};

CcWidget::CcWidget(const CcWidgetHandle& parent)
{
  initWidget(parent);
}

CcWidget::CcWidget(const CcRectangle& oWindowRect, const CcWidgetHandle& rParent) :
  m_hThisHandle(this),
  m_oWindowRect(oWindowRect)
{
  initWidget(rParent);
}

CcWidget::CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, const CcWidgetHandle& parent) :
  m_oWindowRect(iPosX, iPosY, uiWidth, uiHeight)
{
  initWidget(parent);
}

CcWidget::~CcWidget()
{
  if (m_pPrivate->m_Parent != nullptr)
    m_pPrivate->m_Parent->removeChild(this);
  for (const CcWidgetHandle& pWidget : getChildList())
  {
    pWidget->setParent(nullptr);
  }
}

void CcWidget::setCustomPainting(bool bEnable)
{
  m_pPrivate->m_bCustomPaint = bEnable;
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

void CcWidget::setBackgroundColor(const CcColor& oColor)
{
  getStyle()->oBackgroundColor = oColor;
}

void CcWidget::setForegroundColor(const CcColor& oColor)
{
  getStyle()->oForegroundColor = oColor;
}

void CcWidget::setWindowState(EWindowState eWindowState)
{
  getWindow().cast<CcWindow>()->setWindowState(eWindowState);
}


/**
* @brief Get a Subsystem defined Handle
* @return Handle as Pointer, Type is defined by Subsystem.
*/
CcSubSysHandle& CcWidget::getSubSysHandle()
{
  return m_pPrivate->m_hSubSys;
}

void CcWidget::setWindowRect(const CcRectangle& oRect)
{
  m_oWindowRect = oRect;
  onRectangleChanged();
}

CcRectangle CcWidget::getInnerRect()
{
  CcRectangle oRectangle = getWindowRect();
  oRectangle.addBorderSize(-getStyle()->uBorderSize);
  return oRectangle;
}

void CcWidget::drawBorder(const CcColor& oColor, uint32 uiSize )
{
  CcPainter Painter(this);
  Painter.setColor(oColor);
  CcRectangle oRectangle = getWindowRect();
  oRectangle.setPoint(CcPoint(0, 0));
  Painter.drawRectangle(oRectangle, uiSize, false);
}

void CcWidget::draw(bool bDoFlush)
{
  drawBackground(getStyle()->oBackgroundColor);
  drawAllChilds();
  if(bDoFlush)
  {
    flush();
  }
}

void CcWidget::flush()
{

}

void CcWidget::event(EGuiEvent eEvent, void* pEventData)
{
  onEvent(eEvent, pEventData);
  if (eEvent >= EGuiEvent::WindowEvent && eEvent <= EGuiEvent::WindowEventMax)
  {
    onWindowEvent(eEvent);
  }
  else if (eEvent >= EGuiEvent::MouseEvent && eEvent <= EGuiEvent::MouseEventMax)
  {
    onMouseEvent(eEvent, static_cast<CcMouseEvent*>(pEventData));
  }
  else if (eEvent >= EGuiEvent::KeyEvent && eEvent <= EGuiEvent::KeyEventMax)
  {
    onKeyEvent(eEvent, static_cast<CcKeyEvent*>(pEventData));
  }
  m_oEventHandler.call(eEvent, pEventData);
}

void CcWidget::registerOnEvent(EGuiEvent eEvent, IEvent* eEventHandle)
{
  m_oEventHandler.add(eEvent, eEventHandle);
}

void CcWidget::removeOnEvent(EGuiEvent eEvent, CcObject* pObject)
{
  m_oEventHandler.removeObject(eEvent, pObject);
}

void CcWidget::onEvent(EGuiEvent eEvent, void *pMouseEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcWidget::onMouseEvent(EGuiEvent eEvent, CcMouseEvent* pMouseEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcWidget::onKeyEvent(EGuiEvent eEvent, CcKeyEvent* pKeyEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pKeyEvent);
}

void CcWidget::onWindowEvent(EGuiEvent eWindowEvent)
{
  CCUNUSED(eWindowEvent);
}

void CcWidget::setSubSystemHandle(CcSubSysHandle hSubSystem)
{
  m_pPrivate->m_hSubSys = hSubSystem;
}

void CcWidget::drawBackground(const CcColor& oColor)
{
  CcRectangle oInnerRect = getInnerRect();
  uint32 uiTemp = oInnerRect.getAreaSize();
  if (setPixelArea(oInnerRect))
  {
    for (uint32 i = 0; i < uiTemp; i++)
      drawPixel(oColor);
  }
}

void CcWidget::drawPixel(const CcColor& oColor, uint64 uiNumber)
{
  getWindow()->drawPixel(oColor,uiNumber);
}

void CcWidget::drawAllChilds()
{
  // draw childs
  for (const CcWidgetHandle& pWidget : getChildList())
  {
    pWidget->draw();
  }
}

CcWindowHandle& CcWidget::getWindow()
{
  return m_pPrivate->m_Parent->getWindow();
}


EWindowState CcWidget::getWindowState()
{
  return getWindow().cast<CcWindow>()->getState();
}

CcWidgetHandle& CcWidget::getParent()
{
  return m_pPrivate->m_Parent;
}

void CcWidget::setParent(const CcWidgetHandle& oParent)
{
  m_pPrivate->m_Parent = oParent;
}

bool CcWidget::setPixelArea(const CcRectangle& oRectangle)
{
  if (m_pPrivate->m_Parent != nullptr)
  {
    CcRectangle oForwardRec(oRectangle);
    oForwardRec += getWindowRect();
    return m_pPrivate->m_Parent->setPixelArea(oForwardRec);
  }
  else
  {
    return getWindow()->setPixelArea(oRectangle);
  }
}

void CcWidget::registerChild(const CcWidgetHandle& oChildWidget)
{
  m_pPrivate->m_oChildList.append(oChildWidget);
}

CcWidgetHandle& CcWidget::getHitTest(const CcPoint& oPointToFind)
{
  for (size_t i = getChildList().size(); i > 0; i--)
  {
    CcWidgetHandle oTempWidget = getChildList()[i - 1];
    if (oTempWidget->getWindowRect().checkPoint(oPointToFind))
    {
      CcPoint oNewPoint = oPointToFind - oTempWidget->getPos();
      return oTempWidget->getHitTest(oNewPoint);
    }
  }
  return m_pPrivate->m_pThis;
}

void CcWidget::removeChild(const CcWidgetHandle& oChildWidget)
{
  m_pPrivate->m_oChildList.removeItem(oChildWidget);
}

const CcList<CcWidgetHandle>& CcWidget::getChildList()
{
  return m_pPrivate->m_oChildList;
}

void CcWidget::onRectangleChanged()
{
}

void CcWidget::onBackgroundChanged()
{
}

void CcWidget::onForegroundChanged()
{
}

void CcWidget::initWidget(const CcWidgetHandle& rParent)
{
  CCDELETE(m_pPrivate);
  CCNEW(m_pPrivate, CPrivate);
  if(m_pPrivate != nullptr)
  {
    m_pPrivate->m_Parent = rParent;
    if (rParent.isValid())
    {
      m_pPrivate->m_Parent->registerChild(this);
    }
    m_pPrivate->m_pThis = this;
  }
}
