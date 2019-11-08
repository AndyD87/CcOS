#ifndef CcFontTABLELOCA_H
#define CcFontTABLELOCA_H

#include "CcBase.h"
#include "CcStatic.h"

#pragma pack(push, 4)

class CcFontTableLoca
{
public:
  union
  {
    uint16 ui16[1];
    uint32 ui32[1];
  } Offsets;

  uint32 getGlyphOffset(uint16 uiSign, int32 iIndexToLocFormat);
};
#pragma pack(pop)

#endif // CcFontTABLELOCA_H
