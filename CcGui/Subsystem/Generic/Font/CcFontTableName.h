#ifndef CcFontTABLENAME_H
#define CcFontTABLENAME_H

#include "CcBase.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcString.h"

#pragma pack(push, 4)

class CcFontTableName
{
public:
  class CNameRecord
  {
  public:
    uint16 uiPlatformId;
    uint16 uiEncodingId;
    uint16 uiLanguageId;
    uint16 uiNameId;
    uint16 uiLength;
    uint16 uiOffset;

    uint16 getPlatformId()
      { return CcStatic::swapUint16(uiPlatformId); }
    uint16 getEncodingId()
      { return CcStatic::swapUint16(uiEncodingId); }
    uint16 getLanguageId()
      { return CcStatic::swapUint16(uiLanguageId); }
    uint16 getNameId()
      { return CcStatic::swapUint16(uiNameId); }
    uint16 getLength()
      { return CcStatic::swapUint16(uiLength); }
    uint16 getOffset()
      { return CcStatic::swapUint16(uiOffset); }

    CcString getName(CcFontTableName *pTable);
  };

  uint16 uiFormat;
  uint16 uiCount;
  uint16 uiOffset;
  CNameRecord oNameRecords[255];

  uint16 getFormat()
    { return CcStatic::swapUint16(uiFormat); }
  uint16 getCount()
    { return CcStatic::swapUint16(uiCount); }
  uint16 getOffset()
    { return CcStatic::swapUint16(uiOffset); }
  char* getStringStorage()
    { return CCVOIDPTRCAST(char*, this) + CcStatic::swapUint16(uiOffset); }
  CcString getNameById(uint16 iId);
  CcString getNameByOffset(uint16 iTableNr);
  CcList<CNameRecord> getAllRecords();

  inline CcString getCopyright()
    { return getNameById(0); }
  inline CcString getFamilyName()
    { return getNameById(1); }
  inline CcString getSubFamilyName()
    { return getNameById(2); }
  inline CcString getUniqueId()
    { return getNameById(3); }
  inline CcString getFullName()
    { return getNameById(4); }
  inline CcString getVersion()
    { return getNameById(5); }
  inline CcString getPostScriptName()
    { return getNameById(6); }
  inline CcString getTrademark()
    { return getNameById(7); }
  inline CcString getManurfacturer()
    { return getNameById(8); }
  inline CcString getDesigner()
    { return getNameById(9); }
  inline CcString getDescription()
    { return getNameById(10); }
  inline CcString getVendorUrl()
    { return getNameById(11); }
  inline CcString getDesignerUrl()
    { return getNameById(12); }
  inline CcString getLicense()
    { return getNameById(13); }
  inline CcString getLicenseUrl()
    { return getNameById(14); }
  inline CcString getReserved()
    { return getNameById(15); }
  inline CcString getTypographicFamily()
    { return getNameById(16); }
  inline CcString getTypographicSubFamily()
    { return getNameById(17); }
  // ... and so on (https://docs.microsoft.com/en-us/typography/opentype/spec/name#name-ids)
};

#pragma pack(pop)

#endif // CcFontTABLENAME_H
