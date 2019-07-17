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
 * @page      Types
 * @subpage   CcUuid
 *
 * @page      CcUuid
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUuid
 **/
#ifndef _CcUuid_H_
#define _CcUuid_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"

enum class EUuidStringType
{
  BracesWithMinusSeperator = 0,
  BracesWithWithCommaSeperator,
  BraketsWithMinusSeperator,
  BraketsWithWithCommaSeperator,
  NoSeperaotrors
};

typedef struct
{
  uint32 uiTimeLow; 
  uint16 uiTimeMid; 
  uint16 uiTimeHiVersion; 
  uint8 uiClockHigh; 
  uint8 uiClockLow; 
  uint8 puiNode[6];
} SUuid;

typedef struct
{
  uint32 uiData1;
  uint16 uiData2;
  uint16 uiData3;
  uint16 uiData4;
  uint8  puiData5[6];
} SGuid;

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcUuid
{
public:
  CcUuid() = default;
  /**
   * @brief Constructor
   */
  CcUuid(uint32 uiTimeLow, uint16 uiTimeMid, uint16 uiTimeHiVersion, uint8 uiClockHigh, uint8 uiClockLow, uint8* puiNode);

  /**
   * @brief Constructor
   */
  CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint8* puiData5);

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

  bool setUuid(const CcString& sUuidString);
  CcString getUuidString(EUuidStringType eType = EUuidStringType::BracesWithMinusSeperator);

  inline CcUuid& operator=(const CcString& sUuidString)
    { setUuid(sUuidString); return *this; }
  CcUuid& operator=(const CcUuid& oToCopy);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcUuid& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcUuid& oToCompare) const
    { return !operator==(oToCompare);}
private: // types
  typedef union CcKernelSHARED
  {
    SUuid oUuid;
    SGuid oGuid;
    uint8      oArrayUint8[16];
    uint32     oArrayUint32[4];
  } UUuidIpData;
private:
  CcString getSeperatedString(const CcString& sSeperator);
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
  typedef struct CcKernelSHARED
  {
    uint32     oArrayUint32_0 = 0;
    uint32     oArrayUint32_1 = 0;
    uint32     oArrayUint32_2 = 0;
    uint32     oArrayUint32_3 = 0;
  } SUuidData;
  SUuidData m_oData;
};

#endif /* _CcUuid_H_ */
