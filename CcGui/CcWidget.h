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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWidget
 */
#ifndef CcWidget_H_
#define CcWidget_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcObject.h"
#include "CcEventHandler.h"
#include "CcInputEvent.h"
#include "Types/CcColor.h"
#include "Types/CcRectangle.h"
#include "Styles/CcStyles.h"
#include "CcSharedPointer.h"

#define CB_EVENT      0

class CcWindow;
class CcWidget;
class CcWidgetPrivate;

#if WIN32
template class CcGuiSHARED CcHandle<CcWindow>;
template class CcGuiSHARED CcHandle<CcWidget>;
#endif
typedef CcHandle<CcWindow> CcWindowHandle;
typedef CcHandle<CcWidget> CcWidgetHandle;

class CcGuiSHARED CcWidget : public CcObject 
{
public:
  CcWidget(CcWidgetHandle parent);
  CcWidget(int16 iPosX, int16 iPosY, uint16 uiWidth, uint16 uiHeight, CcWidgetHandle parent);
  virtual ~CcWidget();

  void setSize(const CcSize& oSize);
  void setPos(const CcPoint& oPoint);
  void setWindowRect(const CcRectangle& oRect);
  CcRectangle getInnerRect();
  inline uint16 getWidth( void ) const
    { return getWindowRect().getWidth(); }
  inline uint16 getHeight( void ) const
    { return getWindowRect().getHeight(); }
  inline int16 getPosX(void) const
    { return getWindowRect().getX(); }
  inline int16 getPosY(void) const
    { return getWindowRect().getY(); }
  inline const CcPoint& getPos()
    { return m_oWindowRect;}
  virtual const CcSize& getSize()
    { return m_oWindowRect;}
  inline void setBackgroundColor(const CcColor& oColor)
    { m_BackgroundColor = oColor; }
  inline const CcColor& getBackgroundColor(void)
    { return m_BackgroundColor; }
  inline void setForegroundColor(const CcColor& oColor)
    { m_ForegroundColor = oColor; }
  inline const CcColor& getForegroundColor(void)
    { return m_ForegroundColor; }
  virtual void draw(void);
  void drawBorder(void);
  void drawPixel(const CcColor& oColor);
  void drawBackground(void);
  void drawAllChilds(void);
  inline const CcColor& getBorderColor()
    { return m_BorderColor;}
  inline void setBorderColor(const CcColor& oColor)
    { m_BorderColor = oColor;}
  inline void setBorderSize(uint16 uiSize)
    { m_uiBorderSize = uiSize;}
  inline uint16 getBorderSize()
    { return m_uiBorderSize;}
  virtual CcWindowHandle getWindow(void);
  virtual CcWidgetHandle getWindowWidget(void);
  CcWidgetHandle getParent(void);
  void setParent(CcWidgetHandle oParent);
  inline const CcRectangle& getWindowRect(void) const
    { return m_oWindowRect; }
  bool setPixelArea(const CcRectangle& oRectangle);
  void registerChild(CcWidgetHandle oChildWidget);
  void removeChild(CcWidgetHandle oChildWidget);
  const CcList<CcWidgetHandle>& getChildList();

  CcWidgetHandle getHitTest(const CcPoint& oPointToFind);

public:
  virtual void onRectangleChanged(void);
  virtual void onBackgroundChanged(void);

private:
  void initWidgetPrivate();

private:
  CcWidgetPrivate* m_pPrivate = nullptr;
  CcRectangle m_oWindowRect;
  CcColor m_BackgroundColor = CcStyle::WidgetBackgroundColor;
  CcColor m_ForegroundColor = CcStyle::WidgetForegroundColor;
  CcColor m_BorderColor     = CcStyle::WidgetBorderColor;
  uint16 m_uiBorderSize     = CcStyle::WidgetBorderSize;
};

#ifdef WIN32
template class CcGuiSHARED CcSharedPointer<CcWidget>;
#endif

typedef CcSharedPointer<CcWidget> CcWidgetPointer;
#endif /* CcWidget_H_ */
