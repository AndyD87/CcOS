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
 * @subpage   CcDisplay
 *
 * @page      CcDisplay
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDisplay
 */
#ifndef CCDISPLAY_H_
#define CCDISPLAY_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "Types/CcRectangle.h"

// forward declarations
class CcDisplayArea;

/**
 * @brief This class describes an Display Device wich is possible to
 *        show dot matrix.
 */
class CcKernelSHARED CcDisplay : public CcIODevice
{
public:
  /**
   * @brief Create this base class with it's size
   * @param oSize: Size of Display.
   */
  CcDisplay(const CcSize& oSize);

  /**
   * @brief Destructur
   */
  virtual ~CcDisplay();

  /**
   * @brief Overwrite read, Not every Display has the Ability
   *        to read data from it.
   * @return number of bytes read to buffer.
   */
  virtual size_t read(void*, size_t) override
    { return SIZE_MAX; }

  /**
   * @brief Overwrite write, Not every Display has the Ability
   *        to write data to it.
   * @return number of bytes read to buffer.
   */
  virtual size_t write(const void*, size_t) override
    { return SIZE_MAX; }

  /**
   * @brief Open Dispaly with common flags
   *
   * @return eFlags: Flags to pass to display.
   */
  virtual CcStatus open(EOpenFlags eFlags) = 0;

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
  virtual CcDisplayArea* getDisplayArea(const CcRectangle& oArea) = 0;

  /**
   * @brief Get width of Display
   * @return With as uint16
   */
  uint16 getWidth( void ) const
    {return m_oSize.getWidth();}

  /**
   * @brief Get height of Display
   * @return With as uint16
   */
  uint16 getHeight( void ) const
    {return m_oSize.getHeight();}

  /**
   * @brief Set Curser to next Pixel in Dot Matrix.
   */
  void nextCursor(void);

protected: //member
  CcSize m_oSize;
  CcPoint m_oCursor;
  uint16 m_DrawStartX = 0;
  uint16 m_DrawStartY = 0;
  uint16 m_DrawSizeX = 0;
  uint16 m_DrawSizeY = 0;
};

#endif /* CCDISPLAY_H_ */
