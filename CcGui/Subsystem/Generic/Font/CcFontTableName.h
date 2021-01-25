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
#pragma once

#include "CcBase.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcString.h"

#pragma pack(push, 4)

/**
 * @brief Font table with name and language info.
 */
class CcFontTableName
{
public:
  /**
   * @brief Definition of a name record within table.
   */
  class CNameRecord
  {
  public:
    uint16 uiPlatformId;  //!< Pleas refer to font standard
    uint16 uiEncodingId;  //!< Pleas refer to font standard
    uint16 uiLanguageId;  //!< Pleas refer to font standard
    uint16 uiNameId;      //!< Pleas refer to font standard
    uint16 uiLength;      //!< Pleas refer to font standard
    uint16 uiOffset;      //!< Pleas refer to font standard

    //! @return Get platform id already swapped
    uint16 getPlatformId()
    { return CcStatic::swapUint16(uiPlatformId); }
    //! @return Get encoding id already swapped
    uint16 getEncodingId()
    { return CcStatic::swapUint16(uiEncodingId); }
    //! @return Get language id already swapped
    uint16 getLanguageId()
    { return CcStatic::swapUint16(uiLanguageId); }
    //! @return Get name id already swapped
    uint16 getNameId()
    { return CcStatic::swapUint16(uiNameId); }
    //! @return Get length of name already swapped
    uint16 getLength()
    { return CcStatic::swapUint16(uiLength); }
    //! @return Get offset of name already swapped
    uint16 getOffset()
    { return CcStatic::swapUint16(uiOffset); }

    /**
     * @brief Extract name of table with data from this record.
     * @param pTable: Table to read from
     * @return Found name or empty if any error occured.
     */
    CcString getName(CcFontTableName *pTable);
  };

  uint16 uiFormat;                //!< Format of ???
  uint16 uiCount;                 //!< Number of records
  uint16 uiOffset;                //!< Offset in ???
  CNameRecord oNameRecords[255];  //!< Array of name records

  //! @return Get format of table already swapped
  uint16 getFormat()
  { return CcStatic::swapUint16(uiFormat); }
  //! @return Get count of name records already swapped
  uint16 getCount()
  { return CcStatic::swapUint16(uiCount); }
  //! @return Get offset in table already swapped
  uint16 getOffset()
  { return CcStatic::swapUint16(uiOffset); }
  //! @return Get string storage at uiOffset
  char* getStringStorage()
  { return CCVOIDPTRCAST(char*, this) + CcStatic::swapUint16(uiOffset); }

  /**
   * @brief Extract name by index.
   * @param iId: Target index of name queried.
   * @return Parsed string or empty if any error occured
   */
  CcString getNameById(uint16 iId);

  /**
   * @brief Extract name by offset address.
   * @param iTableNr: Target offset of name queried.
   * @return Parsed string or empty if any error occured
   */
  CcString getNameByOffset(uint16 iTableNr);

  /**
   * @brief Get all records extracted as list.
   * @return List of all name records
   */
  CcList<CNameRecord> getAllRecords();

  //! @return Get string at index 0, wich is defined as copyright
  inline CcString getCopyright()
  { return getNameById(0); }
  //! @return Get string at index 0, wich is defined as family name
  inline CcString getFamilyName()
  { return getNameById(1); }
  //! @return Get string at index 0, wich is defined as subfamily name
  inline CcString getSubFamilyName()
  { return getNameById(2); }
  //! @return Get string at index 0, wich is defined as unique id as string
  inline CcString getUniqueId()
  { return getNameById(3); }
  //! @return Get string at index 0, wich is defined as full name
  inline CcString getFullName()
  { return getNameById(4); }
  //! @return Get string at index 0, wich is defined as version as string
  inline CcString getVersion()
  { return getNameById(5); }
  //! @return Get string at index 0, wich is defined as post script name
  inline CcString getPostScriptName()
  { return getNameById(6); }
  //! @return Get string at index 0, wich is defined as trademark
  inline CcString getTrademark()
  { return getNameById(7); }
  //! @return Get string at index 0, wich is defined as manufacturer
  inline CcString getManurfacturer()
  { return getNameById(8); }
  //! @return Get string at index 0, wich is defined as designer
  inline CcString getDesigner()
  { return getNameById(9); }
  //! @return Get string at index 0, wich is defined as description
  inline CcString getDescription()
  { return getNameById(10); }
  //! @return Get string at index 0, wich is defined as vendor
  inline CcString getVendorUrl()
  { return getNameById(11); }
  //! @return Get string at index 0, wich is defined as designer
  inline CcString getDesignerUrl()
  { return getNameById(12); }
  //! @return Get string at index 0, wich is defined as license
  inline CcString getLicense()
  { return getNameById(13); }
  //! @return Get string at index 0, wich is defined as license url
  inline CcString getLicenseUrl()
  { return getNameById(14); }
  //! @return Get string at index 0, wich is defined as reservered
  inline CcString getReserved()
  { return getNameById(15); }
  //! @return Get string at index 0, wich is defined as typgraphic family
  inline CcString getTypographicFamily()
  { return getNameById(16); }
  //! @return Get string at index 0, wich is defined as typgraphic sub family
  inline CcString getTypographicSubFamily()
  { return getNameById(17); }
  // ... and so on (https://docs.microsoft.com/en-us/typography/opentype/spec/name#name-ids)
};

#pragma pack(pop)
