
#include "CcFontTableLoca.h"

uint32 CcFontTableLoca::getGlyphOffset(uint16 uiSign, int32 iIndexToLocFormat)
{
  uint32 uiGlyphOffset = 0;
/*bool readValues(CcFontFile& oFile, qint64 iSeek, uint16 uiIndex, int16 iIndexToLocFormat)*/
  if(iIndexToLocFormat == 1)
  {
    uint32* pData = CCVOIDPTRCAST(uint32*,CCVOIDPTRCAST(char*, this) + (uiSign*sizeof(uint32)));
    uiGlyphOffset = CcStatic::swapUint32(*pData);
  }
  else
  {
    uint16* pData = CCVOIDPTRCAST(uint16*,CCVOIDPTRCAST(char*, this) + (uiSign*sizeof(uint16)));
    uiGlyphOffset = static_cast<uint32>(CcStatic::swapUint16((*pData))) * 2;
  }
  return uiGlyphOffset;
}
