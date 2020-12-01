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
 * @page      CcGui
 * @subpage   CcWidget
 *
 * @page      CcWidget
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWidget
 */
#ifndef H_CcWidget_H_
#define H_CcWidget_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcObject.h"
#include "CcEventHandleMap.h"
#include "CcInputEvent.h"
#include "Types/CcColor.h"
#include "Types/CcRectangle.h"
#include "Style/CcStyle.h"
#include "CcSharedPointer.h"
#include "EEventType.h"
#include "Style/CcStyleWidget.h"

#define CB_EVENT      0

 /**
 * @brief Current visual state of Window as enum
 */
enum class EWindowState
{
  Close,
  Normal,
  Minimized,
  Maximimized,
  FullScreen,
  Hide,
  Tray,
};

class CcWindow;

class CcGuiSHARED CcWidget : public CcObject
{
public:
  CcWidget(CcWidget* rParent);
  CcWidget(const CcRectangle& oWindowRect, CcWidget* rParent);
  CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, CcWidget* rParent);
  CcWidget(const CcWidget& oToCopy) = delete;
  virtual ~CcWidget();

  void setSize(const CcSize& oSize);
  void setBackgroundImage(const CcString& sPath);
  void setBackgroundColor(const CcColor& oColor);
  void setForegroundColor(const CcColor& oColor);
  void setFocus();
  void setPos(const CcPoint& oPoint);
  void setRectangle(const CcRectangle& oRect);
  void setParent(CcWidget* rParent);
  void setStyle(const CcStyleWidget& pStyleSheet);
  void setWindowState(EWindowState eWindowState);
  void setSubSystemHandle(void* hSubSystem);

  const CcColor& getBorderColor();
  void setBorderColor(const CcColor& oColor);
  void setBorderSize(uint16 uiSize);

  uint32 getBorderSize();
  CcWidget* getParent();
  CcWidget* getHitTest(const CcPoint& oPointToFind);
  virtual const CcSize& getSize();
  CcStyleWidget& getStyle();
  const CcStyleWidget& getStyle() const;
  /**
   * @brief Get a Subsystem defined Handle
   * @return Handle as Pointer, Type is defined by Subsystem.
   */
  virtual void* getSubSysHandle();

  const CcRectangle& getRectangle() const;
  CcRectangle getInnerRect() const;
  int32 getWidth() const;
  int32 getHeight() const;
  const CcPoint& getPos();
  EWindowState getWindowState();
  const CcColor& getBackgroundColor();
  const CcColor& getForegroundColor();
  void registerChild(CcWidget* oChildWidget);
  bool removeChild(CcWidget* oChildWidget);
  const CcList<CcWidget*>& getChildList();

  void registerOnEvent(EEventType eEvent, const CcEvent& eEventHandle);
  void removeOnEvent(EEventType eEvent, CcObject* pObject);

  void fillParent();
  void hide();
  void show();
  virtual void event(CcInputEvent* pEventData);
  virtual void draw(bool bDoFlush = true);
  virtual void drawPixel(const CcColor& oColor, uint64 uiNumber = 1);
  virtual void flush();
  virtual CcRectangle getInnerRect();
  virtual CcWindow* getWindow();
  virtual bool setPixelArea(const CcRectangle& oRectangle);

protected:
  void drawBackground(const CcColor& oColor);
  void drawBorder(const CcColor& oColor, uint32 uiSize);
  void drawAllChilds();
  //CcRectangle& getRectangle();
  virtual void onEvent(CcInputEvent* pEventData);
  virtual void onMouseEvent(CcMouseEvent* pMouseEvent);
  virtual void onKeyEvent(CcKeyEvent* pEventData);
  virtual void onWindowEvent(CcInputEvent* pEventData);
  virtual void setCustomPainting(bool bEnable);
  virtual void onRectangleChanged();
  virtual void onBackgroundChanged();
  virtual void onForegroundChanged();

private:
  void initWidget(CcWidget* rParent);

private: // Types
  class CPrivate;
private: // Member
  CPrivate*             m_pPrivate = nullptr;
  static CcWidget* s_hNullHandle;
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcWidget>;
#endif

typedef CcSharedPointer<CcWidget> CcWidgetPointer;
#endif // H_CcWidget_H_
