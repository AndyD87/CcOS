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
 * @subpage   CcGuiSubsystem
 *
 * @page      CcGuiSubsystem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGuiSubsystem
 */
#ifndef H_CcGuiSubsystem_H_
#define H_CcGuiSubsystem_H_

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

class IDisplay;

#ifdef _MSC_VER
template class CcGuiSHARED CcHandle<IDisplay>;
#endif

class CcGuiSHARED CcGuiSubsystem : public CcObject
{
public:
  CcGuiSubsystem(const CcWindowHandle& hWindowHandle) : m_hWindow(hWindowHandle)
    {}
  ~CcGuiSubsystem() = default;
  CcStatus open();
  CcStatus close();
  void loop();
  void drawPixel(const CcColor& oPixel, uint64 uiNumber);
  void draw();
  void getMaxArea(CcRectangle& oArea);
  void hide();
  void show();
  bool hasFrame();
  CcRectangle getInnerRect();
  void updateSize();
  void updatePos();

  bool setPixelArea(const CcRectangle& oArea);
  void setWindowTitle(const CcString& sTitle);
  bool setWindowState(EWindowState eState);

  /**
   * A inheriting class can create a main widget, with it's default handle type.
   * @return Handly casted as void
   */
  CcSubSysHandle getHandle();

  void setDisplay(const CcHandle<IDisplay>& pDisplay);

  CcWindowHandle& getWindowHandle()
    { return m_hWindow; }

  inline CcEventHandler& getInputEventHandler()
    {return m_InputEventHandler;}
  inline CcEventHandler& getControlEventHandler()
    {return m_ControlEventHandler;}

#ifdef WINDOWS
  intptr executeMessage(void* hWnd, uint32 message, intptr wParam, intptr lParam);
#endif

private:
  class CPrivate;
private:
  CPrivate*      m_pPrivate = nullptr;
  CcHandle<IDisplay> m_Display;
  CcEventHandler m_InputEventHandler;
  CcEventHandler m_ControlEventHandler;
  CcWindowHandle m_hWindow;
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcGuiSubsystem>;
#endif

#endif // _CcGuiSubsystem_H_
