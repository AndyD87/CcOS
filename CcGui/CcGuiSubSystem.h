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
 * @page      Devices
 * @subpage   CcGuiSubSystem
 *
 * @page      CcGuiSubSystem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGuiSubSystem
 */
#ifndef _CcGuiSubSystem_H_
#define _CcGuiSubSystem_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcObject.h"
#include "CcEventHandler.h"
#include "CcString.h"
#include "Types/CcRectangle.h"
#include "Types/CcColor.h"
#include "CcHandle.h"
#include "CcSharedPointer.h"
#include "CcWidget.h"

class CcDisplay;

#ifdef WIN32
template class CcGuiSHARED CcHandle<CcDisplay>;
#endif

class CcGuiSHARED CcGuiSubSystem : public CcObject
{
public:
  CcGuiSubSystem(const CcWindowHandle& hWindowHandle);
  virtual ~CcGuiSubSystem() = default;
  virtual CcStatus open() = 0;
  virtual CcStatus close() = 0;
  virtual void loop() = 0;
  virtual void drawPixel(const CcColor& oPixel, uint64 uiNumber) = 0;
  virtual void draw() = 0;
  virtual void getMaxArea(CcRectangle& oArea) = 0;
  virtual void hide() = 0;
  virtual void show() = 0;
  virtual bool hasFrame() = 0;
  virtual CcRectangle getInnerRect() = 0;
  virtual void updateSize() = 0;
  virtual void updatePos() = 0;

  virtual bool setPixelArea(const CcRectangle& oArea) = 0;
  virtual void setWindowTitle(const CcString& sTitle) = 0;
  virtual bool setWindowState(EWindowState eState) = 0;

  /**
   * A inheriting class can create a main widget, with it's default handle type.
   * @return Handly casted as void
   */
  virtual CcSubSysHandle getHandle() = 0;

  void setDisplay(const CcHandle<CcDisplay>& pDisplay);

  CcWindowHandle& getWindowHandle()
    { return m_hWindow; }

  inline CcEventHandler& getInputEventHandler()
    {return m_InputEventHandler;}
  inline CcEventHandler& getControlEventHandler()
    {return m_ControlEventHandler;}
  
  static CcGuiSubSystem* create(const CcWindowHandle& hWindow);

private:
  CcHandle<CcDisplay> m_Display;
  CcEventHandler m_InputEventHandler;
  CcEventHandler m_ControlEventHandler;
  CcWindowHandle m_hWindow;
};

#ifdef WIN32
template class CcGuiSHARED CcSharedPointer<CcGuiSubSystem>;
#endif

#endif /* _CcGuiSubSystem_H_ */
