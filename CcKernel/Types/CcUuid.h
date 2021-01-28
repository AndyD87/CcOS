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
 * @brief     Class CcUuid
 **/
#pragma once

#include "CcBase.h"
#include "CcString.h"

/**
 * @brief Type of string to generate for guid
 */
enum class EUuidStringType
{
  CurlyBracesWithMinusSeperator = 0,
  CurlyBracesWithCommaSeperator,
  BracesWithMinusSeperator,
  BracesWithCommaSeperator,
  BraketsWithMinusSeperator,
  BraketsWithCommaSeperator,
  NoSeperaotrors
};

/**
 * @brief Class implementation
 */
class CcKernelSHARED CcUuid
{
public:
  CcUuid() = default;
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcUuid)

  /**
   * @brief Constructor in RFC4112 formate
   */
  CcUuid(uint32 uiTimeLow, uint16 uiTimeMid, uint16 uiTimeHiVersion, uint8 uiClockHigh, uint8 uiClockLow, uint8* puiNode);

  /**
   * @brief Constructor in GUID with array as node
   */
  CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint8* puiData5);

  /**
   * @brief Constructor in GUID with uint64 as node
   */
  CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint64 uiNode);

  /**
   * @brief Constructor
   */
  CcUuid(uint32 uiTimeLow, uint16 uiTimeMid, uint16 uiTimeHiVersion, uint8 uiClockHigh, uint8 uiClockLow, uint8 puiNode0, uint8 puiNode1, uint8 puiNode2, uint8 puiNode3, uint8 puiNode4, uint8 puiNode5 );

  /**
   * @brief Constructor
   */
  CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint8 puiData5_0, uint8 puiData5_1, uint8 puiData5_2, uint8 puiData5_3, uint8 puiData5_4, uint8 puiData5_5);

  /**
   * @brief Constructor
   */
  CcUuid( const CcString& sUuid );

  /**
   * @brief Destructor
   */
  ~CcUuid( ) = default;

  /**
   * @brief Set uuid from string. Format should be recognized automatically
   * @param sUuidString:  String to read from
   * @return True if parsing succeeded
   */
  bool setUuid(const CcString& sUuidString);

  /**
   * @brief Generate string from this uuid. Format can be set as enum.
   * @param eType: Target type of string
   * @return Formated string
   */
  CcString getUuidString(EUuidStringType eType = EUuidStringType::CurlyBracesWithMinusSeperator) const;

  //! @copydoc getUuidString()
  CcString getString(EUuidStringType eType = EUuidStringType::CurlyBracesWithMinusSeperator) const
  { return getUuidString(eType); }

  /**
   * @brief Overwrite this uuid with uuid from string
   * @param sUuidString: String to set
   * @return Handle to this
   */
  inline CcUuid& operator=(const CcString& sUuidString)
  { setUuid(sUuidString); return *this; }

  /**
   * @brief Overwrite this uuid with content of other uuid
   * @param oToCopy: Uuid to copy from
   * @return Handle to this
   */
  CcUuid& operator=(const CcUuid& oToCopy);

  /**
   * @brief Compare two items if they are same
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcUuid& oToCompare) const;

  /**
   * @brief Compare two items if they are not same
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcUuid& oToCompare) const
  { return !operator==(oToCompare);}
public: // types
  /**
   * @brief Uuid definition from RFC4112
   */
  class CUuid
  {
  public:
    uint32 uiTimeLow;       //!< Look at https://tools.ietf.org/html/rfc4122#section-4.1
    uint16 uiTimeMid;       //!< Look at https://tools.ietf.org/html/rfc4122#section-4.1
    uint16 uiTimeHiVersion; //!< Look at https://tools.ietf.org/html/rfc4122#section-4.1
    uint8 uiClockHigh;      //!< Look at https://tools.ietf.org/html/rfc4122#section-4.1
    uint8 uiClockLow;       //!< Look at https://tools.ietf.org/html/rfc4122#section-4.1
    uint8 puiNode[6];       //!< Look at https://tools.ietf.org/html/rfc4122#section-4.1
  };

  /**
   * @brief Guid like windows is using it
   */
  class CGuid
  {
  public:
    uint32 uiData1;     //!< Look at https://en.wikipedia.org/wiki/Universally_unique_identifier
    uint16 uiData2;     //!< Look at https://en.wikipedia.org/wiki/Universally_unique_identifier
    uint16 uiData3;     //!< Look at https://en.wikipedia.org/wiki/Universally_unique_identifier
    uint16 uiData4;     //!< Look at https://en.wikipedia.org/wiki/Universally_unique_identifier
    uint8  puiData5[6]; //!< Look at https://en.wikipedia.org/wiki/Universally_unique_identifier
  };

  /**
   * @brief Union of all gui variants for fast access.
   */
  typedef union CcKernelSHARED
  {
    CUuid   oUuid;            //!< Uuid definition
    CGuid   oGuid;            //!< Guid definition
    uint8   oArrayUint8[16];  //!< Byte array
    uint32  oArrayUint32[4];  //!< uint32 array
  } UUuidIpData;
private:
  CcString getSeperatedString(const CcString& sSeperator) const;
  UUuidIpData& getData()
  {
    return *CCVOIDPTRCAST(UUuidIpData*, &m_oData);
  }
  const UUuidIpData& getData() const
  {
    return *CCVOIDPTRCONSTCAST(UUuidIpData*, &m_oData);
  }
private:
  /**
   * @brief Visual Studio 2013 does not fully implement c++11.
   *        To initialize without constructor, we are initializing a storage of Uuid size.
   */
  class CcKernelSHARED CUuidData
  {
  public:
    uint32     oArrayUint32_0 = 0;  //!< 1st 32bit part
    uint32     oArrayUint32_1 = 0;  //!< 2nd 32bit part
    uint32     oArrayUint32_2 = 0;  //!< 3rd 32bit part
    uint32     oArrayUint32_3 = 0;  //!< 4th 32bit part
  };
  CUuidData m_oData;
};
