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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcPainter
 *
 *  Low-Level Painting on a Window
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWindow.h"
#include "Types/CcPoint.h"
#include "CcCross.h"
#include "Types/CcColor.h"

/**
 * @brief Execute drawing algorithms for CcWindows
 */
class CcGuiSHARED CcPainter {
public:
  /**
   * @brief Constructor
   * @param Window: Target Window for Painting
   **/
  CcPainter(CcWidget* Window);

  /**
   * @brief Destructor
   */
  virtual ~CcPainter();

  /**
   * @brief Set Color of Figure, that has to get drawn
   * @param R: Red-Value
   * @param G: Green-Value
   * @param B: Blue-Value
   */
  void setColor(uint8 R, uint8 G, uint8 B);

  /**
   * @brief Set Color of Figure, that has to get drawn
   * @param oColor: Color values
   */
  void setColor(const CcColor& oColor);

  /**
   * @brief Draw a Line to Window
   * @param startX: X Coordinate of Startpoint
   * @param startY: Y Coordinate of Startpoint
   * @param stopX: X Coordinate of Stoppoint
   * @param stopY: Y Coordinate of Stoppoint
   */
  void drawLine(const CcPoint& oStart, const CcPoint& oStop);

  /**
   * @brief draw a symetric + Symbol to Window
   * @param position: Position of upper-left corner of cr
   * @param cross
   */
  void drawCross(const CcPoint& position, const CcCross& oCross);

  /**
   * @brief draw a symetric + Symbol to Window
   * @param position: Position of upper-left corner of cr
   * @param cross
   */
  void drawRectangle(const CcRectangle& oRectangle, int32 uiBorderSize, bool bFilled);

private:
  CcWidget* m_Widget;
  CcColor   m_Color;
};
