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

class CcFontFile
{
public:
  CcFontFile();
  ~CcFontFile();

  void setFilePath(const CcString& sFilePath)
    { close(); m_oFile.setFilePath(sFilePath); }

  bool open();
  void close();
  void parse();

  CcFontTableGlyph::EType getGlyphType(uint16 uiSign);
  CSimple getSimpleGlyph(uint16 uiSign);
  CCompound getCompoundGlyph(uint16 uiSign);
  uint16 getUnitsPerEm();
  CcFontTableHead* getHead();
  CcFontTableName* getName();
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
