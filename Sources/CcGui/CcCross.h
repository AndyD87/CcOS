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
 * @brief     Class CcCross
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"

/**
 * @brief Create a icon with a cross as sign.
 *        The sign can be diagonal or horizontal
 */
class CcGuiSHARED CcCross
{
public:
  /**
   * @brief Default object without any drawing information.
   */
  CcCross();

  /**
   * @brief Initialize cross with it's sizes
   * @param uiWidth:      Dimension in X direction
   * @param uiHeight:     Dimension in Y direction
   * @param uiThickness:  Weight of line
   */
  CcCross(uint16 uiWidth, uint16 uiHeight, uint16 uiThickness);
  virtual ~CcCross();

  /**
   * @brief Set new values to the crosse
   * @param uiWidth:      Dimension in X direction
   * @param uiHeight:     Dimension in Y direction
   * @param uiThickness:  Weight of line
   */
  void setValues(uint16 uiWidth, uint16 uiHeight, uint16 uiThickness);

  /**
   * @brief Set diagonal or horizontal orientation.
   * @param bDiagonal: If true cross will be set to diagonal
   */
  inline void setDiagonal(bool bDiagonal) { m_bDiagonal = bDiagonal;}
  //! @return Unsigned width of cross.
  uint16 getWidth() const
  { return m_uiWidth; }
  //! @return Unsigned height of cross.
  uint16 getHeight() const
  { return m_uiHeight; }
  //! @return Unsigned thickness of cross.
  uint16 getThickness() const
  { return m_uiThickness; }
  //! @return True if diagonal is set.
  bool isDiagonal() const
  { return m_bDiagonal; }

private:
  uint16 m_uiWidth  = 0;
  uint16 m_uiHeight = 0;
  uint16 m_uiThickness  = 0;
  bool m_bDiagonal = false;
};
