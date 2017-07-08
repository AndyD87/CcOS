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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcJsonDocument
 */
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonData.h"

const CcString CcJsonDocument::c_sIndent("  ");

CcJsonDocument::CcJsonDocument( void )
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


CcJsonDocument::~CcJsonDocument( void )
{
}

bool CcJsonDocument::parseDocument(const CcString& sDocument)
{
  size_t uiPos = findBeginning(sDocument);
  if (uiPos != SIZE_MAX)
  {
    switch (sDocument[uiPos])
    {
      case '{':
        m_oJsonData.setJsonObject();
        parseMap(m_oJsonData, sDocument, uiPos);
        break;
      case '[':
        m_oJsonData.setJsonArray();
        parseArray(m_oJsonData, sDocument, uiPos);
        break;
    }
  }
  else
  {
    m_oJsonData.setJsonObject();
  }
  return !m_bParseError;
}

CcString& CcJsonDocument::getJsonDocument( bool bCompact)
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

size_t CcJsonDocument::findBeginning(const CcString& sDocument)
{
  size_t uiPosCrlBrack = sDocument.find('{');
  size_t uiPosSqrBrack = sDocument.find('[');
  if (uiPosCrlBrack > uiPosSqrBrack)
    return uiPosSqrBrack;
  else
    return uiPosCrlBrack;
}

size_t CcJsonDocument::findNextEnding(const CcString& sDocument, size_t uiOffset)
{
  size_t uiPosCrlBrack = sDocument.find('}', uiOffset);
  size_t uiPosSqrBrack = sDocument.find(']', uiOffset);
  size_t uiPosComma = sDocument.find(',', uiOffset);
  if (uiPosCrlBrack < uiPosSqrBrack)
  {
    if (uiPosComma < uiPosCrlBrack)
      return uiPosComma;
    else
      return uiPosCrlBrack;
  }
  else
  {
    if (uiPosComma < uiPosSqrBrack)
      return uiPosComma;
    else
      return uiPosSqrBrack;
  }
}

size_t CcJsonDocument::parseMap(CcJsonData& oMap, const CcString& sDocument, size_t uiOffset)
{
  if (sDocument[uiOffset] == '{')
  {
    oMap.setJsonObject();
    do
    {
      CcJsonData oItem;
      uiOffset++;
      uiOffset = sDocument.posNextNotWhitespace(uiOffset);
      if (uiOffset < sDocument.length() && sDocument[uiOffset] == '"')
      {
        CcString sName = sDocument.getStringBetween('"', '"', uiOffset);
        oItem.setName(sName);
        uiOffset += sName.length();
        if (uiOffset < sDocument.length())
        {
          uiOffset += 1 + 1; // sizeof("")
          uiOffset = sDocument.posNextNotWhitespace(uiOffset);
          if (uiOffset < sDocument.length() && sDocument[uiOffset] == ':')
          {
            uiOffset++;
            uiOffset = sDocument.posNextNotWhitespace(uiOffset);
            uiOffset = parseValue(oItem, sDocument, uiOffset);
          }
          oMap.object().add(oItem);
        }
        else
        {
          m_bParseError = true;
          m_sParseErrorMsg = "Error";
        }
      }
      else if (uiOffset < sDocument.length() && sDocument[uiOffset] == '}')
      {
        break;;
      }
      else
      {
        m_bParseError = true;
        m_sParseErrorMsg = "Error";
      }
    } while (uiOffset < sDocument.length() && sDocument[uiOffset] == ',');
    uiOffset = sDocument.posNextNotWhitespace(uiOffset);
    if (uiOffset < sDocument.length() && sDocument[uiOffset] == '}')
    {
      uiOffset++;
      uiOffset = sDocument.posNextNotWhitespace(uiOffset);
    }
    else
    {
      m_bParseError = true;
      m_sParseErrorMsg = "Error";
    }
  }
  return uiOffset;
}

size_t CcJsonDocument::parseArray(CcJsonData& oArray, const CcString& sDocument, size_t uiOffset)
{
  if (sDocument[uiOffset] == '[')
  {
    oArray.setJsonArray();
    do
    {
      CcJsonData oItem;
      uiOffset++;
      uiOffset = sDocument.posNextNotWhitespace(uiOffset);
      if (uiOffset < sDocument.length())
      {
        if (sDocument[uiOffset] != ']')
        {
          uiOffset = parseValue(oItem, sDocument, uiOffset);
          oArray.array().append(oItem);
        }
      }
      else
      {
        m_bParseError = true;
        m_sParseErrorMsg = "Error";
      }
    } while (uiOffset < sDocument.length() && sDocument[uiOffset] == ',');
    uiOffset = sDocument.posNextNotWhitespace(uiOffset);
    if (uiOffset < sDocument.length() && sDocument[uiOffset] == ']')
    {
      uiOffset++;
      uiOffset = sDocument.posNextNotWhitespace(uiOffset);
    }
    else
    {
      m_bParseError = true;
      m_sParseErrorMsg = "Error";
    }
  }
  return uiOffset;
}

size_t CcJsonDocument::parseValue(CcJsonData& oItem, const CcString& sDocument, size_t uiOffset)
{
  if (uiOffset < sDocument.length())
  {
    if (sDocument[uiOffset] == '{') // object
    {
      uiOffset = parseMap(oItem, sDocument, uiOffset);
    }
    else if (sDocument[uiOffset] == '[') // array
    {
      uiOffset = parseArray(oItem, sDocument, uiOffset);
    }
    else if (sDocument[uiOffset] == '"') // string
    {
      uint64 uiPosNextQuote= uiOffset;
      do
      {
        uiPosNextQuote++;
        uiPosNextQuote = sDocument.find("\"", uiPosNextQuote);
      } while (uiPosNextQuote != SIZE_MAX && sDocument[uiPosNextQuote - 1] == '\\');
      uiOffset++;
      if (uiPosNextQuote != SIZE_MAX)
      {
        CcString sValue = sDocument.substr(uiOffset, uiPosNextQuote - uiOffset).replace("\\\"", "\"");
        oItem.setValue(sValue);
        uiOffset = uiPosNextQuote + 1;
      }
      else
      {
        uiOffset = SIZE_MAX;
      }
    }
    else if (sDocument.startWith("true", uiOffset)) // bool
    {
      oItem.setValue(true);
      uiOffset += 4;
    }
    else if (sDocument.startWith("false", uiOffset)) // bool
    {
      oItem.setValue(false);
      uiOffset += 5;
    }
    else if (sDocument.startWith("null", uiOffset)) // ???
    {
      oItem.setValue();
      uiOffset += 4;
    }
    else // number
    {
      size_t uiPosEnd = findNextEnding(sDocument, uiOffset);
      if (uiPosEnd < sDocument.length())
      {
        CcString sSubStr = sDocument.substr(uiOffset, uiPosEnd - uiOffset).trim();
        bool bSuccess;
        if (sSubStr.find('.') != SIZE_MAX)
        {
          double dFromString = sSubStr.toDouble(&bSuccess);
          if (!bSuccess)
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
          int64 iFromString = sSubStr.toInt64(&bSuccess);
          if (!bSuccess)
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
        oItem.setValue();
      }
      uiOffset = uiPosEnd;
    }
  }
  else
  {
    m_bParseError = true;
    m_sParseErrorMsg = "Error";
  }
  return uiOffset;
}

void CcJsonDocument::writeMap(CcString &sOut, const CcJsonData& oItem)
{
  writeIntends(sOut);
  m_uiIntendLevel++;
  const CcJsonObject& pMap = oItem.getJsonObject();
  if (oItem.getName().length() > 0)
  {
    sOut << "\"" << oItem.getName() << "\":";
  }
  sOut << "{";
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
  sOut << "}";
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
    sOut << "\"" << oItem.getName() << "\":";
  }
  sOut << "[";
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
  sOut << "]";
  m_uiIntendLevel--;
  writeNewLine(sOut);
}

void CcJsonDocument::writeValue(CcString &sOut, const CcJsonData& oItem)
{
  if (oItem.getType() != EJsonDataType::Unknown)
  {
    if (oItem.getName().length() > 0)
    {
      sOut << "\"" + oItem.getName() << "\":";
    }
    const CcVariant& pVariant = oItem.getValue();
    if (pVariant.isBool())
    {
      if (pVariant.getBool())
        sOut << "true";
      else
        sOut << "false";
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
      sOut << "\"";
      sOut << pVariant.getString().replace("\"", "\\\"");
      sOut << "\"";
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
    sOut << "\r\n";
  }
}
