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

CcUuid::CcUuid(uint32 uiTimeLow, uint16 uiTimeMid, uint16 uiTimeHiVersion, uint8 uiClockHigh, uint8 uiClockLow, uint8* puiNode)
{
  m_oData.oUuid.uiTimeLow = uiTimeLow;
  m_oData.oUuid.uiTimeMid = uiTimeMid;
  m_oData.oUuid.uiTimeHiVersion = uiTimeHiVersion;
  m_oData.oUuid.uiClockHigh = uiClockHigh;
  m_oData.oUuid.uiClockLow = uiClockLow;
  m_oData.oUuid.puiNode[0] = puiNode[0];
  m_oData.oUuid.puiNode[1] = puiNode[1];
  m_oData.oUuid.puiNode[2] = puiNode[2];
  m_oData.oUuid.puiNode[3] = puiNode[3];
  m_oData.oUuid.puiNode[4] = puiNode[4];
  m_oData.oUuid.puiNode[5] = puiNode[5];
}

CcUuid::CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint8* puiData5)
{
  m_oData.oGuid.uiData1 = uiData1;
  m_oData.oGuid.uiData2 = uiData2;
  m_oData.oGuid.uiData3 = uiData3;
  m_oData.oGuid.uiData4 = uiData4;
  m_oData.oGuid.puiData5[0] = puiData5[0];
  m_oData.oGuid.puiData5[1] = puiData5[1];
  m_oData.oGuid.puiData5[2] = puiData5[2];
  m_oData.oGuid.puiData5[3] = puiData5[3];
  m_oData.oGuid.puiData5[4] = puiData5[4];
  m_oData.oGuid.puiData5[5] = puiData5[5];
}

/**
 * @brief Constructor
 */
CcUuid::CcUuid(uint32 uiTimeLow, uint16 uiTimeMid, uint16 uiTimeHiVersion, uint8 uiClockHigh, uint8 uiClockLow, uint8 puiNode0, uint8 puiNode1, uint8 puiNode2, uint8 puiNode3, uint8 puiNode4, uint8 puiNode5)
{
  m_oData.oUuid.uiTimeLow = uiTimeLow;
  m_oData.oUuid.uiTimeMid = uiTimeMid;
  m_oData.oUuid.uiTimeHiVersion = uiTimeHiVersion;
  m_oData.oUuid.uiClockHigh = uiClockHigh;
  m_oData.oUuid.uiClockLow = uiClockLow;
  m_oData.oUuid.puiNode[0] = puiNode0;
  m_oData.oUuid.puiNode[1] = puiNode1;
  m_oData.oUuid.puiNode[2] = puiNode2;
  m_oData.oUuid.puiNode[3] = puiNode3;
  m_oData.oUuid.puiNode[4] = puiNode4;
  m_oData.oUuid.puiNode[5] = puiNode5;
}

/**
 * @brief Constructor
 */
CcUuid::CcUuid(uint32 uiData1, uint16 uiData2, uint16 uiData3, uint16 uiData4, uint8 puiData5_0, uint8 puiData5_1, uint8 puiData5_2, uint8 puiData5_3, uint8 puiData5_4, uint8 puiData5_5)
{
  m_oData.oGuid.uiData1 = uiData1;
  m_oData.oGuid.uiData2 = uiData2;
  m_oData.oGuid.uiData3 = uiData3;
  m_oData.oGuid.uiData4 = uiData4;
  m_oData.oGuid.puiData5[0] = puiData5_0;
  m_oData.oGuid.puiData5[1] = puiData5_1;
  m_oData.oGuid.puiData5[2] = puiData5_2;
  m_oData.oGuid.puiData5[3] = puiData5_3;
  m_oData.oGuid.puiData5[4] = puiData5_4;
  m_oData.oGuid.puiData5[5] = puiData5_5;
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
        m_oData.oGuid.uiData1 = slSplitted.at(0).toUint32(&bRet);
        if (bRet)
          m_oData.oGuid.uiData2 = slSplitted.at(1).toUint16(&bRet);
        if (bRet)
          m_oData.oGuid.uiData3 = slSplitted.at(2).toUint16(&bRet);
        if (bRet)
          m_oData.oGuid.uiData4 = slSplitted.at(3).toUint16(&bRet);
        if (bRet)
        {
          m_oData.oGuid.puiData5[0] = slSplitted.at(4).toUint8(&bRet);
          if (bRet)
            m_oData.oGuid.puiData5[1] = slSplitted.at(5).toUint8(&bRet);
          if (bRet)
            m_oData.oGuid.puiData5[2] = slSplitted.at(6).toUint8(&bRet);
          if (bRet)
            m_oData.oGuid.puiData5[3] = slSplitted.at(7).toUint8(&bRet);
          if (bRet)
            m_oData.oGuid.puiData5[4] = slSplitted.at(8).toUint8(&bRet);
          if (bRet)
            m_oData.oGuid.puiData5[5] = slSplitted.at(9).toUint8(&bRet);
        }
      }
    }
  }
  return bRet;
}

CcString CcUuid::getUuidString(EUuidStringType eType)
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
  m_oData.oArrayUint32[0] = oToCopy.m_oData.oArrayUint32[0];
  m_oData.oArrayUint32[0] = oToCopy.m_oData.oArrayUint32[1];
  m_oData.oArrayUint32[0] = oToCopy.m_oData.oArrayUint32[2];
  m_oData.oArrayUint32[0] = oToCopy.m_oData.oArrayUint32[3];
  return *this;
}

bool CcUuid::operator==(const CcUuid& oToCompare) const
{
  if (m_oData.oArrayUint32[0] == oToCompare.m_oData.oArrayUint32[0] &&
      m_oData.oArrayUint32[1] == oToCompare.m_oData.oArrayUint32[1] &&
      m_oData.oArrayUint32[2] == oToCompare.m_oData.oArrayUint32[2] &&
      m_oData.oArrayUint32[3] == oToCompare.m_oData.oArrayUint32[3])
  {
    return true;
  }
  return false;
}

CcString CcUuid::getSeperatedString(const CcString& sSeperator)
{
  CcString sRet;
  CcString sTemp;
  sTemp = CcString::fromNumber(m_oData.oGuid.uiData1, 16);
  sTemp.fillBegin("0", 8 - sTemp.length());
  sRet << sTemp << sSeperator;
  sTemp = CcString::fromNumber(m_oData.oGuid.uiData2, 16);
  sTemp.fillBegin("0", 4 - sTemp.length());
  sRet << sTemp << sSeperator;
  sTemp = CcString::fromNumber(m_oData.oGuid.uiData3, 16);
  sTemp.fillBegin("0", 4 - sTemp.length());
  sRet << sTemp << sSeperator;
  sTemp = CcString::fromNumber(m_oData.oGuid.uiData4, 16);
  sTemp.fillBegin("0", 4 - sTemp.length());
  sRet << sTemp << sSeperator;
  for (int i = 0; i < 6; i++)
  {
    sTemp = CcString::fromNumber(m_oData.oGuid.puiData5[i], 16);
    if (sTemp.length() < 2)
      sRet << "0";
    sRet << sTemp;
  }
  return sRet;
}
