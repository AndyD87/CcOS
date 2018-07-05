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
 * @brief     Implemtation of class CcJsonDocument
 */
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonData.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"

const CcString CcJsonDocument::c_sIndent("  ");

CcJsonDocument::CcJsonDocument(void )
{
}

CcJsonDocument::CcJsonDocument(const CcString& sDocument)
{
  parseDocument(sDocument);
}

CcJsonDocument::CcJsonDocument(const CcJsonData& oValue)
{
  m_oJsonData = oValue;
}

CcJsonDocument::CcJsonDocument(const CcJsonObject& oJsonObject)
{
  m_oJsonData.setJsonObject(oJsonObject);
}

CcJsonDocument::CcJsonDocument(const CcJsonArray& oJsonArray)
{
  m_oJsonData.setJsonArray(oJsonArray);
}


CcJsonDocument::~CcJsonDocument(void )
{
}

bool CcJsonDocument::operator==(const CcJsonDocument& oToCompare) const
{
  return m_oJsonData == oToCompare.m_oJsonData;
}

bool CcJsonDocument::parseDocument(const CcString& sDocument)
{
  m_oJsonData.reset();
  m_sDocument = sDocument;
  size_t uiPos = findBeginning(sDocument);
  if (uiPos != SIZE_MAX)
  {
    size_t uiLength = sDocument.length() - uiPos;
    const char* pcDoc = sDocument.getCharString() + uiPos;
    switch (sDocument[uiPos])
    {
      case '{':
      {
        m_oJsonData.setJsonObject();
        parseMap(m_oJsonData, pcDoc, uiLength);
        break;
      }
      case '[':
        m_oJsonData.setJsonArray();
        parseArray(m_oJsonData, pcDoc, uiLength);
        break;
    }
  }
  else
  {
    m_oJsonData.setJsonObject();
  }
  return !m_bParseError;
}

CcString& CcJsonDocument::getDocument( bool bCompact)
{
  m_sDocument.clear();
  m_bIntend = !bCompact;
  switch (m_oJsonData.getType())
  {
    case EJsonDataType::Object:
      writeMap(m_sDocument, m_oJsonData);
      break;
    case EJsonDataType::Array:
      writeArray(m_sDocument, m_oJsonData);
      break;
    case EJsonDataType::Value:
    case EJsonDataType::Unknown:
    default:
      break;
  }
  return m_sDocument;
}

bool CcJsonDocument::isValidData(const CcString& sData)
{
  CcJsonDocument oDocument;
  return oDocument.parseDocument(sData);
}

size_t CcJsonDocument::findBeginning(const CcString& sDocument)
{
  size_t uiPosCrlBrack = sDocument.find(CcGlobalStrings::Brackets::CurlyLeft);
  size_t uiPosSqrBrack = sDocument.find(CcGlobalStrings::Brackets::SquareLeft);
  if (uiPosCrlBrack > uiPosSqrBrack)
    return uiPosSqrBrack;
  else
    return uiPosCrlBrack;
}

size_t CcJsonDocument::findNextEnding(const char* sDocument, size_t uiLength)
{
  char cFound;
  return CcStringUtil::findCharOf(sDocument, uiLength, ",}]", 3, cFound);
}

bool CcJsonDocument::parseMap(CcJsonData& oMap, const char*& sDocument, size_t& uiLength)
{
  bool bRet = true;
  size_t uiNextNotWS = 0;
  if (*sDocument == '{')
  {
    CcJsonData oItem;
    int iLoops = 0;
    oMap.setJsonObject();
    do
    {
      iLoops++;
      uiLength--;
      sDocument++;
      uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
      if (uiNextNotWS < uiLength)
      {
        uiLength -= uiNextNotWS;
        sDocument += uiNextNotWS;
        if (uiLength && *sDocument == '"')
        {
          uiLength--;
          sDocument++;
          uiNextNotWS = CcStringUtil::findChar(sDocument, uiLength, CcGlobalStrings::Seperators::Quote[0]);
          if (uiNextNotWS < uiLength)
          {
            oItem.name().set(sDocument, uiNextNotWS);
          }
          uiLength -= uiNextNotWS + 1;
          sDocument += uiNextNotWS + 1;
          if (uiLength)
          {
            uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
            if (uiNextNotWS < uiLength)
            {
              uiLength -= uiNextNotWS;
              sDocument += uiNextNotWS;
              if (*sDocument == ':')
              {
                uiLength--;
                sDocument++;
                uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
                if (uiNextNotWS < uiLength)
                {
                  uiLength -= uiNextNotWS;
                  sDocument += uiNextNotWS;
                  parseValue(oItem, sDocument, uiLength);
                }
              }
            }
            oMap.object().append(std::move(oItem));
          }
          else
          {
            m_bParseError = true;
            m_sParseErrorMsg = "Error";
          }
        }
        else if (uiLength && *sDocument == '}')
        {
          uiLength--;
          sDocument++;
          break;
        }
        else
        {
          m_bParseError = true;
          m_sParseErrorMsg = "Error";
        }
      }
      else
      {
        bRet = false;
      }
    } while (uiLength && *sDocument == ',');
    uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
    if (uiNextNotWS < uiLength && sDocument[uiNextNotWS] == '}')
    {
      uiLength -= uiNextNotWS + 1;
      sDocument += uiNextNotWS + 1;
      uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
      if (uiNextNotWS < uiLength)
      {
        uiLength -= uiNextNotWS;
        sDocument += uiNextNotWS;
      }
    }
    else
    {
      m_bParseError = true;
      m_sParseErrorMsg = "Error";
    }
  }
  return bRet;
}

bool CcJsonDocument::parseArray(CcJsonData& oArray, const char*& sDocument, size_t& uiLength)
{
  bool bRet = true;
  size_t uiNextNotWS = 0;
  if (*sDocument == '[')
  {
    oArray.setJsonArray();
    do
    {
      CcJsonData oItem;
      sDocument++;
      uiLength--;
      uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
      if (uiNextNotWS < uiLength)
      {
        uiLength -= uiNextNotWS;
        sDocument += uiNextNotWS;
        if (*sDocument != ']')
        {
          parseValue(oItem, sDocument, uiLength);
          oArray.array().append(std::move(oItem));
        }
      }
      else
      {
        bRet = false;
      }
    } while (bRet && uiLength && *sDocument == ',');

    uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
    if (uiNextNotWS < uiLength)
    {
      uiLength -= uiNextNotWS;
      sDocument += uiNextNotWS;
      if (*sDocument == ']')
      {
        uiLength--;
        sDocument++;
        uiNextNotWS = CcStringUtil::findNextNotWhiteSpace(sDocument, uiLength);
        if (uiNextNotWS < uiLength)
        {
          uiLength -= uiNextNotWS;
          sDocument += uiNextNotWS;
        }
      }
      else
      {
        m_bParseError = true;
        m_sParseErrorMsg = "Error";
      }
    }
  }
  return bRet;
}

bool CcJsonDocument::parseValue(CcJsonData& oItem, const char*& sDocument, size_t& uiLength)
{
  bool bRet = true;
  if (*sDocument == '{') // object
  {
    parseMap(oItem, sDocument, uiLength);
  }
  else if (*sDocument == '[') // array
  {
    parseArray(oItem, sDocument, uiLength);
  }
  else if (*sDocument == '"') // string
  {
    uiLength--;
    sDocument++;
    size_t uiPosNextQuote = 0;
    uiPosNextQuote = CcStringUtil::findChar(sDocument + uiPosNextQuote, uiLength, CcGlobalStrings::Seperators::Quote[0], CcGlobalStrings::Seperators::BackSlash[0]);
    if (uiPosNextQuote != SIZE_MAX)
    {
      oItem.setValue(CcString(sDocument, uiPosNextQuote));
      uiLength -= uiPosNextQuote + 1;
      sDocument += uiPosNextQuote + 1;
    }
    else
    {
      bRet = false;
    }
  }
  else if (CcStringUtil::strcmp(sDocument, CcGlobalStrings::True.getCharString(), CcGlobalStrings::True.length()) == 0) // bool
  {
    oItem.setValue(true);
    uiLength -= 4;
    sDocument += 4;
  }
  else if (CcStringUtil::strcmp(sDocument, CcGlobalStrings::False.getCharString(), CcGlobalStrings::False.length()) == 0) // bool
  {
    oItem.setValue(false);
    uiLength -= 5;
    sDocument += 5;
  }
  else if (CcStringUtil::strcmp(sDocument, CcGlobalStrings::Null.getCharString(), CcGlobalStrings::Null.length()) == 0) // ???
  {
    oItem.setValue();
    uiLength -= 4;
    sDocument += 4;
  }
  else
  {
    size_t uiPosEnd = findNextEnding(sDocument, uiLength);
    if (uiPosEnd < uiLength)
    {
      CcString sSubStr(sDocument, uiPosEnd);
      uiLength -= uiPosEnd;
      sDocument += uiPosEnd;
      if (sSubStr.find(CcGlobalStrings::Seperators::Dot) != SIZE_MAX)
      {
        double dFromString = sSubStr.toDouble(&bRet);
        if (!bRet)
        {
          oItem.setValue();
          m_bParseError = true;
          m_sParseErrorMsg = "Error";
        }
        else
        {
          oItem.setValue(dFromString);
        }
      }
      else
      {
        int64 iFromString = sSubStr.toInt64(&bRet);
        if (!bRet)
        {
          oItem.setValue();
          m_bParseError = true;
          m_sParseErrorMsg = "Error";
        }
        else
        {
          oItem.setValue(iFromString);
        }
      }
    }
    else
    {
      bRet = false;
    }
  }
  return bRet;
}

void CcJsonDocument::writeMap(CcString &sOut, const CcJsonData& oItem)
{
  writeIntends(sOut);
  m_uiIntendLevel++;
  const CcJsonObject& pMap = oItem.getJsonObject();
  if (oItem.getName().length() > 0)
  {
    sOut << CcGlobalStrings::Seperators::Quote << oItem.getName() << "\":";
  }
  sOut << CcGlobalStrings::Brackets::CurlyLeft;
  bool bFirstItem = true;
  for (const CcJsonData& rValue : pMap)
  {
    if (bFirstItem == false)
      sOut << ",";
    else
      bFirstItem = false;
    switch (rValue.getType())
    {
      case EJsonDataType::Array:
        writeArray(sOut, rValue);
        break;
      case EJsonDataType::Object:
        writeMap(sOut, rValue);
        break;
      case EJsonDataType::Value:
        writeValue(sOut, rValue);
        break;
      case EJsonDataType::Unknown:
      default:
        break;
    }
  }
  sOut << CcGlobalStrings::Brackets::CurlyRight;
  m_uiIntendLevel--;
  writeNewLine(sOut);
}

void CcJsonDocument::writeArray(CcString &sOut, const CcJsonData& oItem)
{
  writeIntends(sOut);
  m_uiIntendLevel++;
  const CcJsonArray& pArray = oItem.getJsonArray();
  if (oItem.getName().length() > 0)
  {
    sOut << CcGlobalStrings::Seperators::Quote << oItem.getName() << "\":";
  }
  sOut << CcGlobalStrings::Brackets::SquareLeft;
  bool bFirstItem = true;
  for (CcJsonData& rValue : pArray)
  {
    if (bFirstItem == false)
      sOut << ",";
    else
      bFirstItem = false;
    switch (rValue.getType())
    {
      case EJsonDataType::Array:
        writeArray(sOut, rValue);
        break;
      case EJsonDataType::Object:
        writeMap(sOut, rValue);
        break;
      case EJsonDataType::Value:
        writeValue(sOut, rValue);
        break;
      case EJsonDataType::Unknown:
      default:
        break;
    }
  }
  sOut << CcGlobalStrings::Brackets::SquareRight;
  m_uiIntendLevel--;
  writeNewLine(sOut);
}

void CcJsonDocument::writeValue(CcString &sOut, const CcJsonData& oItem)
{
  if (oItem.getType() != EJsonDataType::Unknown)
  {
    if (oItem.getName().length() > 0)
    {
      sOut << CcGlobalStrings::Seperators::Quote + oItem.getName() << "\":";
    }
    const CcVariant& pVariant = oItem.getValue();
    if (pVariant.isBool())
    {
      if (pVariant.getBool())
        sOut << CcGlobalStrings::True;
      else
        sOut << CcGlobalStrings::False;
    }
    else if (pVariant.isInt())
    {
      sOut << pVariant.getString();
    }
    else if (pVariant.isUint())
    {
      sOut << pVariant.getString();
    }
    else if (pVariant.isFloat())
    {
      CcString sFloatString = pVariant.getString();
      // remove trailing 0
      while ( sFloatString.length() > 2 && 
              sFloatString[sFloatString.length() - 1] == '0' &&
              sFloatString[sFloatString.length() - 2] != '.')
         sFloatString.remove(sFloatString.length() - 1);

      sOut << sFloatString;
    }
    else if (pVariant.isString())
    {
      sOut << CcGlobalStrings::Seperators::Quote;
      sOut << pVariant.getString();// .replace(CcGlobalStrings::Seperators::Quote, "\\\"");
      sOut << CcGlobalStrings::Seperators::Quote;
    }
    else
    {
      sOut << "null";
    }
  }
  writeNewLine(sOut);
}

void CcJsonDocument::writeIntends(CcString &sOut) const
{
  if (m_bIntend)
  {
    for (uint16 i = 0; i < m_uiIntendLevel; i++)
    {
      sOut << c_sIndent;
    }
  }
}

void CcJsonDocument::writeNewLine(CcString &sOut) const
{
  if (m_bIntend)
  {
    sOut << CcGlobalStrings::EolLong;
  }
}
