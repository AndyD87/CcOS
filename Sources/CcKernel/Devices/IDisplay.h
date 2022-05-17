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
#include "Types/CcRectangle.h"

// forward declarations
class CcGuiSubsystem;

/**
 * @brief This class describes an Display Device wich is possible to
 *        show dot matrix.
 */
class CcKernelSHARED IDisplay : public IDevice
{
public:
  /**
   * @brief Create this base class with it's size
   * @param oSize: Size of Display.
   */
  IDisplay(const CcSize& oSize) :
    m_oSize(oSize)
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
   * @brief Get an Drawing object wich is able to draw an a region in Display.
   * @param oArea: Location in Display to query for Painting object
   * @return Painting object or null if an error occured.
   */
  virtual CcGuiSubsystem* getGuiSubSystem(const CcRectangle& oArea) = 0;

  /**
   * @brief Get width of Display
   * @return With as int32
   */
  int32 getWidth() const
    {return m_oSize.getWidth();}

  /**
   * @brief Get height of Display
   * @return With as int32
   */
  int32 getHeight() const
    {return m_oSize.getHeight();}

  /**
   * @brief Set Curser to next Pixel in Dot Matrix.
   */
  void nextCursor();

protected: //member
  CcSize m_oSize;           //!< Size of display
  CcPoint m_oCursor;        //!< Location of Cursor
  int32 m_DrawStartX = 0;   //!< Current drawing Area Start coordinate X @todo replace with rectangle
  int32 m_DrawStartY = 0;   //!< Current drawing Area Start coordinate Y @todo replace with rectangle
  int32 m_DrawSizeX = 0;    //!< Current drawing Area width @todo replace with rectangle
  int32 m_DrawSizeY = 0;    //!< Current drawing Area height @todo replace with rectangle
};
