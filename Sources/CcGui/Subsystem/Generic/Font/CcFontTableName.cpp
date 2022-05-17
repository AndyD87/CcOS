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
 * @brief     Class CcFontTableName
 */
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
