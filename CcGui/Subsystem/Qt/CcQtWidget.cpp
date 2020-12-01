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
#include "Qt/CStyleSheet.h"

#include <QPainter>
#include <QWidget>

class CcWidget::CPrivate
{
public:
  CPrivate(CcWidget* rParent) :
    m_Parent(rParent)
  {
    QWidget* pParent = nullptr;
    if (rParent != nullptr)
    {
      pParent = ToQWidget(rParent->getSubSysHandle());
    }
    CCNEW(pSubsystem, QWidget, pParent);
    bSubsystemWidget = true;
  }

  CcWidget*       m_Parent;
  void*           pSubsystem;
  bool            bSubsystemWidget = false;
  bool            m_bCustomPaint = false;
  CcStyleWidget   oStyle;
  CcGuiEventMap   oEventHandler;
  CcList<CcWidget*> oChildList;
  NQt::CStyleSheet oStyleSheet;
};

CcWidget::CcWidget(CcWidget* parent)
{
  initWidget(parent);
}

CcWidget::CcWidget(const CcRectangle& oWindowRect, CcWidget* rParent)
{
  initWidget(rParent);
  getStyle().oRectangle = oWindowRect;
}

CcWidget::CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, CcWidget* parent)
{
  initWidget(parent);
  getStyle().oRectangle = CcRectangle(iPosX, iPosY, uiWidth, uiHeight);
}

CcWidget::~CcWidget()
{
  if (m_pPrivate->m_Parent != nullptr)
  {
    m_pPrivate->m_Parent->removeChild(this);
    if (m_pPrivate->bSubsystemWidget)
    {
      m_pPrivate->bSubsystemWidget = false;
      QWidget* pSubSys = ToQWidget(m_pPrivate->pSubsystem);
      CCDELETE(pSubSys);
    }
  }
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
  if(getStyle().oRectangle != oPoint)
  {
    getStyle().oRectangle = oPoint;
    QPoint oQSize = ToQPoint(oPoint);
    if(getSubSysHandle() &&
       oQSize != ToQWidget(getSubSysHandle())->pos())
    {
      ToQWidget(getSubSysHandle())->move(oQSize);
    }
    CcInputEvent eType(EEventType::WindowPosition);
    event(&eType);
    onRectangleChanged();
  }
}

void CcWidget::setSize(const CcSize& oSize)
{
  if(getStyle().oRectangle != oSize)
  {
    getStyle().oRectangle = oSize;
    QSize oQSize = ToQSize(oSize);
    if(getSubSysHandle() &&
       oQSize != ToQWidget(getSubSysHandle())->size())
    {
      ToQWidget(getSubSysHandle())->setFixedSize(oQSize);
    }
    CcInputEvent eType(EEventType::WindowSize);
    event(&eType);
    onRectangleChanged();
    for(CcWidget* pWidget : m_pPrivate->oChildList)
    {
      pWidget->setSize(getSize());
    }
  }
}

void CcWidget::setBackgroundImage(const CcString& sPath)
{
  getStyle().sBackgroundImage = sPath;
  CcInputEvent oEvent(EEventType::StyleBackgroundImage);
  event(&oEvent);
}

void CcWidget::setBackgroundColor(const CcColor& oColor)
{
  getStyle().oBackgroundColor = oColor;
  CcInputEvent oEvent(EEventType::StyleBackgroundColor);
  event(&oEvent);
}

void CcWidget::setForegroundColor(const CcColor& oColor)
{
  getStyle().oForegroundColor = oColor;
  CcInputEvent oEvent(EEventType::StyleForegroundColor);
  event(&oEvent);
}

void CcWidget::setFocus()
{
  CcInputEvent oEvent(EEventType::WidgetSetFocus);
  event(&oEvent);
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
  return getStyle().oRectangle;
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
  return getStyle().oRectangle;
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
  getStyle().oRectangle = oRect;
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

void CcWidget::event(CcInputEvent* pEventData)
{
  if (pEventData->getType() >= EEventType::WindowEvent && pEventData->getType() <= EEventType::WindowEventMax)
  {
    onWindowEvent(pEventData);
  }
  if (pEventData->getType() >= EEventType::WidgetEvent && pEventData->getType() <= EEventType::WidgetEventMax)
  {
    if (m_pPrivate->pSubsystem)
    {
      switch (pEventData->getType())
      {
        case EEventType::WidgetSetFocus:
        {
          ToQWidget(m_pPrivate->pSubsystem)->setFocus();
          break;
        }
        case EEventType::WidgetHide:
        {
          ToQWidget(m_pPrivate->pSubsystem)->hide();
          break;
        }
        case EEventType::WidgetShow:
        {
          ToQWidget(m_pPrivate->pSubsystem)->show();
          break;
        }
        default:
          break;
      }
    }
  }
  else if (pEventData->getType() >= EEventType::MouseEvent && pEventData->getType() <= EEventType::MouseEventMax)
  {
    onMouseEvent(static_cast<CcMouseEvent*>(pEventData));
  }
  else if (pEventData->getType() >= EEventType::KeyEvent && pEventData->getType() <= EEventType::KeyEventMax)
  {
    onKeyEvent(static_cast<CcKeyEvent*>(pEventData));
  }
  else
  {
    if (m_pPrivate->pSubsystem)
    {
      switch (pEventData->getType())
      {
        case EEventType::StyleBorderStyle:
        {
          m_pPrivate->oStyleSheet.setBorderSize(getStyle().uBorderSize);
          m_pPrivate->oStyleSheet.setBorderColor(getStyle().oBorderColor);
          CcString sStyleSheet = m_pPrivate->oStyleSheet.getStyleSheet();
          ToQWidget(m_pPrivate->pSubsystem)->setStyleSheet(sStyleSheet.getCharString());
          break;
        }
        case EEventType::StyleForegroundColor:
        {
          m_pPrivate->oStyleSheet.setForegroundColor(getStyle().oForegroundColor);
          CcString sStyleSheet = m_pPrivate->oStyleSheet.getStyleSheet();
          ToQWidget(m_pPrivate->pSubsystem)->setStyleSheet(sStyleSheet.getCharString());
          break;
        }
        case EEventType::StyleBackgroundColor:
        {
          m_pPrivate->oStyleSheet.setBackgroundColor(getStyle().oBackgroundColor);
          CcString sStyleSheet = m_pPrivate->oStyleSheet.getStyleSheet();
          ToQWidget(m_pPrivate->pSubsystem)->setStyleSheet(sStyleSheet.getCharString());
          break;
        }
        case EEventType::StyleBackgroundImage:
        {
          NQt::CStyleSheet oSheet;
          m_pPrivate->oStyleSheet.setBackgroundImage(getStyle().sBackgroundImage);
          CcString sStyleSheet = m_pPrivate->oStyleSheet.getStyleSheet();
          ToQWidget(m_pPrivate->pSubsystem)->setStyleSheet(sStyleSheet.getCharString());
          break;
        }
        case EEventType::StyleFillParent:
        {
          if( m_pPrivate->m_Parent &&
              ToQWidget(m_pPrivate->m_Parent->m_pPrivate->pSubsystem))
          {
            QSize oSize = ToQWidget(m_pPrivate->m_Parent->m_pPrivate->pSubsystem)->size();
            ToQWidget(m_pPrivate->pSubsystem)->setFixedSize(oSize);
            ToQWidget(m_pPrivate->pSubsystem)->update();
          }
          break;
        }
        default:
          break;
      }
    }
  }
  onEvent(pEventData);
  m_pPrivate->oEventHandler.call(pEventData->getType(), pEventData);
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
  if(m_pPrivate->pSubsystem)
    ToQWidget(m_pPrivate->pSubsystem)->repaint();
}

void CcWidget::registerOnEvent(EEventType eEvent, const CcEvent& eEventHandle)
{
  m_pPrivate->oEventHandler.append(eEvent, eEventHandle);
}

void CcWidget::removeOnEvent(EEventType eEvent, CcObject* pObject)
{
  m_pPrivate->oEventHandler.removeObject(eEvent, pObject);
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
  // Delete default widget
  m_pPrivate->bSubsystemWidget = false; 
  QWidget* pSubSys = ToQWidget(m_pPrivate->pSubsystem);
  CCDELETE(pSubSys);

  // Set new widget
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

void CcWidget::show()
{
  CcInputEvent eType(EEventType::WidgetShow);
  event(&eType);
}

uint32 CcWidget::getBorderSize()
{
  return m_pPrivate->oStyle.uBorderSize;
}

const CcSize& CcWidget::getSize()
{
  return getStyle().oRectangle;
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
  CcInputEvent oEvent(EEventType::StyleBorderStyle);
  event(&oEvent);
  oEvent.setType(EEventType::StyleForegroundColor);
  event(&oEvent);
  oEvent.setType(EEventType::StyleBackgroundColor);
  event(&oEvent);
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

bool CcWidget::removeChild(CcWidget* oChildWidget)
{
  return m_pPrivate->oChildList.removeItem(oChildWidget);
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
