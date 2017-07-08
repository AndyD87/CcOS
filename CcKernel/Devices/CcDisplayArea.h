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
 * @subpage   CcDisplayArea
 *
 * @page      CcDisplayArea
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcDisplayArea
 */
#ifndef CcDisplayArea_H_
#define CcDisplayArea_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcObject.h"
#include "CcEventHandler.h"
#include "CcString.h"
#include "Types/CcRectangle.h"
#include "Types/CcColor.h"

class CcDisplay;

#ifdef WIN32
template class CcKernelSHARED CcHandle<CcDisplay>;
#endif

enum class EDisplayCommands
{
  Restore,
  Normal,
  Minimized,
  Maximimized,
  Tray,
  Close,
  Size
};

class CcKernelSHARED CcDisplayArea : public CcObject
{
public:
  CcDisplayArea(const CcRectangle& oArea);
  ~CcDisplayArea();
  virtual void drawPixel(const CcColor& oPixel) = 0;
  virtual bool setPixelArea(const CcRectangle& oArea) = 0;
  virtual void draw() = 0;
  void setArea(const CcRectangle& oArea);
  void setPos(const CcPoint& oPosition);
  void setSize(const CcSize& oSize);
  void setDisplay(const CcHandle<CcDisplay>& pDisplay);

  virtual void getMaxArea(CcRectangle& oArea) = 0;
  virtual void hide() = 0;
  virtual void show() = 0;

  inline int16 getPosX() const
    { return m_oDrawingArea.getX(); }
  inline int16 getPosY() const
    { return m_oDrawingArea.getY(); }
  inline uint16 getWidth() const
    { return m_oDrawingArea.getWidth(); }
  inline uint16 getHeight() const
    { return m_oDrawingArea.getHeight(); }

  virtual void setWindowTitle(const CcString& sTitle)
    { CCUNUSED(sTitle); }

  inline CcEventHandler& getInputEventHandler()
    {return m_InputEventHandler;}
  inline CcEventHandler& getControlEventHandler()
    {return m_ControlEventHandler;}

  inline const CcRectangle& getDrawingArea() const
    { return m_oDrawingArea; }
protected:
  virtual void updateSize() = 0;
  virtual void updatePos() = 0;

private:
  CcRectangle   m_oDrawingArea;
  CcHandle<CcDisplay> m_Display;
  CcEventHandler m_InputEventHandler;
  CcEventHandler m_ControlEventHandler;
};

#endif /* CcDisplayArea_H_ */
