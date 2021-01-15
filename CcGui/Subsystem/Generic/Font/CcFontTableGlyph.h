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
#ifndef H_CcFontTableGlyph_H
#define H_CcFontTableGlyph_H

#include "CcBase.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcPoint.h"

/**
 * @brief Simple format for path descriptions in fonts
 */
class CSimple
{
public:
  /**
   * @addtogroup CSimplePointFlags Point flags
   * @{
   */
  static const uint8 ON_CURVE = 1;      //!< Point is describing an corve
  static const uint8 X_IS_BYTE = 2;     //!< Point value is X absolute
  static const uint8 Y_IS_BYTE = 4;     //!< Point value is Y absolute
  static const uint8 REPEAT = 8;        //!< Point has to be repeated
  static const uint8 X_DELTA = 16;      //!< Point value is X relative
  static const uint8 Y_DELTA = 32;      //!< Point value is Y relative
  /**
   * @}
   */

  /**
   * @brief Rescaale all x and y values from point.
   * @param fFactor: Factor to rescale
   */
  void scalePoints(float fFactor)
  {
    scalePoints(fFactor, fFactor);
  }

  /**
   * @brief Rescaale all x and y values from point
   * @param fXFactor: Factor for X to rescale
   * @param fYFactor: Factor for Y to rescale
   */
  void scalePoints(float fXFactor, float fYFactor);

  /**
   * @brief Move all points with absolute x and y value
   * @param uiX: Move on X Coordinates
   * @param uiY: Move on Y Coordinates
   */
  void movePoints(int16 uiX, int16 uiY);

  /**
   * @brief Execut transformation function for all points
   * @param fA      : A value for transformation function
   * @param fB      : B value for transformation function
   * @param fC      : C value for transformation function
   * @param fD      : D value for transformation function
   * @param iXOffset: X value for transformation function
   * @param iYOffset: Y value for transformation function
   */
  void transformPoints(float fA, float fB, float fC, float fD, int16 iXOffset, int16 iYOffset);
public:
  /**
   * @brief Point storage wit flags and coordinates
   */
  class CPoint
  {
  public:
    /**
     * @brief Create point with coordinates and flags
     * @param uiFlag: Flags to init with from @ref CSimplePointFlags
     * @param iX    : X Coordinate to init with
     * @param iY    : Y Coordinate to init with
     */
    CPoint(uint8 uiFlag, int iX, int iY) :
      uiFlag(uiFlag),
      oPoint(iX, iY)
    {
    }

    uint8 uiFlag = 0; //!< Flags from @ref CSimplePointFlags
    CcPoint oPoint;   //!< Defined points
  };

  CcList<uint16> oEndpointsOfContours;      //!< Number of contours available
  uint16         uiInstructionLength = 0;   //!< Number of instructions available to work with points
  CcList<uint8>  oInstructions;             //!< All instructions
  CcList<CPoint> oPoints;                   //!< All points
};

/**
 * @brief Storage of compontents and definition of basic structs
 */
class CCompound
{
public:
  /**
   * @todo Determine the usage of all flags, and do implementations.
   * @addtogroup CCompoundComponentsFlags
   * @{
   */
  static const uint16 ARG_1_AND_2_ARE_WORDS     = 1;    //!< Set but not used at the moment
  static const uint16 ARGS_ARE_XY_VALUES        = 2;    //!< Set but not used at the moment
  static const uint16 ROUND_XY_TO_GRID          = 4;    //!< Set but not used at the moment
  static const uint16 WE_HAVE_A_SCALE           = 8;    //!< Set but not used at the moment
  static const uint16 RESERVED                  = 16;   //!< Set but not used at the moment
  static const uint16 MORE_COMPONENTS           = 32;   //!< Set but not used at the moment
  static const uint16 WE_HAVE_AN_X_AND_Y_SCALE  = 64;   //!< Set but not used at the moment
  static const uint16 WE_HAVE_A_TWO_BY_TWO      = 128;  //!< Set but not used at the moment
  static const uint16 WE_HAVE_INSTRUCTIONS      = 256;  //!< Set but not used at the moment
  static const uint16 USE_MY_METRICS            = 512;  //!< Set but not used at the moment
  static const uint16 OVERLAP_COMPONENT         = 1024; //!< Set but not used at the moment
  /**
   * @}
   */

  /**
   * @brief Defint a matrix for transform operations
   */
  typedef struct
  {
    float fA; //!< A value for transformation.
    float fB; //!< B value for transformation.
    float fC; //!< C value for transformation.
    float fD; //!< D value for transformation.
  } Matrix;

  /**
   * @brief Component with transform infromations.
   */
  typedef struct
  {
    bool bDoTransform = false;  //!< true if transform is required
    uint16 uiIndex;             //!< Index of component
    Matrix oMatrix;             //!< Transform infromations
    int16 iXOffset;             //!< X value of point for transfomation
    int16 iYOffset;             //!< Y value of point for transfomation
  } CComponent;

  CcList<CComponent> oComponents; //!< List of compontnts to with transform infromations.
};

#pragma pack(push, 4)

/**
 * @brief The CcFontTableGlyph class
 */
class CcFontTableGlyph
{
public:
  /**
   * @brief Type of glyph table as Comound or Simple list.
   */
  enum class EType
  {
    Simple,
    Compound
  };

  int16 iNumberOfContours;  //!< Number of contours in glyph
  int16 iXMin;              //!< lowest X-Value
  int16 iYMin;              //!< highest X-Value
  int16 iXMax;              //!< lowest Y-Value
  int16 iYMax;              //!< highest Y-Value

  /**
   * @brief The definition for one component
   */
  class CComponent
  {
  public:
    uint16  uiFlags;      //!< Flags from @ref CCompoundComponentsFlags
    uint16  uiGlyphIndex; //!< Index of current component
    //! Arguments for component
    union
    {
      struct
      {
        int16 iArg1;  //!< First argument in int16 format
        int16 iArg2;  //!< Second argument in int16 format
      } i16;          //!< int16 component arguments
      struct
      {
        int8 iArg1;   //!< First argument in int8 format
        int8 iArg2;   //!< Second argument in int8 format
      } i8;           //!< int8 component arguments
    } Type;
    uint16 uiData[1]; //!< Location of first data element

    //! @return Already swapped flags
    uint16 getFlags()
     { return CcStatic::swapUint16(uiFlags);}
    //! @return Already swapped index
    uint16 getIndex()
     { return CcStatic::swapUint16(uiGlyphIndex);}
    //! @return Already swapped first int16 argument
    int16 getArg1I16()
     { return CcStatic::swapInt16(Type.i16.iArg1);}
    //! @return Already swapped second int16 argument
    int16 getArg2I16()
     { return CcStatic::swapInt16(Type.i16.iArg2);}
    //! @return First int8 argument
    int8 getArg1I8()
     { return Type.i8.iArg1;}
    //! @return Second int8 argument
    int8 getArg2I8()
      { return Type.i8.iArg2;}
  };

  /**
   * @brief Type depending content of glyph table, this union will be used
   *        to cast this pointer in target Compound or Simple list.
   */
  union
  {
    //! Content is a simple list of contour elements
    struct
    {
      uint16 pEndPtsOfContours[1]; //!< Begining of contour data list
    } Simple;
    //! Content is a Compound structore
    CComponent Compound;
  } Type;

  //! @return Get already swapped number of contours
  int16 getNumberOfContours()
  { return CcStatic::swapInt16(iNumberOfContours); }
  //! @return Get already swapped lowest X-Value
  int16 getXMin()
  { return CcStatic::swapInt16(iXMin); }
  //! @return Get already swapped Highes X-Value
  int16 getXMax()
  { return CcStatic::swapInt16(iXMax); }
  //! @return Get already swapped lowest Y-Value
  int16 getYMin()
  { return CcStatic::swapInt16(iYMin); }
  //! @return Get already swapped Highes Y-Value
  int16 getYMax()
  { return CcStatic::swapInt16(iYMax); }
  //! @return Get already swapped end of Countrours list
  uint16 getSimpleEndPtsOfContours(uint16 uiIndex)
  {return CcStatic::swapUint16(Type.Simple.pEndPtsOfContours[uiIndex]);}

  /**
   * @brief Get glyph at specific location.
   * @param uiOffset: Location of target glyph
   * @return Glyph pointer at target offset.
   */

  CcFontTableGlyph* getGlyph(uint32 uiOffset);
  /**
   * @brief Get current type of glyph list
   * @return Type as enum
   */
  EType getType();

  /**
   * @brief Interpret glyph content as compound object
   * @return Compound object from glyph data.
   */
  CCompound getCompoundGlyph();

  /**
   * @brief Interpret glyph content as simple object
   * @return Simple object from glyph data.
   */
  CSimple getSimleGlyph();
};

#pragma pack(pop)

#endif // H_CcFontTableGlyph_H
