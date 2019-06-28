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
#ifndef _CcWidget_H_
#define _CcWidget_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcObject.h"
#include "CcEventHandleMap.h"
#include "CcInputEvent.h"
#include "Types/CcColor.h"
#include "Types/CcRectangle.h"
#include "Style/CcStyle.h"
#include "CcSharedPointer.h"
#include "EGuiEvent.h"
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
  Hide,
  Tray,
};


class CcWindow;
class CcWidget;

#ifdef _MSC_VER
template class CcGuiSHARED CcHandle<CcWindow>;
template class CcGuiSHARED CcHandle<CcWidget>;
template class CcGuiSHARED CcHandle<void>;
#endif

typedef CcHandle<CcWindow> CcWindowHandle;
typedef CcHandle<CcWidget> CcWidgetHandle;
typedef CcHandle<void>     CcSubSysHandle;

class CcGuiSHARED CcWidget : public CcObject 
{
public:
  CcWidget(const CcWidgetHandle& rParent);
  CcWidget(const CcRectangle& oWindowRect, const CcWidgetHandle& rParent);
  CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, const CcWidgetHandle& rParent);
  virtual ~CcWidget();

  void setSize(const CcSize& oSize);
  
  void setBackgroundColor(const CcColor& oColor);
  void setForegroundColor(const CcColor& oColor);
  void setPos(const CcPoint& oPoint);
  void setWindowRect(const CcRectangle& oRect);
  void setParent(const CcWidgetHandle& rParent);
  void setStyle(CcStyleWidget* pStyleSheet)
    { m_pStyleheet = pStyleSheet; }
  void setWindowState(EWindowState eWindowState);
  /**
   * @brief Get a Subsystem defined Handle
   * @return Handle as Pointer, Type is defined by Subsystem.
   */
  void setSubSystemHandle(CcSubSysHandle hSubSystem);


  inline const CcColor& getBorderColor()
    { return m_pStyleheet->oBorderColor;}
  inline void setBorderColor(const CcColor& oColor)
    { m_pStyleheet->oBorderColor = oColor;}
  inline void setBorderSize(uint16 uiSize)
    { m_pStyleheet->uBorderSize = uiSize;}
  inline uint32 getBorderSize()
    { return m_pStyleheet->uBorderSize;}
  virtual CcWidgetHandle& getHandle()
    { return m_hThisHandle; }
  CcWidgetHandle& getParent();
  CcWidgetHandle& getHitTest(const CcPoint& oPointToFind);
  virtual const CcSize& getSize()
    { return m_oWindowRect;}
  CcStyleWidget* getStyle()
    {return m_pStyleheet;}
  const CcStyleWidget* getStyle() const
    {return m_pStyleheet;}
  /**
   * @brief Get a Subsystem defined Handle
   * @return Handle as Pointer, Type is defined by Subsystem.
   */
  CcSubSysHandle& getSubSysHandle();

  inline const CcRectangle& getWindowRect() const
    { return m_oWindowRect; }
  CcRectangle getInnerRect() const;
  inline int32 getWidth() const
    { return getWindowRect().getWidth(); }
  inline int32 getHeight() const
    { return getWindowRect().getHeight(); }
  inline const CcPoint& getPos()
    { return m_oWindowRect;}
  EWindowState getWindowState();
  inline const CcColor& getBackgroundColor()
    { return m_pStyleheet->oBackgroundColor; }
  inline const CcColor& getForegroundColor()
    { return m_pStyleheet->oForegroundColor; }
  void registerChild(const CcWidgetHandle& oChildWidget);
  void removeChild(const CcWidgetHandle& oChildWidget);
  const CcList<CcWidgetHandle>& getChildList();

  void event(EGuiEvent eEvent, void* pEventData);
  void registerOnEvent(EGuiEvent eEvent, IEvent* eEventHandle);
  void removeOnEvent(EGuiEvent eEvent, CcObject* pObject);

  virtual void draw(bool bDoFlush = true);
  virtual void drawPixel(const CcColor& oColor, uint64 uiNumber = 1);
  virtual void flush();
  virtual CcRectangle getInnerRect();
  virtual CcWindowHandle& getWindow();
  virtual bool setPixelArea(const CcRectangle& oRectangle);

protected:
  void drawBackground(const CcColor& oColor);
  void drawBorder(const CcColor& oColor, uint32 uiSize);
  void drawAllChilds();
  virtual void onEvent(EGuiEvent eEvent, void *pMouseEvent);
  virtual void onMouseEvent(EGuiEvent eEvent, CcMouseEvent* pMouseEvent);
  virtual void onKeyEvent(EGuiEvent eEvent, CcKeyEvent* pMouseEvent);
  virtual void onWindowEvent(EGuiEvent eEvent);
  virtual void setCustomPainting(bool bEnable);

  virtual void onRectangleChanged();
  virtual void onBackgroundChanged();
  virtual void onForegroundChanged();

private:
  void initWidget(const CcWidgetHandle& rParent);
  void initStyle();
  void initSubSystem();

private: // Types
  class CPrivate;
private: // Member
  CcWidgetHandle   m_hThisHandle;
  CPrivate*        m_pPrivate    = nullptr;
  CcStyleWidget*   m_pStyleheet  = nullptr;
  CcGuiEventMap    m_oEventHandler;
  CcRectangle m_oWindowRect;
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcWidget>;
#endif

typedef CcSharedPointer<CcWidget> CcWidgetPointer;
#endif /* _CcWidget_H_ */
