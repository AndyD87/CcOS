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
 * @brief     Class IDisplay
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "IDrawingArea.h"
#include "CcColor.h"
#include "Types/CcRectangle.h"

// forward declarations
class CcGuiSubsystem;

/**
 * @brief This class describes an Display Device wich is possible to
 *        show dot matrix.
 */
class CcKernelSHARED IDisplay : public IDevice, public IDrawingArea
{
public:
  enum class EType
  {
    MonoChrome = 0,
    GrayScale,
    Color,
  };

  /**
   * @brief Create this base class with it's size
   * @param oSize: Size of Display.
   */
  IDisplay(const CcSize& oSize) :
    IDrawingArea(oSize)
  {}

  /**
   * @brief Destructur
   */
  virtual ~IDisplay() = default;

  /**
   * @brief Set Brightness of Backlight
   *
   * @return uiBrightness: New brightness value
   */
  virtual void setBacklight( uint8 uiBrightness ) = 0;

  /**
   * @brief Set pixel value at specific location.
   * @param uiX:    Target X Coordinate of pixel
   * @param uiY:    Target Y Coordinate of pixel
   * @param oValue: Color to set on this pixel
   */
  virtual void setPixel(int32 uiX, int32 uiY, const CcColor& oValue) = 0;
  virtual void setPixel(int32 uiX, int32 uiY, uint8 uiGreyScaleValue) = 0;
  virtual void setPixel(int32 uiX, int32 uiY, bool bMonochromValue) = 0;

  //! @return Get type of this imaging device
  virtual EType getType() const = 0;

  virtual void draw() = 0;
protected: //member
};
