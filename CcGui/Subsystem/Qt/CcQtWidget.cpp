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
#include "Style/CcStyleWidget.h"
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
  CcStyleWidget   oStyle;
  CcGuiEventMap   oEventHandler;
  CcRectangle     oRectangle;
  CcList<CcWidget*> oChildList;

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
  CCDELETE(m_pPrivate);
}

void CcWidget::setCustomPainting(bool bEnable)
{
  m_pPrivate->m_bCustomPaint = bEnable;
}

void CcWidget::setPos(const CcPoint& oPoint)
{
  if(m_pPrivate->oRectangle != oPoint)
  {
    m_pPrivate->oRectangle = oPoint;
    QPoint oQSize = ToQPoint(oPoint);
    if(getSubSysHandle() &&
       oQSize != ToQWidget(getSubSysHandle())->pos())
    {
      ToQWidget(getSubSysHandle())->move(oQSize);
    }
    CcStyle::EType eType = CcStyle::EType::None;
    event(EEventType::WindowPosition, &eType);
    onRectangleChanged();
  }
}

void CcWidget::setSize(const CcSize& oSize)
{
  if(m_pPrivate->oRectangle != oSize)
  {
    m_pPrivate->oRectangle = oSize;
    QSize oQSize = ToQSize(oSize);
    if(getSubSysHandle() &&
       oQSize != ToQWidget(getSubSysHandle())->size())
    {
      ToQWidget(getSubSysHandle())->setFixedSize(oQSize);
    }
    CcStyle::EType eType = CcStyle::EType::None;
    event(EEventType::WindowSize, &eType);
    onRectangleChanged();
    for(CcWidget* pWidget : m_pPrivate->oChildList)
    {
      pWidget->setSize(getSize());
    }
  }
}

void CcWidget::setBackgroundColor(const CcColor& oColor)
{
  getStyle().oBackgroundColor = oColor;
  CcStyle::EType eType = CcStyle::EType::BackgroundColor;
  event(EEventType::WidgetStyleChanged, &eType);
}

void CcWidget::setForegroundColor(const CcColor& oColor)
{
  getStyle().oForegroundColor = oColor;
  CcStyle::EType eType = CcStyle::EType::ForegroundColor;
  event(EEventType::WidgetStyleChanged, &eType);
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
  return m_pPrivate->oStyle.oBackgroundColor;
}
const CcColor& CcWidget::getForegroundColor()
{
  return m_pPrivate->oStyle.oForegroundColor;
}

void CcWidget::setRectangle(const CcRectangle& oRect)
{
  m_pPrivate->oRectangle = oRect;
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
  Painter.drawRectangle(oRectangle, static_cast<int32>(uiSize), false);
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

void CcWidget::event(EEventType eEvent, void* pEventData)
{
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
  else if (eEvent == EEventType::WidgetStyleChanged)
  {
    CcStyle::EType eStyleEvent = *static_cast<CcStyle::EType*>(pEventData);
    if (m_pPrivate->pSubsystem)
    {
      switch (eStyleEvent)
      {
        case CcStyle::EType::BorderStyle:
        {
          QString sStyle = "border: ";
          sStyle += QString::number(getStyle().uBorderSize) + "px solid ";
          sStyle += getStyle().oBorderColor.getCssString().getCharString();
          ToQWidget(m_pPrivate->pSubsystem)->setStyleSheet(sStyle);
          break;
        }
        case CcStyle::EType::BackgroundColor:
        {
          QPalette oPalette = ToQWidget(m_pPrivate->pSubsystem)->palette();
          QColor oqColor = ToQColor(getStyle().oBackgroundColor);
#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
          oPalette.setColor(QPalette::Window, oqColor);
#endif
          oPalette.setColor(ToQWidget(m_pPrivate->pSubsystem)->backgroundRole(), oqColor);
          ToQWidget(m_pPrivate->pSubsystem)->setPalette(oPalette);
          break;
        }
        case CcStyle::EType::ForegroundColor:
        {
          QPalette oPalette = ToQWidget(m_pPrivate->pSubsystem)->palette();
          QColor oqColor = ToQColor(getStyle().oForegroundColor);
#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
          oPalette.setColor(QPalette::WindowText, oqColor);
#endif
          oPalette.setColor(ToQWidget(m_pPrivate->pSubsystem)->foregroundRole(), oqColor);
          ToQWidget(m_pPrivate->pSubsystem)->setPalette(oPalette);
          break;
        }
        case CcStyle::EType::FillParent:
        {
          if( m_pPrivate->m_Parent &&
              ToQWidget(m_pPrivate->m_Parent->m_pPrivate->pSubsystem))
          {
            QSize oSize = ToQWidget(m_pPrivate->m_Parent->m_pPrivate->pSubsystem)->size();
            ToQWidget(m_pPrivate->pSubsystem)->setFixedSize(oSize);
          }
          break;
        }
        default:
          break;
      }
    }
  }
  onEvent(eEvent, pEventData);
  m_pPrivate->oEventHandler.call(eEvent, pEventData);
}

void CcWidget::registerOnEvent(EEventType eEvent, CcEvent eEventHandle)
{
  m_pPrivate->oEventHandler.add(eEvent, eEventHandle);
}

void CcWidget::removeOnEvent(EEventType eEvent, CcObject* pObject)
{
  m_pPrivate->oEventHandler.removeObject(eEvent, pObject);
}

void CcWidget::onEvent(EEventType eEvent, void *pMouseEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcWidget::onMouseEvent(EEventType eEvent, CcMouseEvent* pMouseEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcWidget::onKeyEvent(EEventType eEvent, CcKeyEvent* pKeyEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pKeyEvent);
}

void CcWidget::onWindowEvent(EEventType eWindowEvent)
{
  CCUNUSED(eWindowEvent);
}

void CcWidget::setSubSystemHandle(void* hSubSystem)
{
  m_pPrivate->pSubsystem = hSubSystem;
  ToQWidget(hSubSystem)->setAutoFillBackground(true);
}

const CcColor& CcWidget::getBorderColor()
{
  return m_pPrivate->oStyle.oBorderColor;
}

void CcWidget::setBorderColor(const CcColor& oColor)
{
  getStyle().oBorderColor = oColor;
  CcStyle::EType eType = CcStyle::EType::BorderStyle;
  event(EEventType::WidgetStyleChanged, &eType);
}

void CcWidget::setBorderSize(uint16 uiSize)
{
  m_pPrivate->oStyle.uBorderSize = uiSize;
  CcStyle::EType eType = CcStyle::EType::BorderStyle;
  event(EEventType::WidgetStyleChanged, &eType);
}

void CcWidget::fillParent()
{
  CcStyle::EType eType = CcStyle::EType::FillParent;
  event(EEventType::WidgetStyleChanged, &eType);
}

uint32 CcWidget::getBorderSize()
{
  return m_pPrivate->oStyle.uBorderSize;
}

const CcSize& CcWidget::getSize()
{
  return m_pPrivate->oRectangle;
}

CcStyleWidget& CcWidget::getStyle()
{
  return m_pPrivate->oStyle;
}

const CcStyleWidget& CcWidget::getStyle() const
{
  return m_pPrivate->oStyle;
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

void CcWidget::setStyle(const CcStyleWidget& oStyleSheet)
{
  m_pPrivate->oStyle = oStyleSheet;
  CcStyle::EType eType = CcStyle::EType::BorderStyle;
  event(EEventType::WidgetStyleChanged, &eType);
  eType = CcStyle::EType::ForegroundColor;
  event(EEventType::WidgetStyleChanged, &eType);
  eType = CcStyle::EType::BackgroundColor;
  event(EEventType::WidgetStyleChanged, &eType);
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
  m_pPrivate->oChildList.append(oChildWidget);
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
  m_pPrivate->oChildList.removeItem(oChildWidget);
}

const CcList<CcWidget*>& CcWidget::getChildList()
{
  return m_pPrivate->oChildList;
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
  }
}
