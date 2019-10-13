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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcUuid
 */
#include "CcUuid.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcGlobalStrings.h"

CcUuid::CcUuid(uint32 uiTimeLow, uint16 uiTimeMid, uint16 uiTimeHiVersion, uint8 uiClockHigh, uint8 uiClockLow, uint8* puiNode)
{
  getData().oUuid.uiTimeLow = uiTimeLow;
  getData().oUuid.uiTimeMid = uiTimeMid;
  getData().oUuid.uiTimeHiVersion = uiTimeHiVersion;
  getData().oUuid.uiClockHigh = uiClockHigh;
  getData().oUuid.uiClockLow = uiClockLow;
  getData().oUuid.puiNode[0] = puiNode[0];
  getData().oUuid.puiNode[1] = puiNode[1];
  getData().oUuid.puiNode[2] = puiNode[2];
  getData().oUuid.puiNode[3] = puiNode[3];
  getData().oUuid.puiNode[4] = puiNode[4];
  getData().oUuid.puiNode[5] = puiNode[5];
}

CcUuid::CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint8* puiData5)
{
  getData().oGuid.uiData1 = uiData1;
  getData().oGuid.uiData2 = uiData2;
  getData().oGuid.uiData3 = uiData3;
  getData().oGuid.uiData4 = uiData4;
  getData().oGuid.puiData5[0] = puiData5[0];
  getData().oGuid.puiData5[1] = puiData5[1];
  getData().oGuid.puiData5[2] = puiData5[2];
  getData().oGuid.puiData5[3] = puiData5[3];
  getData().oGuid.puiData5[4] = puiData5[4];
  getData().oGuid.puiData5[5] = puiData5[5];
}

/**
 * @brief Constructor
 */
CcUuid::CcUuid(uint32 uiTimeLow, uint16 uiTimeMid, uint16 uiTimeHiVersion, uint8 uiClockHigh, uint8 uiClockLow, uint8 puiNode0, uint8 puiNode1, uint8 puiNode2, uint8 puiNode3, uint8 puiNode4, uint8 puiNode5)
{
  getData().oUuid.uiTimeLow = uiTimeLow;
  getData().oUuid.uiTimeMid = uiTimeMid;
  getData().oUuid.uiTimeHiVersion = uiTimeHiVersion;
  getData().oUuid.uiClockHigh = uiClockHigh;
  getData().oUuid.uiClockLow = uiClockLow;
  getData().oUuid.puiNode[0] = puiNode0;
  getData().oUuid.puiNode[1] = puiNode1;
  getData().oUuid.puiNode[2] = puiNode2;
  getData().oUuid.puiNode[3] = puiNode3;
  getData().oUuid.puiNode[4] = puiNode4;
  getData().oUuid.puiNode[5] = puiNode5;
}

/**
 * @brief Constructor
 */
CcUuid::CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint8 puiData5_0, uint8 puiData5_1, uint8 puiData5_2, uint8 puiData5_3, uint8 puiData5_4, uint8 puiData5_5)
{
  getData().oGuid.uiData1 = uiData1;
  getData().oGuid.uiData2 = uiData2;
  getData().oGuid.uiData3 = uiData3;
  getData().oGuid.uiData4 = uiData4;
  getData().oGuid.puiData5[0] = puiData5_0;
  getData().oGuid.puiData5[1] = puiData5_1;
  getData().oGuid.puiData5[2] = puiData5_2;
  getData().oGuid.puiData5[3] = puiData5_3;
  getData().oGuid.puiData5[4] = puiData5_4;
  getData().oGuid.puiData5[5] = puiData5_5;
}

CcUuid::CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint64 uiNode)
{
  uint8* puiNode = CCVOIDPTRCAST(uint8*, &uiNode);
  getData().oGuid.uiData1 = uiData1;
  getData().oGuid.uiData2 = uiData2;
  getData().oGuid.uiData3 = uiData3;
  getData().oGuid.uiData4 = uiData4;
  getData().oGuid.puiData5[0] = puiNode[5];
  getData().oGuid.puiData5[1] = puiNode[4];
  getData().oGuid.puiData5[2] = puiNode[3];
  getData().oGuid.puiData5[3] = puiNode[2];
  getData().oGuid.puiData5[4] = puiNode[1];
  getData().oGuid.puiData5[5] = puiNode[0];
}

CcUuid::CcUuid(const CcString& sUuid)
{
  setUuid(sUuid);
}

bool CcUuid::setUuid(const CcString& sUuidString)
{
  bool bRet = false;
  CcString sReplace = sUuidString.getReplace("{", "").replace("}", "").replace("0x", "");
  CcStringList slSplitted = sReplace.split('-');
  if (slSplitted.size() == 5)
  {
    slSplitted.at(4).trim();
    if (slSplitted.at(4).length() == 12)
    {
      slSplitted.append(slSplitted.at(4).substr(0, 2));
      slSplitted.append(slSplitted.at(4).substr(2, 2));
      slSplitted.append(slSplitted.at(4).substr(4, 2));
      slSplitted.append(slSplitted.at(4).substr(6, 2));
      slSplitted.append(slSplitted.at(4).substr(8, 2));
      slSplitted.append(slSplitted.at(4).substr(10, 2));
      slSplitted.remove(4);
      for (CcString& sArray : slSplitted)
      {
        sArray.trim();
        sArray = "0x" + sArray;
      }
      if (slSplitted.at(0).length() == 10 &&
          slSplitted.at(1).length() == 6 &&
          slSplitted.at(2).length() == 6 &&
          slSplitted.at(3).length() == 6)
      {
        getData().oGuid.uiData1 = slSplitted.at(0).toUint32(&bRet);
        if (bRet)
          getData().oGuid.uiData2 = slSplitted.at(1).toUint16(&bRet);
        if (bRet)
          getData().oGuid.uiData3 = slSplitted.at(2).toUint16(&bRet);
        if (bRet)
          getData().oGuid.uiData4 = slSplitted.at(3).toUint16(&bRet);
        if (bRet)
        {
          getData().oGuid.puiData5[0] = slSplitted.at(4).toUint8(&bRet);
          if (bRet)
            getData().oGuid.puiData5[1] = slSplitted.at(5).toUint8(&bRet);
          if (bRet)
            getData().oGuid.puiData5[2] = slSplitted.at(6).toUint8(&bRet);
          if (bRet)
            getData().oGuid.puiData5[3] = slSplitted.at(7).toUint8(&bRet);
          if (bRet)
            getData().oGuid.puiData5[4] = slSplitted.at(8).toUint8(&bRet);
          if (bRet)
            getData().oGuid.puiData5[5] = slSplitted.at(9).toUint8(&bRet);
        }
      }
    }
  }
  return bRet;
}

CcString CcUuid::getUuidString(EUuidStringType eType) const
{
  CcString sUuid;
  switch (eType)
  {
    case EUuidStringType::BracesWithMinusSeperator:
      sUuid << "{";
      sUuid << getSeperatedString("-");
      sUuid << "}";
      break;
    case EUuidStringType::BracesWithWithCommaSeperator:
      sUuid << "{";
      sUuid << getSeperatedString(",");
      sUuid << "}";
      break;
    case EUuidStringType::BraketsWithMinusSeperator:
      sUuid << "(";
      sUuid << getSeperatedString("-");
      sUuid << ")";
      break;
    case EUuidStringType::BraketsWithWithCommaSeperator:
      sUuid << "(";
      sUuid << getSeperatedString(",");
      sUuid << ")";
      break;
    case EUuidStringType::NoSeperaotrors:
      sUuid = getSeperatedString("");
      break;
    default:
      break;
  }
  return sUuid;
}

CcUuid& CcUuid::operator=(const CcUuid& oToCopy)
{
  getData().oArrayUint32[0] = oToCopy.getData().oArrayUint32[0];
  getData().oArrayUint32[0] = oToCopy.getData().oArrayUint32[1];
  getData().oArrayUint32[0] = oToCopy.getData().oArrayUint32[2];
  getData().oArrayUint32[0] = oToCopy.getData().oArrayUint32[3];
  return *this;
}

bool CcUuid::operator==(const CcUuid& oToCompare) const
{
  if (getData().oArrayUint32[0] == oToCompare.getData().oArrayUint32[0] &&
      getData().oArrayUint32[1] == oToCompare.getData().oArrayUint32[1] &&
      getData().oArrayUint32[2] == oToCompare.getData().oArrayUint32[2] &&
      getData().oArrayUint32[3] == oToCompare.getData().oArrayUint32[3])
  {
    return true;
  }
  return false;
}

CcString CcUuid::getSeperatedString(const CcString& sSeperator) const
{
  CcString sRet;
  CcString sTemp;
  sTemp = CcString::fromNumber(getData().oGuid.uiData1, 16);
  sTemp.fillBegin(CcGlobalStrings::Numbers::i0, 8 - sTemp.length());
  sRet << sTemp << sSeperator;
  sTemp = CcString::fromNumber(getData().oGuid.uiData2, 16);
  sTemp.fillBegin(CcGlobalStrings::Numbers::i0, 4 - sTemp.length());
  sRet << sTemp << sSeperator;
  sTemp = CcString::fromNumber(getData().oGuid.uiData3, 16);
  sTemp.fillBegin(CcGlobalStrings::Numbers::i0, 4 - sTemp.length());
  sRet << sTemp << sSeperator;
  sTemp = CcString::fromNumber(getData().oGuid.uiData4, 16);
  sTemp.fillBegin(CcGlobalStrings::Numbers::i0, 4 - sTemp.length());
  sRet << sTemp << sSeperator;
  for (int i = 0; i < 6; i++)
  {
    sTemp = CcString::fromNumber(getData().oGuid.puiData5[i], 16);
    if (sTemp.length() < 2)
      sRet << CcGlobalStrings::Numbers::i0;
    sRet << sTemp;
  }
  return sRet;
}
