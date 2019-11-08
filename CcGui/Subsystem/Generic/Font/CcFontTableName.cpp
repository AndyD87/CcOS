#include "CcBase.h"
#include "CcFontTableName.h"

QString CcFontTableName::CNameRecord::getName(CcFontTableName* pTable)
{
  QString sName;
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
    sName = QString::fromUtf16(pUnicodeStringData, getLength()/2);

    CCDELETEARRAY(pUnicodeStringData);
  }
  else
  {
    sName = QString::fromUtf8(pName, getLength());
  }
  return sName;
}

QList<CcFontTableName::CNameRecord> CcFontTableName::getAllRecords()
{
  QList<CcFontTableName::CNameRecord> oList;

  return oList;
}

QString CcFontTableName::getNameById(uint16 iId)
{
  QString sName;
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


QString CcFontTableName::getNameByOffset(uint16 iTableNr)
{
  QString sName;
  if(iTableNr < getCount())
  {
    sName = oNameRecords[iTableNr].getName(this);
  }
  return sName;
}
