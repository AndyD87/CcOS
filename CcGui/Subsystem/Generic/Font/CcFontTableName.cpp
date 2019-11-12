#include "CcBase.h"
#include "CcFontTableName.h"
#include "CcStringUtil.h"

CcString CcFontTableName::CNameRecord::getName(CcFontTableName* pTable)
{
  CcString sName;
  char* pName = pTable->getStringStorage() + getOffset();
  if(getPlatformId() == 0 || // Unicode
     getPlatformId() == 3)   // Windows unicode
  {
    unsigned short* pUnicodeString = CCVOIDPTRCAST(unsigned short*, pName);
    CCNEWARRAYTYPE(pUnicodeStringData,unsigned short,getLength()/2);
    for(uint16 ui=0; ui<getLength()/2; ui++)
    {
      pUnicodeStringData[ui] = CcStatic::swapUint16(pUnicodeString[ui]);
    }
    sName = CcStringUtil::fromUtf16(pUnicodeStringData, getLength()/2);

    CCDELETEARRAY(pUnicodeStringData);
  }
  else
  {
    sName = CcString(pName, getLength());
  }
  return sName;
}

CcList<CcFontTableName::CNameRecord> CcFontTableName::getAllRecords()
{
  CcList<CcFontTableName::CNameRecord> oList;

  return oList;
}

CcString CcFontTableName::getNameById(uint16 iId)
{
  CcString sName;
  if(iId < getCount() &&
     iId == oNameRecords[iId].getNameId())
  {
    sName = oNameRecords[iId].getName(this);
  }
  else
  {
    for(uint16 ui=0; ui < getCount(); ui++)
    {
      if(iId == oNameRecords[ui].getNameId())
      {
        sName = oNameRecords[ui].getName(this);
        break;
      }
    }
  }
  return sName;
}

CcString CcFontTableName::getNameByOffset(uint16 iTableNr)
{
  CcString sName;
  if(iTableNr < getCount())
  {
    sName = oNameRecords[iTableNr].getName(this);
  }
  return sName;
}
