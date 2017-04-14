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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief      Implementation of class CcString
 */
#include "CcString.h"
#include "CcByteArray.h"
#include "CcStringUtil.h"
#include "CcStringList.h"
#include "stdlib.h"
#include "stdio.h"
#include <locale>
#include <clocale>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdarg>
#include <string>
#ifdef WIN32
#endif

#ifndef WIN32
  #define TO_STRING_DEPRECATED
#endif

bool CcString::s_bIsUtf8 = CcString::utf8("en_US.UTF8");

bool CcString::utf8(const char* locale)
{
  std::setlocale(LC_ALL, locale);
  return true;
}

CcString::CcString()
{
  m_String = new std::string; 
  //CCMONITORNEW(m_String);
}

CcString::CcString(const CcString& rhs)
{
  m_String = new std::string(*rhs.m_String); 
  //CCMONITORNEW(m_String);
}

CcString::CcString(size_t uiLength, const char cDefaultChar)
{
  m_String = new std::string(uiLength, cDefaultChar);
  //CCMONITORNEW(m_String);
}

CcString::CcString(const char* cString)
{
  if (cString != nullptr)
  {
    m_String = new std::string(cString); 
    //CCMONITORNEW(m_String);
  }
  else
  {
    m_String = new std::string(); 
    //CCMONITORNEW(m_String);
  }
}

CcString::CcString(const std::string& stdString)
{
  m_String = new std::string(stdString); 
  //CCMONITORNEW(m_String);
}

CcString::CcString(const char* cString, size_t uiLength)
{
  m_String = new std::string(cString, uiLength); 
  //CCMONITORNEW(m_String);
}

CcString::CcString(const char cChar)
{
  m_String = new std::string; 
  //CCMONITORNEW(m_String);
  append(cChar);
}

CcString::CcString(const CcByteArray& baString)
{
  m_String = new std::string; 
  //CCMONITORNEW(m_String);
  append(baString);
}

CcString::~CcString() 
{
  if (m_String != nullptr)
  {
    //CCMONITORDELETE(m_String); 
    delete m_String;
  }
}


const std::string& CcString::getStdString(void) const
{
  return *m_String;
}

std::wstring CcString::getStdWString(void) const
{
  const std::ctype<wchar_t>& cType = std::use_facet<std::ctype<wchar_t> >(std::locale());
  std::vector<wchar_t> wideStringBuffer(getStdString().length());
  cType.widen(m_String->data(), getStdString().data() + getStdString().length(), &wideStringBuffer[0]);
  return std::wstring(&wideStringBuffer[0], wideStringBuffer.size());
}

CcString& CcString::format(const char* sFormat, ...)
{
  char cString[1024];
  va_list argptr;
  va_start(argptr, sFormat);
#ifndef WIN32
  ::sprintf(cString, sFormat, argptr);
#else
  sprintf_s(cString, sFormat, argptr);
#endif
  va_end(argptr);
  set(cString);
  return *this;
}

CcString& CcString::remove(size_t uiPos, size_t uiLength)
{
  CcString sTemp = m_String->substr(0, uiPos);
  sTemp.append(m_String->substr(uiPos + uiLength));
  *this = sTemp;
  return *this;
}

CcString CcString::substr(size_t pos, size_t len) const
{
  CcString sRet;
  if (pos < length())
  {
    if (len == SIZE_MAX)
      sRet = (*m_String).substr(pos);
    else if (len + pos < length())
      sRet = (*m_String).substr(pos, len);
    else
      sRet = (*m_String).substr(pos, std::string::npos);
  }
  return sRet;
}

CcString CcString::replace(const CcString& needle, const CcString& replace) const
{
  size_t pos=0;
  CcString sRet(*this);
  while (pos < sRet.length())
  {
    pos = sRet.find(needle, pos);
    if (pos != SIZE_MAX)
    {
      sRet.erase(pos, needle.length());
      sRet.insert(pos, replace);
      pos += replace.length();
    }
  }
  return sRet;
}

CcString CcString::getStringBetween(const CcString& preStr, const CcString& postStr, size_t offset, size_t *pos) const
{
  CcString sRet;
  size_t posFirst = (*m_String).find_first_of(preStr.getStdString(), offset) ;
  if (posFirst != std::string::npos)
  {
    posFirst += preStr.length();
    size_t posSecond = (*m_String).find_first_of(postStr.getStdString(), posFirst);
    if (posSecond != std::string::npos)
    {
      size_t len = posSecond - posFirst;
      std::string temp = (*m_String).substr(posFirst, len);
      sRet.append(temp);
      if (pos != 0)
        *pos = posFirst + preStr.length();
    }
    else
    {
      sRet = substr(posFirst);
    }
  }
  return sRet;
}

CcString CcString::getLastLine(void) const
{
  size_t posR = findLast("\r");
  size_t posN = findLast("\n");
  if (posR > posN)
    return substr(posR);
  else if (posN > posR)
    return substr(posN);
  else
    return *this;
}

bool CcString::compare(const CcString& sToCompare, ESensitivity bSensitivity) const
{
  if(bSensitivity==ESensitivity::CaseSensitiv)
  {
    if((*m_String) == (*sToCompare.m_String))
      return true;
    return false;
  }
  else
  {
    CcString cs1(*this);
    cs1.toLower();
    CcString cs2(sToCompare);
    cs2.toLower();
    return cs1.compare(cs2, ESensitivity::CaseSensitiv);
  }
}

size_t CcString::findLast(const CcString& sToFind) const
{
  size_t iRet = SIZE_MAX;
  size_t temp=0;
  while ((temp = find(sToFind, temp)) != SIZE_MAX)
  {
    iRet = temp;
    temp++;
  }
  return iRet;
}

size_t CcString::find(const CcString& sToFind, size_t offset) const
{
  return (*m_String).find(sToFind.getStdString(), offset);
}

bool CcString::startWith(const CcString& sToCompare, size_t offset) const
{
  if (sToCompare.length() > (*m_String).length())
  {
    return false;
  }
  else{
    for (size_t i = 0; i < sToCompare.length(); i++)
    {
      if (sToCompare.at(i) != at(i + offset))
        return false;
    }
  }
  return true;
}

bool CcString::endWith(const CcString& sToCompare, size_t offset) const
{
  if (sToCompare.length() > (*m_String).length())
  {
    return false;
  }
  else
  {
    for (size_t i = 0; i < sToCompare.length(); i++)
    {
      if (sToCompare.at(i) != at(i + offset))
        return false;
    }
  }
  return true;
}

uint64 CcString::toUint64( bool *bOk) const
{
  uint64 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = std::stoull(*m_String, &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

uint32 CcString::toUint32( bool *bOk) const
{
  uint32 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = std::stoul(*m_String, &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

uint16 CcString::toUint16(bool *bOk) const
{
  uint16 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = 0xffff & std::stoul(*m_String, &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

uint8 CcString::toUint8(bool *bOk) const
{
  uint8 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = 0xff & std::stoul(*m_String, &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

int64 CcString::toInt64( bool *bOk)const
{
  int64 iRet = 0;
  try
  {
    iRet = std::stoll(*m_String);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

int32 CcString::toInt32( bool *bOk)const
{
  int32 iRet = 0;
  try
  {
    iRet = std::stol(*m_String);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

int16 CcString::toInt16(bool *bOk)const
{
  int16 iRet = 0;
  try
  {
    iRet = 0x0000ffff & std::stoi(*m_String);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

int8 CcString::toInt8(bool *bOk) const
{
  int8 iRet = 0;
  try
  {
    iRet = 0x000000ff & std::stoi(*m_String);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

float CcString::toFloat(bool* bOk) const
{
  float fRet = 0;
  try
  {
    fRet = std::stof(*m_String);
    if (bOk != 0)
      *bOk = true;
  }
  catch(...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return fRet;
}

double CcString::toDouble(bool* bOk) const
{
  double fRet = 0;
  try
  {
    fRet = std::stod(*m_String);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return fRet;
}

CcString& CcString::toUpper(void)
{
  std::transform(m_String->begin(), m_String->end(), m_String->begin(), ::toupper);
  return *this;
}

CcString& CcString::toLower(void)
{
  std::transform(m_String->begin(), m_String->end(), m_String->begin(), ::tolower);
  return *this;
}

CcString& CcString::appendNumber(uint8 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << (uint) number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string((uint) number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(int8 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << (int) number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string((int) number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(uint16 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(int16 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(uint32 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(int32 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(uint64 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(int64 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

#ifdef WIN32
CcString& CcString::appendNumber(uint number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}

CcString& CcString::appendNumber(int number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  (*m_String).append(sTemp);
  return *this;
}
#endif

CcString& CcString::appendNumber(double number)
{
  std::ostringstream os;
  os << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << number;
  (*m_String).append(os.str());
  return *this;
}

CcString& CcString::appendNumber(float number)
{
  std::ostringstream os;
  os << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << number;
  (*m_String).append(os.str());
  return *this;
}

size_t CcString::length( void ) const
{
  return (*m_String).length();
}

size_t CcString::posNextNotWhitespace(size_t offset) const
{
  size_t uiRet = SIZE_MAX;
  for (; offset < length() && uiRet == SIZE_MAX; offset++)
  {
    switch (at(offset))
    {
      case ' ':
        continue;
      case '\t':
        continue;
      case '\n':
        continue;
      case '\r':
        continue;
      case '\f':
        continue;
      case '\v':
        continue;
      default:
        uiRet = offset;
    }
  }
  return uiRet;
}

size_t CcString::posNextWhitespace(size_t offset) const
{
  size_t uiRet = SIZE_MAX;
  for (; offset < length() && uiRet == SIZE_MAX; offset++)
  {
    switch (at(offset))
    {
      case ' ':
        uiRet = offset;
        break;
      case '\t':
        uiRet = offset;
        break;
      case '\n':
        uiRet = offset;
        break;
      case '\r':
        uiRet = offset;
        break;
      case '\f':
        uiRet = offset;
        break;
      case '\v':
        uiRet = offset;
        break;
    }
  }
  return uiRet;
}

char& CcString::at(size_t pos) const
{
  return (*m_String).at(pos);
}

const char* CcString::getCharString(void) const 
{
  const char* ret = (*m_String).c_str();
  return ret;
}

CcByteArray CcString::getByteArray(void) const
{
  CcByteArray ca(getCharString());
  return ca;
}

CcString CcString::getOsPath() const
{
#ifdef WIN32
    return replace('/', '\\');
#else
    return *this;
#endif
}

CcStringList CcString::split(const CcString& delimiter) const
{
  CcStringList slRet;
  size_t save = 0;
  size_t offset = find(delimiter);
  while (offset != SIZE_MAX)
  {
    if (offset != save)
    {
      slRet.append(substr(save, offset -save));
    }
    save = offset + delimiter.length();
    offset = find(delimiter, save);
  }
  if (save != length())
    slRet.append(substr(save));
  return slRet;
}

CcStringList CcString::splitEvery(size_t uiNumber) const
{
  CcStringList slRet;
  size_t i = 0;
  for (; i < length(); i+=uiNumber)
  {
    slRet.add(substr(i, uiNumber));
  }
  return slRet;
}

CcStringList CcString::splitLines() const
{
  CcStringList slRet;
  size_t save = 0;
  size_t offsetN;
  size_t offsetR;
  do
  {
    offsetN = find("\n", save);
    offsetR = find("\r", save);
    if (offsetR < offsetN)
    {
      slRet.append(substr(save, offsetR - save));
      if (offsetR < length()-1 && 
          at(offsetR + 1) )
      {
        save = offsetR + 2;
      }
      else
      {
        save = offsetR + 1;
      }
    }
    else if (offsetN < offsetR)
    {
      slRet.append(substr(save, offsetN - save - 1));
      save = offsetR + 1;
    }
    else
    {
      slRet.append(substr(save));
      save = SIZE_MAX;
    }
  } while (save != SIZE_MAX);
  return slRet;
}


CcString CcString::fromNumber(uint8 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(uint16 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(uint32 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(uint64 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int8 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int16 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int32 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int64 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

#ifdef WIN32
CcString CcString::fromNumber(int number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(uint number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}
#endif

CcString CcString::fromNumber(float number)
{
  CcString sRet;
  sRet.appendNumber(number);
  return sRet;
}

CcString CcString::fromNumber(double number)
{
  CcString sRet;
  sRet.appendNumber(number);
  return sRet;
}

CcString &CcString::normalizePath(void)
{
  *this = replace("\\", "/");
  return *this;
}

CcString CcString::extractFilename(void) const
{
  CcString sRet;
  size_t pos1 = findLast("/");
  size_t pos2 = findLast("\\");
  size_t subStrPos = SIZE_MAX;
  if (pos1 != SIZE_MAX)
  {
    if (pos2 != SIZE_MAX)
    {
      if (pos1 > pos2)
        subStrPos = pos1;
      else
        subStrPos = pos2;
    }
    else{
      subStrPos = pos1;
    }
  }
  else{
    subStrPos = pos2;
  }
  if (subStrPos != SIZE_MAX)
    sRet = substr(subStrPos);
  else 
    sRet = *this;
  return sRet;
}

CcString CcString::extractPath(void)
{
  CcString sRet;
  size_t pos1 = findLast("/");
  if(pos1 != SIZE_MAX)
  {
    sRet = substr(0, pos1 + 1);
  }
  else
  {
    sRet = '/';
  }
  return sRet;
}

CcString &CcString::appendPath(const CcString& toAppend)
{
  CcString seperator;
  if (toAppend.length() > 0)
  {
    if (length() > 0 &&
      at(length() - 1) != '/')
    {
      seperator = "/";
    }
    if (toAppend.startWith("/") ||
      (toAppend.length() > 1 && toAppend.at(1) == ':'))
    {
      clear();
      append(toAppend);
    }
    else
    {
      if (toAppend.startWith("./"))
      {
        append(seperator);
        append(toAppend.substr(2));
      }
      else
      {
        append(seperator);
        append(toAppend);
      }
    }
  }
  return *this;
}

CcString& CcString::fillBegin(const CcString& sFillString, size_t uiCount)
{
  CcString sBeginning;
  for (size_t i = 0; i < uiCount; i++)
  {
    sBeginning << sFillString;
  }
  *this = sBeginning + *this;
  return *this;
}

CcString& CcString::fillBeginUpToLength(const CcString& sFillString, size_t uiCount)
{
  if (uiCount > length())
    fillBegin(sFillString, uiCount - length());
  return *this;
}

CcString& CcString::fillEnd(const CcString& sFillString, size_t uiCount)
{
  CcString sEnding;
  for (size_t i = 0; i < uiCount; i++)
  {
    sEnding.append(sFillString);
  }
  this->append(sEnding);
  return *this;
}

CcString& CcString::fillEndUpToLength(const CcString& sFillString, size_t uiCount)
{
  return fillEnd(sFillString, uiCount - length());
}

CcString CcString::trimL(void) const
{
  CcString sRet = *this;
  while ( 0 != sRet.length() &&
          CcStringUtil::isWhiteSpace(at(0)))
  {
    sRet.m_String->erase(0);
  }
  return sRet;
}

CcString CcString::trimR(void) const
{
  CcString sRet = *this;
  size_t pos = sRet.length() - 1;
  while (pos < sRet.length() &&
          CcStringUtil::isWhiteSpace(at(pos))
        )
  {
    sRet.m_String->erase(pos);
    pos--;
  }
  return sRet;
}

CcString& CcString::append(const CcString& toAppend)
{
  (*m_String).append(toAppend.getStdString());
  return *this;
}

CcString& CcString::append(const char* toAppend)
{
  (*m_String).append(toAppend);
  return *this;
}

CcString& CcString::append(const char toAppend)
{
  (*m_String).append(&toAppend, 1);
  return *this;
}

CcString& CcString::append(const char *toAppend, size_t length)
{
  (*m_String).insert((*m_String).end(), toAppend, toAppend + length);
  return *this;
}

CcString& CcString::append(const CcByteArray &toAppend, size_t pos, size_t len)
{
  if (len == SIZE_MAX)
    len = toAppend.size()-pos;
  char* arr = toAppend.getArray(pos);
  (*m_String).append(arr, len);
  return *this;
}

CcString& CcString::append(std::string &toAppend)
{
  (*m_String).append(toAppend);
  return *this;
}


CcString& CcString::prepend(const CcString& toAppend)
{
  (*m_String).insert(0, toAppend.getStdString());
  return *this;
}

CcString& CcString::prepend(const char* toAppend)
{
  (*m_String).insert(0, toAppend);
  return *this;
}

CcString& CcString::prepend(const char toAppend)
{
  (*m_String).insert(0, &toAppend, 1);
  return *this;
}

CcString& CcString::prepend(const char *toAppend, size_t length)
{
  (*m_String).insert(0, toAppend, length);
  return *this;
}

CcString& CcString::prepend(const CcByteArray &toAppend, size_t pos, size_t len)
{
  if (len == SIZE_MAX)
    len = toAppend.size() - pos;
  char* arr = toAppend.getArray(pos);
  (*m_String).insert(0, arr, len);
  return *this;
}

CcString& CcString::prepend(const std::string &toAppend)
{
  (*m_String).insert(0, toAppend);
  return *this;
}

CcString& CcString::set(const CcString& toAppend)
{
  clear();
  append(toAppend);
  return *this;
}

CcString& CcString::setOsPath(const CcString & sPathToSet)
{
#ifdef WIN32
  return set(sPathToSet.replace('/', '\\').getCharString());
#else
  return set(sPathToSet.replace('/', '\\').getCharString());
#endif
}

CcString& CcString::appendIPv4(const ipv4_t &ipAddr)
{
  appendNumber(ipAddr.ip4);
  append('.');
  appendNumber(ipAddr.ip3);
  append('.');
  appendNumber(ipAddr.ip2);
  append('.');
  appendNumber(ipAddr.ip1);
  return *this;
}

CcString& CcString::insert(size_t pos, const CcString& toInsert)
{
  (*m_String).insert(pos, toInsert.getCharString());
  return *this;
}

void CcString::clear( void )
{
  (*m_String).clear();
}

CcString &CcString::erase(size_t pos, size_t len)
{
  if (len == SIZE_MAX)
    len = length();
  (*m_String).erase(pos, len);
  return *this;
}

bool CcString::operator<(const CcString& toCompare)
{
  if ((*m_String) < toCompare.getStdString())
    return true;
  return false;
}

bool CcString::operator>(const CcString& toCompare)
{
  if ((*m_String) > toCompare.getStdString())
    return true;
  return false;
}

CcString& CcString::operator=(CcString&& oToMove)
{
  if (this != &oToMove)
  {
    //CCMONITORDELETE(m_String);
    delete m_String;
    m_String = oToMove.m_String;
    oToMove.m_String = nullptr;
  }
  return *this;
}

#ifdef WIN32

CcString::CcString(wchar_t * wstr) :
CcString("")
{
  append(wstr);
}

const char* CcString::getLPCSTR(void) const
{
  return m_String->c_str();
}

char*   CcString::getLPSTR(void) const
{
  return (char*)getLPCSTR();
}

const wchar_t* CcString::getLPCWSTR(void) const
{
  return getStdWString().c_str();
}

void CcString::append(const wchar_t* str)
{
  if (str != nullptr)
  {
    std::wstring wstr(str);
    for (std::wstring::const_iterator it = wstr.begin();
      it != wstr.end();
      ++it)
    {
      append(static_cast<char>(*it));
    }
  }
}

void CcString::append(const std::wstring& str)
{
  m_String->append(str.begin(), str.end());
}

#endif
