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
 * @brief     Class CcFont
 */
#pragma once

#include "CcBase.h"
#include "Font/CcFontFile.h"
#include "Font/CcFontTableGlyph.h"
#include "CcString.h"
#include "CcPoint.h"

/**
 * @brief Basic class for managing a font.
 */
class CcFont
{
public:
  /**
   * @brief Initialize and prepare for loading a font.
   */
  CcFont();

  /**
   * @brief Load font from file
   * @param sPath: Path to file
   */
  void loadFont(const CcString& sPath);

  /**
   * @brief Get simple glyph at sign index from loaded font.
   *        If glyph is a simple glyph, can be verified by getGlyphType
   * @param uiSign: Target index of glyph
   * @return Glyph in simple format
   */
  CSimple getSimpleGlyph(uint16 uiSign)
  { return m_oFontFile.getSimpleGlyph(uiSign);}

  /**
   * @brief Get compound glyph at sign index from loaded font.
   *        If glyph is a compound glyph, can be verified by getGlyphType
   * @param uiSign: Target index of glyph
   * @return Glyph in compound format
   */
  CCompound getCompoundGlyph(uint16 uiSign)
  { return m_oFontFile.getCompoundGlyph(uiSign);}

  /**
   * @brief Get type of glyph at specified index.
   * @param uiSign: Target sign to get info from
   * @return Read infor from Font file.
   */
  CcFontTableGlyph::EType getGlyphType(uint16 uiSign)
  { return m_oFontFile.getGlyphType(uiSign);}
  //QPixmap drawGlyph(uint16 uiSign);

  /**
   * @brief Get name of font, loaded from font file.
   * @return Name as String.
   */
  CcString getName();
private:
  //void drawGlyph(QPixmap& oPixMap, CcPoint& oCenter, const CSimple& oGlyph);
  class CPrivate;
private:
  CcString m_sFontFile;
  CcFontFile m_oFontFile;
};
