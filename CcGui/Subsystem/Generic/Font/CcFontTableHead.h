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
#pragma once

#include "CcBase.h"
#include "CcStatic.h"

#pragma pack(push, 4)

/**
 * @brief Font table head with some required informations of handling this font.
 */
class CcFontTableHead
{
public:
  uint16 uiMajorVersion;        //!< Pls look at standard for this value
  uint16 uiMinorVersion;        //!< Pls look at standard for this value
  int32 iFontRevision;          //!< Pls look at standard for this value
  uint32 uiChecksumAdjustment;  //!< Pls look at standard for this value
  uint32 uiMagicNumber;         //!< Pls look at standard for this value
  uint16 uiFlags;               //!< Pls look at standard for this value
  uint16 uiUintsPerEm;          //!< Pls look at standard for this value
  int64 iCreated;               //!< Pls look at standard for this value
  int64 iModified;              //!< Pls look at standard for this value
  int16 iXMin;                  //!< Pls look at standard for this value
  int16 iYMin;                  //!< Pls look at standard for this value
  int16 iXMax;                  //!< Pls look at standard for this value
  int16 iYMax;                  //!< Pls look at standard for this value
  uint16 uiMacStyle;            //!< Pls look at standard for this value
  uint16 uiLowestRecPPEM;       //!< Pls look at standard for this value
  int16 iFontDirectionHint;     //!< Pls look at standard for this value
  int16 iIndexToLocFormat;      //!< Pls look at standard for this value
  int16 iGlyphFormat;           //!< Pls look at standard for this value

  //! @return Get x min value already swapped
  int16 getXMin()
  { return CcStatic::swapInt16(iXMin); }
  //! @return Get x max value already swapped
  int16 getXMax()
  { return CcStatic::swapInt16(iXMax); }
  //! @return Get y min value already swapped
  int16 getYMin()
  { return CcStatic::swapInt16(iYMin); }
  //! @return Get y max value already swapped
  int16 getYMax()
  { return CcStatic::swapInt16(iYMax); }

  //! @return Get result of magic number comparision. True if number matches with standard
  bool checkMagicNumber()
  { return getMagicNumber() == 0x5f0f3cf5; }
  //! @return swapped magic number
  uint32 getMagicNumber()
  { return CcStatic::swapUint32(uiMagicNumber); }
  //! @return Get local format swapped
  int16 getIndexToLocFormat()
  { return CcStatic::swapInt16(iIndexToLocFormat); }
  //! @return Get units per em already swapped
  uint16 getUintsPerEm()
  { return CcStatic::swapUint16(uiUintsPerEm); }

};

#pragma pack(pop)
