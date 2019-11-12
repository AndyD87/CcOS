#ifndef CcFontTableMaxp_H
#define CcFontTableMaxp_H

#include "CcBase.h"
#include "CcStatic.h"
#include "CcList.h"

#pragma pack(push, 4)

class CcFontTableMaxp
{
public:
  uint32 uiVersion;
  uint16 uiNumGlyphs;
  uint16 uiMaxPoints;
  uint16 uiMaxContours;
  uint16 uiMaxCompositePoints;
  uint16 uiMaxCompositeContours;
  uint16 uiMaxZones;
  uint16 uiMaxTwilightPoints;
  uint16 uiMaxStorage;
  uint16 uiMaxFunctionDefs;
  uint16 uiMaxInstructionDefs;
  uint16 uiMaxStackElements;
  uint16 uiMaxSizeOfInstructions;
  uint16 uiMaxComponentElements;
  uint16 uiMaxComponentDepth;

  uint32 getVersion()
    { return CcStatic::swapUint32(uiVersion);              }
  uint16 getNumGlyphs()
    { return CcStatic::swapUint16(uiNumGlyphs);            }
  uint16 getMaxPoints()
    { return CcStatic::swapUint16(uiMaxPoints);            }
  uint16 getMaxContours()
    { return CcStatic::swapUint16(uiMaxContours);          }
  uint16 getMaxCompositePoints()
    { return CcStatic::swapUint16(uiMaxCompositePoints);   }
  uint16 getMaxCompositeContours()
    { return CcStatic::swapUint16(uiMaxCompositeContours); }
  uint16 getMaxZones()
    { return CcStatic::swapUint16(uiMaxZones);             }
  uint16 getMaxTwilightPoints()
    { return CcStatic::swapUint16(uiMaxTwilightPoints);    }
  uint16 getMaxStorage()
    { return CcStatic::swapUint16(uiMaxStorage);           }
  uint16 getMaxFunctionDefs()
    { return CcStatic::swapUint16(uiMaxFunctionDefs);      }
  uint16 getMaxInstructionDefs()
    { return CcStatic::swapUint16(uiMaxInstructionDefs);   }
  uint16 getMaxStackElements()
    { return CcStatic::swapUint16(uiMaxStackElements);     }
  uint16 getMaxSizeOfInstructions()
    { return CcStatic::swapUint16(uiMaxSizeOfInstructions);}
  uint16 getMaxComponentElements()
    { return CcStatic::swapUint16(uiMaxComponentElements); }
  uint16 getMaxComponentDepth()
    { return CcStatic::swapUint16(uiMaxComponentDepth);    }
};

#pragma pack(pop)

#endif // CcFontTableMaxp_H
