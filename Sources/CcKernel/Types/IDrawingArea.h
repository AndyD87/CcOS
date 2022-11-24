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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IDrawingArea
 */
#pragma once

#include "CcBase.h"
#include "CcSize.h"
#include "CcPoint.h"

/**
 * @brief Basic Class for all Devices in System.
 */
class CcKernelSHARED IDrawingArea :
  public CcSize
{
public:
  IDrawingArea(const CcSize& oSize) :
    CcSize(oSize)
  {}

  /**
   * @brief Destructor
   *        Every derived device has to stop it self before destructor is called.
   */
  virtual ~IDrawingArea() = default;

  virtual IDrawingArea* createDrawingArea(const CcRectangle& oArea);
  virtual void removeDrawingArea(IDrawingArea* pArea);

  virtual void setPixel(int32 iX, int32 iY)
  { m_oCursor.setPoint(iX, iY); }

  void setPixel(const CcPoint& oPoint)
  { setPixel(oPoint.getX(), oPoint.getY()); }
protected:
  CcPoint m_oCursor;       //!< Location of Cursor
};
