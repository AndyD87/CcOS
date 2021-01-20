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
 * @brief     Class IPixMap
 */
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "IDevice.h"
#include "CcSize.h"

class CcColor;
class CcRectangle;

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED IPixMap : public IDevice
{
public:
  /**
   * @brief Type of imaging device grayscale or colored, and what ever else
   *        will following
   */
  enum class EType
  {
    GrayScale = 0,
    Color,
  };

  /**
   * @brief Set area of cursor for drawing
   * @param oArea: Area as rectangle
   * @return True if Area is sucessfully applied.
   */
  virtual bool setPixelArea(const CcRectangle& oArea) = 0;

  /**
   * @brief Draw opixel at specific location.
   * @param uiX:    Target X Coordinate of pixel
   * @param uiY:    Target Y Coordinate of pixel
   * @param oValue: Color to set on this pixel
   */
  virtual void drawPixel(int32 uiX, int32 uiY, const CcColor& oValue) = 0;

  //! @return Get height of this Pixmap
  inline int32 getHeight()
  { return getSize().getHeight(); }

   //! @return Get type of this imaging device
  virtual EType getType() const = 0;

  //! @return Get width and height of PixMap as Size
  virtual const CcSize& getSize() const = 0;

  //! @return Get width of this Pixmap
  inline int32 getWidth()
  { return getSize().getWidth(); }

};
