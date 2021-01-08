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

class CcWidget::CPrivate
{
public:
  CPrivate(CcWidget* rThis, CcWidget* rParent) :
    m_pThis(rThis),
    m_Parent(rParent)
  {
  }
  CcWidget*  m_pThis;
  CcWidget*  m_Parent;
  void*  m_hSubSys;
  bool            m_bCustomPaint = false;
  CcStyleWidget   m_oStyleheet;
  CcGuiEventMap   m_oEventHandler;
  CcRectangle     m_oWindowRect;
  CcList<CcWidget*> m_oChildList;

};

CcWidget::CcWidget(CcWidget* parent)
{
  initWidget(parent);
}

CcWidget::CcWidget(const CcRectangle& oWindowRect, CcWidget* rParent)
{
  initWidget(rParent);
  m_pPrivate->m_oWindowRect = oWindowRect;
}

CcWidget::CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, CcWidget* parent)
{
  initWidget(parent);
  m_pPrivate->m_oWindowRect = CcRectangle(iPosX, iPosY, uiWidth, uiHeight);
}

CcWidget::~CcWidget()
{
  if (m_pPrivate->m_Parent != nullptr)
    m_pPrivate->m_Parent->removeChild(this);
  for (CcWidget* pWidget : getChildList())
  {
    pWidget->setParent(nullptr);
  }
  CCDELETE(m_pPrivate);
}

void CcWidget::setCustomPainting(bool bEnable)
{
  m_pPrivate->m_bCustomPaint = bEnable;
}

void CcWidget::setPos(const CcPoint& oPoint)
{
  m_pPrivate->m_oWindowRect = oPoint;
  onRectangleChanged();
}

void CcWidget::setSize(const CcSize& oSize)
{
  m_pPrivate->m_oWindowRect = oSize;
  onRectangleChanged();
}

void CcWidget::setBackgroundColor(const CcColor& oColor)
{
  getStyle().oBackgroundColor = oColor;
}

void CcWidget::setForegroundColor(const CcColor& oColor)
{
  getStyle().oForegroundColor = oColor;
}

void CcWidget::setWindowState(EWindowState eWindowState)
{
  getWindow()->setWindowState(eWindowState);
}

void* CcWidget::getSubSysHandle()
{
  return m_pPrivate->m_hSubSys;
}

const CcRectangle& CcWidget::getRectangle() const
{
  return m_pPrivate->m_oWindowRect;
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
  return m_pPrivate->m_oStyleheet.oBackgroundColor;
}
const CcColor& CcWidget::getForegroundColor()
{
  return m_pPrivate->m_oStyleheet.oForegroundColor;
}

void CcWidget::setRectangle(const CcRectangle& oRect)
{
  m_pPrivate->m_oWindowRect = oRect;
  onRectangleChanged();
}

CcRectangle CcWidget::getInnerRect()
{
  CcRectangle oRectangle = getRectangle();
  oRectangle.addBorderSize(-getStyle().uBorderSize);
  return oRectangle;
}

void CcWidget::drawBorder(const CcColor& oColor, uint32 uiSize )
{
  CcPainter Painter(this);
  Painter.setColor(oColor);
  CcRectangle oRectangle = getRectangle();
  oRectangle.setPoint(CcPoint(0, 0));
  Painter.drawRectangle(oRectangle, uiSize, false);
}

void CcWidget::draw(bool bDoFlush)
{
  drawBackground(getStyle().oBackgroundColor);
  drawAllChilds();
  if(bDoFlush)
  {
    flush();
  }
}

void CcWidget::flush()
{

}

void CcWidget::event(CcInputEvent* pEventData)
{
  onEvent(pEventData);
  if (pEventData->getType() >= EEventType::WindowEvent && pEventData->getType() <= EEventType::WindowEventMax)
  {
    onWindowEvent(pEventData);
  }
  else if (pEventData->getType() >= EEventType::MouseEvent && pEventData->getType() <= EEventType::MouseEventMax)
  {
    onMouseEvent(static_cast<CcMouseEvent*>(pEventData));
  }
  else if (pEventData->getType() >= EEventType::KeyEvent && pEventData->getType() <= EEventType::KeyEventMax)
  {
    onKeyEvent(static_cast<CcKeyEvent*>(pEventData));
  }
  m_pPrivate->m_oEventHandler.call(pEventData->getType(), pEventData);
}

void CcWidget::registerOnEvent(EEventType eEvent, const CcEvent& eEventHandle)
{
  m_pPrivate->m_oEventHandler.add(eEvent, eEventHandle);
}

void CcWidget::removeOnEvent(EEventType eEvent, CcObject* pObject)
{
  m_pPrivate->m_oEventHandler.removeObject(eEvent, pObject);
}

void CcWidget::onEvent(CcInputEvent* pEventData)
{
  CCUNUSED(pEventData);
}

void CcWidget::onMouseEvent(CcMouseEvent* pEventData)
{
  CCUNUSED(pEventData);
}

void CcWidget::onKeyEvent(CcKeyEvent* pEventData)
{
  CCUNUSED(pEventData);
}

void CcWidget::onWindowEvent(CcInputEvent *pEventData)
{
  CCUNUSED(pEventData);
}

void CcWidget::setSubSystemHandle(void* hSubSystem)
{
  CCUNUSED(hSubSystem);
}

const CcColor& CcWidget::getBorderColor()
{
  return m_pPrivate->m_oStyleheet.oBorderColor;
}

void CcWidget::setBorderColor(const CcColor& oColor)
{
  getStyle().oBorderColor = oColor;
  CcInputEvent eType(EEventType::StyleBorderStyle);
  event(&eType);
}

void CcWidget::setBorderSize(uint16 uiSize)
{
  getStyle().uBorderSize = uiSize;
  CcInputEvent eType(EEventType::StyleBorderStyle);
  event(&eType);
}

void CcWidget::fillParent()
{
  CcInputEvent eType(EEventType::StyleFillParent);
  event(&eType);
}

void CcWidget::hide()
{
  CcInputEvent eType(EEventType::WidgetHide);
  event(&eType);
}

uint32 CcWidget::getBorderSize()
{
  return m_pPrivate->m_oStyleheet.uBorderSize;
}

const CcSize& CcWidget::getSize()
{
  return m_pPrivate->m_oWindowRect;
}

CcStyleWidget& CcWidget::getStyle()
{
  return m_pPrivate->m_oStyleheet;
}

const CcStyleWidget& CcWidget::getStyle() const
{
  return m_pPrivate->m_oStyleheet;
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
  for (CcWidget* pWidget : getChildList())
  {
    pWidget->draw();
  }
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

EWindowState CcWidget::getWindowState()
{
  return getWindow()->getState();
}

CcWidget* CcWidget::getParent()
{
  return m_pPrivate->m_Parent;
}

void CcWidget::setParent(CcWidget* oParent)
{
  m_pPrivate->m_Parent = oParent;
}

void CcWidget::setStyle(const CcStyleWidget& pStyleSheet)
{
  m_pPrivate->m_oStyleheet = pStyleSheet;
}

bool CcWidget::setPixelArea(const CcRectangle& oRectangle)
{
  if (m_pPrivate->m_Parent != nullptr)
  {
    CcRectangle oForwardRec(oRectangle);
    oForwardRec += getRectangle();
    return m_pPrivate->m_Parent->setPixelArea(oForwardRec);
  }
  else
  {
    return getWindow()->setPixelArea(oRectangle);
  }
}

void CcWidget::registerChild(CcWidget* oChildWidget)
{
  m_pPrivate->m_oChildList.append(oChildWidget);
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
  return m_pPrivate->m_pThis;
}

bool CcWidget::removeChild(CcWidget* oChildWidget)
{
  return m_pPrivate->m_oChildList.removeItem(oChildWidget);
}

const CcList<CcWidget*>& CcWidget::getChildList()
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

void CcWidget::initWidget(CcWidget* rParent)
{
  CCNEW(m_pPrivate, CPrivate, this, rParent);
  if(m_pPrivate != nullptr)
  {
    m_pPrivate->m_Parent = rParent;
    if (rParent)
    {
      m_pPrivate->m_Parent->registerChild(this);
    }
  }
}
