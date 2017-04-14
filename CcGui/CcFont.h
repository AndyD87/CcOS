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
 * @page      CcFont
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcFont
 */
#ifndef CCFONT_H_
#define CCFONT_H_

#include "CcBase.h"
#include "CcGui.h"
/**
 * @brief Class for Handeling Fonts
 */
class CcGuiSHARED CcFont {
public:
  /**
   * @brief Constructor with target Font-size
   */
  CcFont( uint16 uiFontSizeX );

  /**
   * @brief Destructor
   */
  virtual ~CcFont();

  /**
   * @brief Get Pixle-values from an char
   */
  char* getPixles(char cValue);

  /**
   * @brief Get width of actual Font
   */
  uint16 getFontSizeX( void )
    { return m_FontSizeX;}

  /**
   * @brief get height of actual Font
   */
  uint16 getFontSizeY( void )
    { return m_FontSizeY;}

protected:
  uint16 m_FontSize;    //!< Font-size set from constructor
  uint16 m_FontSizeX;   //!< Font-width of loaded Font
  uint16 m_FontSizeY;   //!< Font-height of loaded Font

  char* m_cFontBuffer;  //!< pointer to Buffer of loaded Font
};

#endif /* CCFONT_H_ */
