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
 * @page      Linux
 * @subpage   CcX11SubSystem
 *
 * @page      CcX11SubSystem
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcX11SubSystem
 */
#ifndef H_CcX11SubSystem_H_
#define H_CcX11SubSystem_H_

#include "CcColor.h"
#include <X11/Xlib.h>
#include "IGuiSubsystem.h"

typedef struct{
  uint8 B;
  uint8 G;
  uint8 R;
} bitmapRGB;

typedef  struct{
  uint16 width;
  uint16 height;
  uint32 pixCount;
  bitmapRGB *bitmap;
} bitmapAll;

extern bitmapAll g_Bitmap;
class CcX11SubSystemPrivate;

class CcX11SubSystem : public IGuiSubsystem
{
public:
  CcX11SubSystem(const CcWindowHandle& hWindowHandle);
  ~CcX11SubSystem();

  virtual CcStatus open() override;
  virtual CcStatus close() override;
  virtual void loop() override;
  virtual void drawPixel(const CcColor& oPixel, uint64 uiNumber) override;
  virtual bool setPixelArea(const CcRectangle& oArea) override;
  virtual void draw() override;
  virtual void getMaxArea(CcRectangle& oArea) override;
  virtual void hide() override;
  virtual void show() override;

  virtual bool hasFrame() override;
  virtual CcRectangle getInnerRect() override;
  virtual void setWindowTitle(const CcString& sTitle) override;
  virtual bool setWindowState(EWindowState eState) override;
  virtual CcSubSysHandle getHandle() override;

protected:
  virtual void updateSize() override;
  virtual void updatePos() override;

private: //member
  CcX11SubSystemPrivate* m_pPrivate;
};

#endif // H_CcX11SubSystem_H_
