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
 * @brief     Class CcWidget
 */

#include "CcWidget.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcInputEvent.h"
#include "CcWindowsGuiUtil.h"
#include "CcBitmap.h"
#include "CcCWnd.h"
#include "Style/CcStyleWidget.h"

class CcWidget::CPrivate
{
public:
  CcCWnd*         pWidget;
  void*           hSubSystemHandle;
  CcWidget*       m_Parent = nullptr;
  CcList<CcWidget*> m_oChildList;
  CcRectangle     m_oWindowRect;
  CcGuiEventMap   m_oEventHandler;
  CcStyleWidget   oStyle;
};

CcWidget::CcWidget(CcWidget* rParent)
{
  initWidget(rParent);
}

CcWidget::CcWidget(const CcRectangle& oWindowRect, CcWidget* rParent)
{
  initWidget(rParent);
  m_pPrivate->m_oWindowRect = oWindowRect;
}

CcWidget::CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, CcWidget* rParent)
{
  initWidget(rParent);
  m_pPrivate->m_oWindowRect.set(iPosX, iPosY, uiWidth, uiHeight);
}

CcWidget::~CcWidget()
{
  if (m_pPrivate!=nullptr)
  {
    if (m_pPrivate->m_Parent != nullptr)
      m_pPrivate->m_Parent->removeChild(this);
    if (m_pPrivate->pWidget != nullptr)
    {
      m_pPrivate->pWidget->DestroyWindow();
      CCMONITORDELETE(m_pPrivate->pWidget);
      delete m_pPrivate->pWidget;
      m_pPrivate->pWidget = nullptr;
    }
    for (CcWidget* pWidget : getChildList())
    {
      pWidget->setParent(nullptr);
    }
  }
  CCDELETE(m_pPrivate);
}

void CcWidget::setPos(const CcPoint& oPoint)
{
  if (m_pPrivate->m_oWindowRect != oPoint)
  {
    m_pPrivate->m_oWindowRect = oPoint;
    onRectangleChanged();
  }
}

void CcWidget::setSize(const CcSize& oSize)
{
  if (m_pPrivate->m_oWindowRect != oSize)
  {
    m_pPrivate->m_oWindowRect = oSize;
    onRectangleChanged();
  }
}

void CcWidget::setBackgroundColor(const CcColor& oColor)
{
  getStyle().oBackgroundColor = oColor;
  onBackgroundChanged();
}

void CcWidget::setForegroundColor(const CcColor& oColor)
{
  getStyle().oForegroundColor = oColor;
  onForegroundChanged();
}

void CcWidget::setRectangle(const CcRectangle& oRect)
{
  m_pPrivate->m_oWindowRect = oRect;
  onRectangleChanged();
}

CcWidget* CcWidget::getHitTest(const CcPoint& oPointToFind)
{
  for (size_t i = getChildList().size(); i > 0; i--)
  {
    CcWidget* oTempWidget = getChildList()[i - 1];
    if (oTempWidget->getRectangle().checkPoint(oPointToFind))
    {
      CcPoint oNewPoint = oPointToFind - oTempWidget->getPos();
      return oTempWidget->getHitTest(oNewPoint);
    }
  }
  return this;
}

const CcRectangle& CcWidget::getRectangle() const
{
  return m_pPrivate->m_oWindowRect;
}

CcRectangle CcWidget::getInnerRect() const
{
  CcRectangle oRect(getRectangle());
  oRect.addBorderSize(getStyle().uBorderSize);
  return oRect;
}

void* CcWidget::getSubSysHandle()
{
  return m_pPrivate->hSubSystemHandle;
}

void CcWidget::draw(bool bDoFlush)
{
  drawBackground(getStyle().oBackgroundColor);
  drawBorder(getStyle().oBorderColor, getStyle().uBorderSize);
  drawAllChilds();
  if (bDoFlush)
  {
    flush();
  }
}

void CcWidget::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  if (m_pPrivate->pWidget != nullptr)
  {
    m_pPrivate->pWidget->drawPixel(oPixel, uiNumber);
  }
  else
  {
    CCDEBUG("Where do you want to paint");
  }
}

void CcWidget::flush()
{
  if (m_pPrivate->hSubSystemHandle != nullptr)
  {
    CWNDHANDLE(this)->RedrawWindow();
    CWNDHANDLE(this)->Invalidate();
    CWNDHANDLE(this)->UpdateWindow();
  }
}

CcRectangle CcWidget::getInnerRect()
{
  CcRectangle oRectangle(getRectangle().getSize());
  oRectangle.addWidth( - (2 * getStyle().uBorderSize));
  oRectangle.addHeight( - (2 * getStyle().uBorderSize));
  return oRectangle;
}

CcWindow* CcWidget::getWindow()
{
  if (m_pPrivate->m_Parent == nullptr)
  {
    return CcWindow::Null;
  }
  else
  {
    return m_pPrivate->m_Parent->getWindow();
  }
}

bool CcWidget::setPixelArea(const CcRectangle& oArea)
{
  bool bRet = false;
  if (m_pPrivate->pWidget)
  {
    if ((getInnerRect().getWidth() > oArea.getX() &&
      getInnerRect().getHeight() > oArea.getY()) ||
      (getInnerRect().getWidth() > oArea.getBottomRightCorner().getX() &&
        getInnerRect().getHeight() > oArea.getBottomRightCorner().getY()))
    {
      bRet = m_pPrivate->pWidget->setPixelArea(oArea);
    }
    else
      CCDEBUG("Out of area");
  }
  else
  {
    CCDEBUG("Where do you want to paint");
  }
  return bRet;
}

void CcWidget::drawAllChilds()
{
  // draw childs
  for (CcWidget* pWidget : getChildList())
  {
    pWidget->draw();
  }
}

EWindowState CcWidget::getWindowState()
{
  return getWindow()->getState();
}

int32 CcWidget::getWidth() const
{
  return getRectangle().getWidth();
}

int32 CcWidget::getHeight() const
{
  return getRectangle().getHeight();
}

const CcPoint& CcWidget::getPos()
{
  return m_pPrivate->m_oWindowRect;
}

const CcColor& CcWidget::getBackgroundColor()
{
  return m_pPrivate->oStyle.oBackgroundColor;
}

const CcColor& CcWidget::getForegroundColor()
{
  return m_pPrivate->oStyle.oForegroundColor;
}

CcWidget* CcWidget::getParent()
{
  return m_pPrivate->m_Parent;
}

void CcWidget::setParent(CcWidget* rParent)
{
  if (rParent == nullptr)
  {
    m_pPrivate->m_Parent = nullptr;
  }
  else if (m_pPrivate->m_Parent != rParent)
  {
    if(m_pPrivate->m_Parent != nullptr)
      m_pPrivate->m_Parent->removeChild(this);
    m_pPrivate->m_Parent = rParent;
    m_pPrivate->m_Parent->registerChild(this);
    if (m_pPrivate->m_Parent->getSubSysHandle() != nullptr &&
        m_pPrivate->hSubSystemHandle != nullptr)
    {
      CWNDHANDLE(this)->SetParent(CWNDHANDLE(m_pPrivate->m_Parent));
    }
  }
}

void CcWidget::setStyle(const CcStyleWidget& oStyleSheet)
{
  m_pPrivate->oStyle = oStyleSheet;
  CcStyle::EType eType = EEventType::StyleBorderStyle;
  event(EEventType::WidgetStyleChanged, &eType);
  eType = EEventType::StyleForegroundColor;
  event(EEventType::WidgetStyleChanged, &eType);
  eType = EEventType::StyleBackgroundColor;
  event(EEventType::WidgetStyleChanged, &eType);
}

void CcWidget::setWindowState(EWindowState eWindowState)
{
  getWindow()->setWindowState(eWindowState);
}

void CcWidget::setSubSystemHandle(void* hSubSystem)
{
  if (m_pPrivate->pWidget != nullptr &&
      m_pPrivate->hSubSystemHandle == m_pPrivate->pWidget)
  {
    m_pPrivate->pWidget->DestroyWindow();
    CCDELETE(m_pPrivate->pWidget);
  }
  m_pPrivate->hSubSystemHandle = hSubSystem;
}

const CcColor& CcWidget::getBorderColor()
{
  return m_pPrivate->oStyle.oBorderColor;
}

void CcWidget::setBorderColor(const CcColor& oColor)
{
  getStyle().oBorderColor = oColor;
  CcStyle::EType eType = EEventType::StyleBorderStyle;
  event(EEventType::WidgetStyleChanged, &eType);
}

void CcWidget::setBorderSize(uint16 uiSize)
{
  m_pPrivate->oStyle.uBorderSize = uiSize;
  CcStyle::EType eType = EEventType::StyleBorderStyle;
  event(EEventType::WidgetStyleChanged, &eType);
}

uint32 CcWidget::getBorderSize()
{
  return m_pPrivate->oStyle.uBorderSize;
}

const CcSize& CcWidget::getSize()
{
  return m_pPrivate->m_oWindowRect;
}

CcStyleWidget& CcWidget::getStyle()
{
  return m_pPrivate->oStyle;
}

const CcStyleWidget& CcWidget::getStyle() const
{
  return m_pPrivate->oStyle;
}

void CcWidget::registerChild(CcWidget* oChildWidget)
{
  m_pPrivate->m_oChildList.append(oChildWidget);
  if (oChildWidget->getSubSysHandle() != nullptr)
  {

  }
}

void CcWidget::drawBackground(const CcColor& oColor)
{
  CcRectangle oInnerRect(getSize());
  if (setPixelArea(oInnerRect))
  {
    drawPixel(oColor, oInnerRect.getAreaSize());
  }
}

void CcWidget::drawBorder(const CcColor& oColor, uint32 uiSize)
{
  CcPainter Painter(this);
  Painter.setColor(oColor);
  CcRectangle oRectangle(getSize());
  Painter.drawRectangle(oRectangle, uiSize, false);
}

void CcWidget::onEvent(EEventType eEvent, void *pMouseEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcWidget::onMouseEvent(CcMouseEvent* pEventData)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcWidget::onKeyEvent(CcKeyEvent* pEventData)
{
  CCUNUSED(eEvent);
  CCUNUSED(pKeyEvent);
}

void CcWidget::onWindowEvent(EEventType eWindowEvent)
{
  CCUNUSED(eWindowEvent);
}

void CcWidget::setCustomPainting(bool bEnable)
{
  if (bEnable)
  {
    if (m_pPrivate->pWidget)
    {
      m_pPrivate->pWidget->enablePainting();
    }
  }
}

void CcWidget::removeChild(CcWidget* oChildWidget)
{
  m_pPrivate->m_oChildList.removeItem(oChildWidget);
}

const CcList<CcWidget*>& CcWidget::getChildList()
{
  return m_pPrivate->m_oChildList;
}

void CcWidget::event(CcInputEvent* pEventData)
{
  onEvent(eEvent, pEventData);
  if (eEvent >= EEventType::WindowEvent && eEvent <= EEventType::WindowEventMax)
  {
    onWindowEvent(eEvent);
  }
  else if (eEvent >= EEventType::MouseEvent && eEvent <= EEventType::MouseEventMax)
  {
    onMouseEvent(eEvent, static_cast<CcMouseEvent*>(pEventData));
  }
  else if (eEvent >= EEventType::KeyEvent && eEvent <= EEventType::KeyEventMax)
  {
    onKeyEvent(eEvent, static_cast<CcKeyEvent*>(pEventData));
  }
  m_pPrivate->m_oEventHandler.call(eEvent, pEventData);
}

void CcWidget::registerOnEvent(EEventType eEvent, CcEvent eEventHandle)
{
  m_pPrivate->m_oEventHandler.add(eEvent, eEventHandle);
}

void CcWidget::removeOnEvent(EEventType eEvent, CcObject* pObject)
{
  m_pPrivate->m_oEventHandler.removeObject(eEvent, pObject);
}

void CcWidget::onRectangleChanged()
{
  if (CWNDHANDLE(this) != nullptr)
  {
    const CWnd* pParentWindow = &CWnd::wndTop;
    if (!CWNDHANDLE(this)->SetWindowPos(pParentWindow,
      getRectangle().getX(),
      getRectangle().getY(),
      getRectangle().getWidth(),
      getRectangle().getHeight(),
      SWP_SHOWWINDOW))
    {
      CCDEBUG("Failed to set Window rectangle");
    }
  }
}

void CcWidget::onBackgroundChanged()
{
}

void CcWidget::onForegroundChanged()
{
}

void CcWidget::initWidget(CcWidget* rParent)
{
  CCNEW(m_pPrivate,CPrivate);
  m_pPrivate->m_Parent = rParent;
  if (rParent != nullptr)
  {
    rParent->registerChild(this);
    CCNEW(m_pPrivate->pWidget, CcCWnd, this);
    m_pPrivate->hSubSystemHandle = m_pPrivate->pWidget;
    m_pPrivate->pWidget->enablePainting();
  }
}
