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
#include "CcQt.h"

#include <QWidget>

class CcWidget::CPrivate
{
public:
  CPrivate(CcWidget* rParent) :
    m_Parent(rParent)
  {}
  CcWidget*       m_Parent;
  void*           pSubsystem;
  bool            m_bCustomPaint = false;
  CcStyleWidget*  m_pStyleheet = nullptr;
  CcGuiEventMap   m_oEventHandler;
  CcRectangle     oRectangle;
  CcList<CcWidget*> m_oChildList;

};

CcWidget::CcWidget(CcWidget* parent)
{
  initWidget(parent);
}

CcWidget::CcWidget(const CcRectangle& oWindowRect, CcWidget* rParent)
{
  initWidget(rParent);
  m_pPrivate->oRectangle = oWindowRect;
}

CcWidget::CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, CcWidget* parent)
{
  initWidget(parent);
  m_pPrivate->oRectangle = CcRectangle(iPosX, iPosY, uiWidth, uiHeight);
}

CcWidget::~CcWidget()
{
  if (m_pPrivate->m_Parent != nullptr)
    m_pPrivate->m_Parent->removeChild(this);
  for (CcWidget* pWidget : getChildList())
  {
    pWidget->setParent(nullptr);
  }
  CCDELETE(m_pPrivate->m_pStyleheet);
  CCDELETE(m_pPrivate);
}

void CcWidget::setCustomPainting(bool bEnable)
{
  m_pPrivate->m_bCustomPaint = bEnable;
}

void CcWidget::setPos(const CcPoint& oPoint)
{
  m_pPrivate->oRectangle = oPoint;
  onRectangleChanged();
}

void CcWidget::setSize(const CcSize& oSize)
{
  m_pPrivate->oRectangle = oSize;
  onRectangleChanged();
}

void CcWidget::setBackgroundColor(const CcColor& oColor)
{
  if(m_pPrivate->pSubsystem)
  {
    getStyle()->oBackgroundColor = oColor;
    QPalette oPalette = ToQWidget(m_pPrivate->pSubsystem)->palette();
    oPalette.setColor(ToQWidget(m_pPrivate->pSubsystem)->backgroundRole(), ToQColor(oColor));
    ToQWidget(m_pPrivate->pSubsystem)->setPalette(oPalette);
  }
}

void CcWidget::setForegroundColor(const CcColor& oColor)
{
  if(m_pPrivate->pSubsystem)
  {
    getStyle()->oForegroundColor = oColor;
    QPalette oPalette = ToQWidget(m_pPrivate->pSubsystem)->palette();
    QColor oqColor = ToQColor(oColor);
    oPalette.setColor(ToQWidget(m_pPrivate->pSubsystem)->foregroundRole(), oqColor);
    ToQWidget(m_pPrivate->pSubsystem)->setPalette(oPalette);
  }
}

void CcWidget::setWindowState(EWindowState eWindowState)
{
  getWindow()->setWindowState(eWindowState);
}

void* CcWidget::getSubSysHandle()
{
  return m_pPrivate->pSubsystem;
}

const CcRectangle& CcWidget::getRectangle() const
{
  return m_pPrivate->oRectangle;
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
  return m_pPrivate->oRectangle;
}

const CcColor& CcWidget::getBackgroundColor()
{
  return m_pPrivate->m_pStyleheet->oBackgroundColor;
}
const CcColor& CcWidget::getForegroundColor()
{
  return m_pPrivate->m_pStyleheet->oForegroundColor;
}

void CcWidget::setWindowRect(const CcRectangle& oRect)
{
  m_pPrivate->oRectangle = oRect;
  onRectangleChanged();
}

CcRectangle CcWidget::getInnerRect()
{
  CcRectangle oRectangle = getRectangle();
  oRectangle.addBorderSize(-getStyle()->uBorderSize);
  return oRectangle;
}

void CcWidget::drawBorder(const CcColor& oColor, uint32 uiSize )
{
  CcPainter Painter(this);
  Painter.setColor(oColor);
  CcRectangle oRectangle = getRectangle();
  oRectangle.setPoint(CcPoint(0, 0));
  Painter.drawRectangle(oRectangle, static_cast<int32>(uiSize), false);
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
  m_pPrivate->m_oEventHandler.call(eEvent, pEventData);
}

void CcWidget::registerOnEvent(EGuiEvent eEvent, IEvent* eEventHandle)
{
  m_pPrivate->m_oEventHandler.add(eEvent, eEventHandle);
}

void CcWidget::removeOnEvent(EGuiEvent eEvent, CcObject* pObject)
{
  m_pPrivate->m_oEventHandler.removeObject(eEvent, pObject);
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

void CcWidget::setSubSystemHandle(void* hSubSystem)
{
  m_pPrivate->pSubsystem = hSubSystem;
}

const CcColor& CcWidget::getBorderColor()
{
  return m_pPrivate->m_pStyleheet->oBorderColor;
}

void CcWidget::setBorderColor(const CcColor& oColor)
{
  if(m_pPrivate->pSubsystem)
  {
    QString sStyle = "border: ";
    sStyle += QString::number(getStyle()->uBorderSize) + "px solid ";
    sStyle += oColor.getCssString().getCharString();
    ToQWidget(m_pPrivate->pSubsystem)->setStyleSheet(sStyle);
  }
}

void CcWidget::setBorderSize(uint16 uiSize)
{
  m_pPrivate->m_pStyleheet->uBorderSize = uiSize;
}

uint32 CcWidget::getBorderSize()
{
  return m_pPrivate->m_pStyleheet->uBorderSize;
}

const CcSize& CcWidget::getSize()
{
  return m_pPrivate->oRectangle;
}

CcStyleWidget* CcWidget::getStyle()
{
  return m_pPrivate->m_pStyleheet;
}

const CcStyleWidget* CcWidget::getStyle() const
{
  return m_pPrivate->m_pStyleheet;
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

CcRectangle& CcWidget::getRectangle()
{
  return m_pPrivate->oRectangle;
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
  return m_pPrivate->m_Parent->getWindow();
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

void CcWidget::setStyle(CcStyleWidget* pStyleSheet)
{
  m_pPrivate->m_pStyleheet = pStyleSheet;
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
  return this;
}

void CcWidget::removeChild(CcWidget* oChildWidget)
{
  m_pPrivate->m_oChildList.removeItem(oChildWidget);
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
  CCNEW(m_pPrivate, CPrivate, rParent);
  if(m_pPrivate != nullptr)
  {
    m_pPrivate->m_Parent = rParent;
    if (m_pPrivate->m_Parent)
    {
      m_pPrivate->m_Parent->registerChild(this);
    }
    CCNEW(m_pPrivate->m_pStyleheet, CcStyleWidget);
  }
}
