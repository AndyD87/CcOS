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
 * @brief     Class CcFontTableHead
 */
#ifndef CcFontTABLEHEAD_H
#define CcFontTABLEHEAD_H

#include "CcBase.h"
#include "CcStatic.h"

#pragma pack(push, 4)

class CcFontTableHead
{
public:
  uint16 uiMajorVersion;
  uint16 uiMinorVersion;
  int32 iFontRevision;
  uint32 uiChecksumAdjustment;
  uint32 uiMagicNumber;
  uint16 uiFlags;
  uint16 uiUintsPerEm;
  int64 iCreated;
  int64 iModified;
  int16 iXMin;
  int16 iYMin;
  int16 iXMax;
  int16 iYMax;
  uint16 uiMacStyle;
  uint16 uiLowestRecPPEM;
  int16 iFontDirectionHint;
  int16 iIndexToLocFormat;
  int16 iGlyphFormat;

  int16 getXMin()
    { return CcStatic::swapInt16(iXMin); }
  int16 getXMax()
    { return CcStatic::swapInt16(iXMax); }
  int16 getYMin()
    { return CcStatic::swapInt16(iYMin); }
  int16 getYMax()
    { return CcStatic::swapInt16(iYMax); }

  bool checkMagicNumber()
    { return getMagicNumber() == 0x5f0f3cf5; }
  uint32 getMagicNumber()
    { return CcStatic::swapUint32(uiMagicNumber); }
  int16 getIndexToLocFormat()
    { return CcStatic::swapInt16(iIndexToLocFormat); }
  uint16 getUintsPerEm()
    { return CcStatic::swapUint16(uiUintsPerEm); }

};

#pragma pack(pop)

#endif // CcFontTABLEHEAD_H
