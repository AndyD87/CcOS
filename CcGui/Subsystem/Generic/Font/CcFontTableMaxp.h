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
 * @brief     Class CcFontTableMaxp
 */
#pragma once

#include "CcBase.h"
#include "CcStatic.h"
#include "CcList.h"

#pragma pack(push, 4)

/**
 * @brief Table with maxp information. Currently not in use
 */
class CcFontTableMaxp
{
public:
  uint32 uiVersion;               //!< Pls look at standard for this value
  uint16 uiNumGlyphs;             //!< Pls look at standard for this value
  uint16 uiMaxPoints;             //!< Pls look at standard for this value
  uint16 uiMaxContours;           //!< Pls look at standard for this value
  uint16 uiMaxCompositePoints;    //!< Pls look at standard for this value
  uint16 uiMaxCompositeContours;  //!< Pls look at standard for this value
  uint16 uiMaxZones;              //!< Pls look at standard for this value
  uint16 uiMaxTwilightPoints;     //!< Pls look at standard for this value
  uint16 uiMaxStorage;            //!< Pls look at standard for this value
  uint16 uiMaxFunctionDefs;       //!< Pls look at standard for this value
  uint16 uiMaxInstructionDefs;    //!< Pls look at standard for this value
  uint16 uiMaxStackElements;      //!< Pls look at standard for this value
  uint16 uiMaxSizeOfInstructions; //!< Pls look at standard for this value
  uint16 uiMaxComponentElements;  //!< Pls look at standard for this value
  uint16 uiMaxComponentDepth;     //!< Pls look at standard for this value

  //! @return Get version swapped
  uint32 getVersion()
  { return CcStatic::swapUint32(uiVersion);              }
  //! @return Get number of glyphs already swapped
  uint16 getNumGlyphs()
  { return CcStatic::swapUint16(uiNumGlyphs);            }
  //! @return Get maximum number of points per glyph already swapped
  uint16 getMaxPoints()
  { return CcStatic::swapUint16(uiMaxPoints);            }
  //! @return Get maximum number of contours per glyph already swapped
  uint16 getMaxContours()
  { return CcStatic::swapUint16(uiMaxContours);          }
  //! @return Get maximum number of composit points per glyph already swapped
  uint16 getMaxCompositePoints()
  { return CcStatic::swapUint16(uiMaxCompositePoints);   }
  //! @return Get maximum number of composit contours already swapped
  uint16 getMaxCompositeContours()
  { return CcStatic::swapUint16(uiMaxCompositeContours); }
  //! @return Get maximum number of zones already swapped
  uint16 getMaxZones()
  { return CcStatic::swapUint16(uiMaxZones);             }
  //! @return Get maximum number of twilight points already swapped
  uint16 getMaxTwilightPoints()
  { return CcStatic::swapUint16(uiMaxTwilightPoints);    }
  //! @return Get maximum storage size already swapped
  uint16 getMaxStorage()
  { return CcStatic::swapUint16(uiMaxStorage);           }
  //! @return Get maximum number of function definitions already swapped
  uint16 getMaxFunctionDefs()
  { return CcStatic::swapUint16(uiMaxFunctionDefs);      }
  //! @return Get maximum number of instruction definitions already swapped
  uint16 getMaxInstructionDefs()
  { return CcStatic::swapUint16(uiMaxInstructionDefs);   }
  //! @return Get maximum number of stack elements already swapped
  uint16 getMaxStackElements()
  { return CcStatic::swapUint16(uiMaxStackElements);     }
  //! @return Get maximum size of one instruction already swapped
  uint16 getMaxSizeOfInstructions()
  { return CcStatic::swapUint16(uiMaxSizeOfInstructions);}
  //! @return Get maximum size of one component element already swapped
  uint16 getMaxComponentElements()
  { return CcStatic::swapUint16(uiMaxComponentElements); }
  //! @return Get maximum components recursive calls already swapped
  uint16 getMaxComponentDepth()
  { return CcStatic::swapUint16(uiMaxComponentDepth);    }
};

#pragma pack(pop)
