#include "CcBase.h"
#include "CcFontTableGlyph.h"
#include "CcFontStatic.h"
#include "CcStatic.h"

CcFontTableGlyph* CcFontTableGlyph::getGlyph(uint32 uiOffset)
{
  return CCVOIDPTRCAST(CcFontTableGlyph*, CCVOIDPTRCAST(char*,this) + uiOffset);
}

CcFontTableGlyph::EType CcFontTableGlyph::getType()
{
  EType eType;
  if(getNumberOfContours() ==  -1)
  {
    eType = EType::Compound;
  }
  else
  {
    eType = EType::Simple;
  }
  return eType;
}

#define CcFontTableGlyph_IncCompound(COMPOUND) COMPOUND = (CCVOIDPTRCAST(CComponent*, (CCVOIDPTRCAST(char*, COMPOUND) + sizeof(uint16))))

CcFontTableGlyph::CCompound CcFontTableGlyph::getCompoundGlyph()
{
  CCompound oCompound;
  CComponent* pCompound = &Type.Compound;
  uint16 uiFlags = CCompound::MORE_COMPONENTS;
  while(IS_FLAG_SET(uiFlags, CCompound::MORE_COMPONENTS))
  {
    CCompound::CComponent oComponent;
    CcStatic::memset(&oComponent, 0, sizeof(oComponent));
    uint16* pData = pCompound->uiData;
    uiFlags = pCompound->getFlags();
    oComponent.uiIndex = pCompound->getIndex();
    if (IS_FLAG_SET(uiFlags, CCompound::ARG_1_AND_2_ARE_WORDS))
    {
      oComponent.iXOffset = pCompound->getArg1I16();
      oComponent.iYOffset = pCompound->getArg2I16();
      CcFontTableGlyph_IncCompound(pCompound);
      CcFontTableGlyph_IncCompound(pCompound);
    }
    else
    {
      oComponent.iXOffset = pCompound->getArg1I8();
      oComponent.iYOffset = pCompound->getArg2I8();
      pData--;
      CcFontTableGlyph_IncCompound(pCompound);
    }
    // Increment for beginnig data
    CcFontTableGlyph_IncCompound(pCompound);
    CcFontTableGlyph_IncCompound(pCompound);

    if (IS_FLAG_SET(uiFlags, CCompound::WE_HAVE_A_SCALE))
    {
      oComponent.bDoTransform = true;
      oComponent.oMatrix.fA = CcFontStatic::uint16ToFloat(pData[0]);
      oComponent.oMatrix.fD = oComponent.oMatrix.fA;
      CcFontTableGlyph_IncCompound(pCompound);
    }
    else if (IS_FLAG_SET(uiFlags, CCompound::WE_HAVE_AN_X_AND_Y_SCALE))
    {
      oComponent.bDoTransform = true;
      oComponent.oMatrix.fA = CcFontStatic::uint16ToFloat(pData[0]);
      oComponent.oMatrix.fD = CcFontStatic::uint16ToFloat(pData[1]);
      CcFontTableGlyph_IncCompound(pCompound);
      CcFontTableGlyph_IncCompound(pCompound);
    }
    else if (IS_FLAG_SET(uiFlags, CCompound::WE_HAVE_A_TWO_BY_TWO))
    {
      oComponent.bDoTransform = true;
      oComponent.oMatrix.fA = CcFontStatic::uint16ToFloat(pData[0]);
      oComponent.oMatrix.fB = CcFontStatic::uint16ToFloat(pData[1]);
      oComponent.oMatrix.fC = CcFontStatic::uint16ToFloat(pData[2]);
      oComponent.oMatrix.fD = CcFontStatic::uint16ToFloat(pData[3]);
      CcFontTableGlyph_IncCompound(pCompound);
      CcFontTableGlyph_IncCompound(pCompound);
      CcFontTableGlyph_IncCompound(pCompound);
      CcFontTableGlyph_IncCompound(pCompound);
    }
    oCompound.oComponents.append(oComponent);
  }
  return oCompound;
}

CcFontTableGlyph::CSimple CcFontTableGlyph::getSimleGlyph()
{
  CSimple oSimple;
  uint8* pInstructions  = nullptr;
  uint8* pFlags         = nullptr;
  uint8* pCoordinates   = nullptr;
  uint16 uiMaxPoints    = 0;
  // get positive value, because we know we are >0
  uint16 uiNumber = static_cast<uint16>(getNumberOfContours());
  for(uint16 i=0; i< uiNumber; i++)
  {
    uint16 uiPoints = getSimpleEndPtsOfContours(i);
    if(uiPoints > uiMaxPoints)
      uiMaxPoints = uiPoints;
    oSimple.oEndpointsOfContours.append(getSimpleEndPtsOfContours(i));
  }
  oSimple.uiInstructionLength = getSimpleEndPtsOfContours(uiNumber);
  uiMaxPoints++;


  pInstructions = CCVOIDPTRCAST(uint8*, &Type.Simple.pEndPtsOfContours[uiNumber + 1]);
  for(uint16 i=0; i< oSimple.uiInstructionLength; i++)
  {
    oSimple.oInstructions.append(pInstructions[i]);
  }

  pFlags = CCVOIDPTRCAST(uint8*, &pInstructions[oSimple.uiInstructionLength]);
  for(uint16 uiVal = 0; uiVal < uiMaxPoints; uiVal++, pCoordinates++)
  {
    uint8 uiFlag = *pFlags;
    pFlags++;
    CSimple::CPoint oPoint(uiFlag,0,0);
    oSimple.oPoints.append(oPoint);
    if(IS_FLAG_SET(uiFlag, CSimple::REPEAT))
    {
      uint8 uiRepeats = *pFlags;
      pFlags++;
      while(uiRepeats--)
      {
        uiVal++;
        oSimple.oPoints.append(oPoint);
      }
    }
  }

  int16 iValue = 0;
  for( uint16 i = 0; i < uiMaxPoints; i++ )
  {
    uint8 flag = oSimple.oPoints[i].uiFlag;
    if ( flag & CSimple::X_IS_BYTE )
    {
      if ( flag & CSimple::X_DELTA )
      {
        iValue +=  *pFlags;
        pFlags++;
      }
      else
      {
        iValue -=  *pFlags;
        pFlags++;
      }
    }
    else if ( ~flag & CSimple::X_DELTA )
    {
      iValue += (static_cast<int16>(pFlags[0]) << 8) | pFlags[1];
      pFlags++;
      pFlags++;
    }
    else
    {
      // value is unchanged.
    }

    oSimple.oPoints[i].oPoint.x() = iValue;
  }
  iValue = 0;
  for( uint16 i = 0; i < uiMaxPoints; i++ )
  {
    uint8 flag = oSimple.oPoints[i].uiFlag;
    if ( flag & CSimple::Y_IS_BYTE )
    {
      if ( flag & CSimple::Y_DELTA )
      {
        iValue +=  *pFlags;
        pFlags++;
      }
      else
      {
        iValue -=  *pFlags;
        pFlags++;
      }
    }
    else if ( ~flag & CSimple::Y_DELTA )
    {
      iValue += (static_cast<int16>(pFlags[0]) << 8) | pFlags[1];
      pFlags++;
      pFlags++;
    }
    else
    {
      // value is unchanged.
    }
    oSimple.oPoints[i].oPoint.y() = iValue;
  }
  return oSimple;
}

void CcFontTableGlyph::CSimple::scalePoints(float fXFactor, float fYFactor)
{
  for(CPoint& rPoint : oPoints)
  {
    rPoint.oPoint.x() = static_cast<int>(static_cast<float>(rPoint.oPoint.x()) * fXFactor);
    rPoint.oPoint.y() = static_cast<int>(static_cast<float>(rPoint.oPoint.y()) * fYFactor);
  }
}

void CcFontTableGlyph::CSimple::movePoints(int16 uiX, int16 uiY)
{
  for(CPoint& rPoint : oPoints)
  {
    rPoint.oPoint.x() += uiX;
    rPoint.oPoint.y() += uiY;
  }
}

void CcFontTableGlyph::CSimple::transformPoints(float fA, float fB, float fC, float fD, int16 iXOffset, int16 iYOffset)
{
  // https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6glyf.html
  float fAn = (fA > 0)? fA : -fA;
  float fBn = (fB > 0)? fB : -fB;
  float fCn = (fC > 0)? fC : -fC;
  float fDn = (fD > 0)? fD : -fD;
  float m = CCMAX(fAn, fBn);
  float n = CCMAX(fCn, fDn);
  float border = static_cast<float>(33)/65536;
  if((fAn - fCn) < border) m = m * 2;
  if((fAn - fCn) < border) n = n * 2;
  for(CPoint& rPoint : oPoints)
  {
    float x = static_cast<float>(rPoint.oPoint.x());
    float y = static_cast<float>(rPoint.oPoint.x());
    rPoint.oPoint.x() = static_cast<int>(m*(((fA/m)*x) + ((fC/m)*y) + iXOffset));
    rPoint.oPoint.y() = static_cast<int>(n*(((fB/n)*x) + ((fD/n)*y) + iYOffset));
  }
}
