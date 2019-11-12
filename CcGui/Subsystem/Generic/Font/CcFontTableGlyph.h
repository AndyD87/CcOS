
#ifndef CcFontTABLEGLYPH_H
#define CcFontTABLEGLYPH_H

#include "CcBase.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcPoint.h"


class CSimple
{
public:
  static const uint8 ON_CURVE = 1;
  static const uint8 X_IS_BYTE = 2;
  static const uint8 Y_IS_BYTE = 4;
  static const uint8 REPEAT = 8;
  static const uint8 X_DELTA = 16;
  static const uint8 Y_DELTA = 32;

  void scalePoints(float fFactor)
  {
    scalePoints(fFactor, fFactor);
  }
  void scalePoints(float fXFactor, float fYFactor);
  void movePoints(int16 uiX, int16 uiY);
  void transformPoints(float fA, float fB, float fC, float fD, int16 iXOffset, int16 iYOffset);
public:
  class CPoint
  {
  public:
    CPoint(uint8 uiFlag, int iX, int iY) :
      uiFlag(uiFlag),
      oPoint(iX, iY)
    {
    }
    uint8 uiFlag = 0;
    CcPoint oPoint;
  };

  CcList<uint16> oEndpointsOfContours;
  uint16        uiInstructionLength = 0;
  uint16        uiInstructionLength2 = 0;
  CcList<uint8>  oInstructions;
  CcList<CPoint> oPoints;
};

class CCompound
{
public:
  static const uint16 ARG_1_AND_2_ARE_WORDS = 1;
  static const uint16 ARGS_ARE_XY_VALUES = 2;
  static const uint16 ROUND_XY_TO_GRID = 4;
  static const uint16 WE_HAVE_A_SCALE = 8;
  static const uint16 RESERVED = 16;
  static const uint16 MORE_COMPONENTS = 32;
  static const uint16 WE_HAVE_AN_X_AND_Y_SCALE = 64;
  static const uint16 WE_HAVE_A_TWO_BY_TWO = 128;
  static const uint16 WE_HAVE_INSTRUCTIONS = 256;
  static const uint16 USE_MY_METRICS = 512;
  static const uint16 OVERLAP_COMPONENT = 1024;

  typedef struct
  {
    float fA;
    float fB;
    float fC;
    float fD;
  } Matrix;
  typedef struct
  {
    bool bDoTransform = false;
    uint16 uiIndex;
    Matrix oMatrix;
    int16 iXOffset;
    int16 iYOffset;
  } CComponent;

  CcList<CComponent> oComponents;
};

#pragma pack(push, 4)

class CcFontTableGlyph
{
public:
  enum class EType
  {
    Simple,
    Compound
  };

  int16 iNumberOfContours;
  int16 iXMin;
  int16 iYMin;
  int16 iXMax;
  int16 iYMax;

  class CComponent
  {
  public:
    uint16	uiFlags;
    uint16	uiGlyphIndex;
    union
    {
      struct
      {
        int16 iArg1;
        int16 iArg2;
      } i16;
      struct
      {
        int8 iArg1;
        int8 iArg2;
      } i8;
    } Type;
    uint16 uiData[1];

    uint16 getFlags()
     { return CcStatic::swapUint16(uiFlags);}
    uint16 getIndex()
     { return CcStatic::swapUint16(uiGlyphIndex);}
    int16 getArg1I16()
     { return CcStatic::swapInt16(Type.i16.iArg1);}
    int16 getArg2I16()
     { return CcStatic::swapInt16(Type.i16.iArg2);}
    int8 getArg1I8()
     { return Type.i8.iArg1;}
    int8 getArg2I8()
      { return Type.i8.iArg2;}
  };

  union
  {
    struct
    {
      uint16 pEndPtsOfContours[1];
    } Simple;
    CComponent Compound;
  } Type;

  int16 getNumberOfContours()
    { return CcStatic::swapInt16(iNumberOfContours); }
  int16 getXMin()
    { return CcStatic::swapInt16(iXMin); }
  int16 getXMax()
    { return CcStatic::swapInt16(iXMax); }
  int16 getYMin()
    { return CcStatic::swapInt16(iYMin); }
  int16 getYMax()
    { return CcStatic::swapInt16(iYMax); }

  uint16 getSimpleEndPtsOfContours(uint16 uiIndex)
    {return CcStatic::swapUint16(Type.Simple.pEndPtsOfContours[uiIndex]);}

  CcFontTableGlyph* getGlyph(uint32 uiOffset);
  EType getType();
  CCompound getCompoundGlyph();
  CSimple getSimleGlyph();
};

#pragma pack(pop)

#endif // CcFontTABLEGLYPH_H
