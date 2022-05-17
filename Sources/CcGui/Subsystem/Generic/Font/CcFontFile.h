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
 * @brief     Class CcFontFile
 */
#pragma once

#include "CcBase.h"
#include "CcFile.h"
#include "CcByteArray.h"
#include "CcFontTableGlyph.h"
#include "CcFontTableHead.h"
#include "CcFontTableLoca.h"
#include "CcFontTableName.h"
#include "CcFontTableMaxp.h"

class CcString;
class CcDateTime;

/**
 * @brief Font file to read font data from
 */
class CcFontFile
{
public:
  /**
   * @brief Create font file object and init private work data
   */
  CcFontFile();

  /**
   * @brief Delete font file object and deinit private work data
   */
  ~CcFontFile();

  //! @param sFilePath: Target file to open.
  void setFilePath(const CcString& sFilePath)
  { close(); m_oFile.setFilePath(sFilePath); }

  /**
   * @brief Try to open font file.
   * @return False if file could not be read
   */
  bool open();

  /**
   * @brief Close current opened font file
   */
  void close();

  /**
   * @brief Parse font file and extract all glyphs.
   */
  void parse();

  /**
   * @brief Get type of Glyph on specific index
   * @param uiSign: Target glyph to query
   * @return Type of glyph
   */
  CcFontTableGlyph::EType getGlyphType(uint16 uiSign);

  /**
   * @brief Get glyph at specified index in simple format.
   *        If it is the correct format, please verify with getGlyphType before
   * @param uiSign: Target glyph
   * @return Glyph in simple format.
   */
  CSimple getSimpleGlyph(uint16 uiSign);

  /**
   * @brief Get glyph at specified index in compound format.
   *        If it is the correct format, please verify with getGlyphType before
   * @param uiSign: Target glyph
   * @return Glyph in compound format.
   */
  CCompound getCompoundGlyph(uint16 uiSign);

  //! @return Number of pixels per em
  uint16 getUnitsPerEm();
  //! @return Get Head table from file
  CcFontTableHead* getHead();
  //! @return Get Name table from file
  CcFontTableName* getName();
  //! @return Get Maxp table from file
  CcFontTableMaxp* getMaxp();

private:
  uint64 pos()
    { return m_oFile.getFilePointer(); }
  bool seek(uint64 iPos)
    { return m_oFile.setFilePointer(iPos); }

  class CPrivate;
  CPrivate*   m_pPrivate = nullptr;
  CcByteArray m_oFileBuffer;
  CcFile      m_oFile;
};
