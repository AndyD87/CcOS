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
 * @subpage   CcFont
 *
 * @page      CcFont
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFont
 */
#ifndef H_CcFont_H
#define H_CcFont_H

#include "CcBase.h"
#include "Font/CcFontFile.h"
#include "Font/CcFontTableGlyph.h"
#include "CcString.h"
#include "CcPoint.h"

class CcFont
{
public:
  CcFont();

  void loadFont(const CcString& sPath);
  CSimple getSimpleGlyph(uint16 uiSign)
    { return m_oFontFile.getSimpleGlyph(uiSign);}
  CCompound getCompoundGlyph(uint16 uiSign)
    { return m_oFontFile.getCompoundGlyph(uiSign);}
  CcFontTableGlyph::EType getGlyphType(uint16 uiSign)
    { return m_oFontFile.getGlyphType(uiSign);}
  //QPixmap drawGlyph(uint16 uiSign);

  CcString getName();
private:
  //void drawGlyph(QPixmap& oPixMap, CcPoint& oCenter, const CSimple& oGlyph);
  class CPrivate;
private:
  CcString m_sFontFile;
  CcFontFile m_oFontFile;
};

#endif // H_CcFont_H
