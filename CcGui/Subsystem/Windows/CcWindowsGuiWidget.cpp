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
#include "CcWindowsGuiUtil.h"
#include "CcBitmap.h"
#include "CcCWnd.h"

class CcWidget::CPrivate
{
public:
  CcCWnd*         pWidget;
  void*  hSubSystemHandle;
  CcWidget*  m_Parent = nullptr;
  CcList<CcWidget*> m_oChildList;
};

CcWidget::CcWidget(CcWidget* rParent) :
  m_hThisHandle(this)
{
  initWidget(rParent);
}

CcWidget::CcWidget(const CcRectangle& oWindowRect, CcWidget* rParent) :
  m_hThisHandle(this),
  m_oWindowRect(oWindowRect)
{
  initWidget(rParent);
}

CcWidget::CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, CcWidget* rParent) :
  m_hThisHandle(this),
  m_oWindowRect(iPosX, iPosY, uiWidth, uiHeight)
{
  initWidget(rParent);
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
  if (m_oWindowRect != oPoint)
  {
    m_oWindowRect = oPoint;
    onRectangleChanged();
  }
}

void CcWidget::setSize(const CcSize& oSize)
{
  if (m_oWindowRect != oSize)
  {
    m_oWindowRect = oSize;
    onRectangleChanged();
  }
}

void CcWidget::setBackgroundColor(const CcColor& oColor)
{
  m_pStyleheet->oBackgroundColor = oColor;
  onBackgroundChanged();
}

void CcWidget::setForegroundColor(const CcColor& oColor)
{
  m_pStyleheet->oForegroundColor = oColor;
  onForegroundChanged();
}

void CcWidget::setWindowRect(const CcRectangle& oRect)
{
  m_oWindowRect = oRect;
  onRectangleChanged();
}

CcWidget*& CcWidget::getHitTest(const CcPoint& oPointToFind)
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
  return m_hThisHandle;
}

CcRectangle CcWidget::getInnerRect() const
{
  CcRectangle oRect(getRectangle());
  oRect.addBorderSize(getStyle()->uBorderSize);
  return oRect;
}

void*& CcWidget::getSubSysHandle()
{
  return m_pPrivate->hSubSystemHandle;
}

void CcWidget::draw(bool bDoFlush)
{
  drawBackground(getStyle()->oBackgroundColor);
  drawBorder(getStyle()->oBorderColor, m_pStyleheet->uBorderSize);
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
    CWNDHANDLE()->RedrawWindow();
    CWNDHANDLE()->Invalidate();
    CWNDHANDLE()->UpdateWindow();
  }
}

CcRectangle CcWidget::getInnerRect()
{
  CcRectangle oRectangle(getRectangle().getSize());
  oRectangle.addWidth( - (2 * m_pStyleheet->uBorderSize));
  oRectangle.addHeight( - (2 * m_pStyleheet->uBorderSize));
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
  return getWindow().cast<CcWindow>()->getState();
}

CcWidget*& CcWidget::getParent()
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
      m_pPrivate->m_Parent->removeChild(m_hThisHandle);
    m_pPrivate->m_Parent = rParent;
    m_pPrivate->m_Parent->registerChild(m_hThisHandle);
    if (m_pPrivate->m_Parent->getSubSysHandle() != nullptr &&
        m_pPrivate->hSubSystemHandle != nullptr)
    {
      CWNDHANDLE()->SetParent(m_pPrivate->m_Parent->CWNDHANDLE());
    }
  }
}

void CcWidget::setWindowState(EWindowState eWindowState)
{
  getWindow().cast<CcWindow>()->setWindowState(eWindowState);
}

void CcWidget::setSubSystemHandle(void* hSubSystem)
{
  if (m_pPrivate->pWidget != nullptr &&
    m_pPrivate->hSubSystemHandle.ptr() == m_pPrivate->pWidget)
  {
    m_pPrivate->pWidget->DestroyWindow();
    CCDELETE(m_pPrivate->pWidget);
  }
  m_pPrivate->hSubSystemHandle = hSubSystem;
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

void CcWidget::onRectangleChanged()
{
  if (CWNDHANDLE() != nullptr)
  {
    const CWnd* pParentWindow = &CWnd::wndTop;
    if (!CWNDHANDLE()->SetWindowPos(pParentWindow,
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
  initStyle();
  m_pPrivate->m_Parent = rParent;
  if (rParent != nullptr)
  {
    initSubSystem();
    rParent->registerChild(this);
  }
}

void CcWidget::initStyle()
{
  m_pStyleheet = &CcStyleWidget::Default;
}

void CcWidget::initSubSystem()
{
  m_pPrivate->pWidget = new CcCWnd(this);
  m_pPrivate->hSubSystemHandle = m_pPrivate->pWidget;
  m_pPrivate->pWidget->enablePainting();
}
