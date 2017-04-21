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
 * @page      CcGui
 * @subpage   CcText
 *
 * @page      CcText
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcText
 */

#ifndef CCTEXT_H_
#define CCTEXT_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcFont.h"
#include "CcString.h"

/**
 * @brief Class for creating an manipulating a Textbox on Display.
 */
class CcGuiSHARED CcText : public CcWidget, CcFont {
public:
  /**
   * @brief Constructor with Font-size of displaying text
   * @param fontSize: FontSize of Displayin Text in pixles, default:6
   */
  CcText(CcWidget* parent, uint16 fontSize = 8);
  /**
   * @brief Destructor
   */
  virtual ~CcText();

  /**
   * @brief Set Color of Font
   */
  void setFontColor(uchar R, uchar G, uchar B);
  /**
   * @brief Set Offset where Text has to Start on Display
   */
  void setTextOffset(uint16 x, uint16 y );

  /**
   * @brief Set String that has to be shown on Display
   */
  void setString(const CcString& sString);

  /**
   * @brief Flush Text out on Display
   */
  void drawString( void );

  /**
   * @brief Get the Size of Window Text will need to show
   */
  void getTextSize( uint16* x, uint16* y );

  /**
   * @brief get Handle to internal String-Member
   */
  const CcString& getString( void );
protected:
  /**
   * @brief Start the Calulation for Size of Window for displaying Text
   */
  void calcTextSize( void );
private:
  /**
   * @brief Write a Char to Display, Position was previously set.
   * @param cValue: Char to Write
   */
  void writeChar(char cValue);

private: //members
  CcString m_sString;   //!< String for Display
  CcColor m_cFontColor;
  uint16 m_uiOffsetX;   //!< Position-Offest Y-Value Text is shown
  uint16 m_uiOffsetY;   //!< Position-Offest X-Value Text is shown
  uint16 m_TextSizeX;   //!< Calculated width in Pixles of showing Text
  uint16 m_TextSizeY;   //!< Calculated height in Pixles of showing Text
};

#endif /* CCTEXT_H_ */
