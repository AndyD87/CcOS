#ifndef CcFontTABLENAME_H
#define CcFontTABLENAME_H

#include "CcBase.h"
#include "CcStatic.h"
#include <QList>
#include <QString>

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

    QString getName(CcFontTableName *pTable);
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
  QString getNameById(uint16 iId);
  QString getNameByOffset(uint16 iTableNr);
  QList<CNameRecord> getAllRecords();

  inline QString getCopyright()
    { return getNameById(0); }
  inline QString getFamilyName()
    { return getNameById(1); }
  inline QString getSubFamilyName()
    { return getNameById(2); }
  inline QString getUniqueId()
    { return getNameById(3); }
  inline QString getFullName()
    { return getNameById(4); }
  inline QString getVersion()
    { return getNameById(5); }
  inline QString getPostScriptName()
    { return getNameById(6); }
  inline QString getTrademark()
    { return getNameById(7); }
  inline QString getManurfacturer()
    { return getNameById(8); }
  inline QString getDesigner()
    { return getNameById(9); }
  inline QString getDescription()
    { return getNameById(10); }
  inline QString getVendorUrl()
    { return getNameById(11); }
  inline QString getDesignerUrl()
    { return getNameById(12); }
  inline QString getLicense()
    { return getNameById(13); }
  inline QString getLicenseUrl()
    { return getNameById(14); }
  inline QString getReserved()
    { return getNameById(15); }
  inline QString getTypographicFamily()
    { return getNameById(16); }
  inline QString getTypographicSubFamily()
    { return getNameById(17); }
  // ... and so on (https://docs.microsoft.com/en-us/typography/opentype/spec/name#name-ids)
};

#pragma pack(pop)

#endif // CcFontTABLENAME_H
