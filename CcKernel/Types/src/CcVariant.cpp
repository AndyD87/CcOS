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
 * @brief     Implementation of Class CcVariant
 */
#include "CcVariant.h"
#include "CcStringUtil.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcUrl.h"
#include "CcDateTime.h"

#ifdef WIN32
std::string wstrtostr(const std::wstring &wstr)
{
    // Convert a Unicode string to an ASCII string
    std::string strTo;
    char *szTo = new char[wstr.length() + 1]; CCMONITORNEW(szTo);
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), nullptr, nullptr);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

std::wstring strtowstr(const std::string &str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1]; CCMONITORNEW(wszTo);
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}
#endif

CcVariant::CcVariant( void ):
  m_eType(EVariantType::NoType)
{
  m_Data.Pointer = nullptr;
}

CcVariant::CcVariant(EVariantType eType)
{
  m_eType = eType;
  if (eType == EVariantType::String)
  {
    m_Data.String = new CcString(); CCMONITORNEW(m_Data.String);
  }
  else
  {
    m_Data.ui64Data = 0;
  }
}

CcVariant::CcVariant(bool bVal)
{
  set(bVal);
}

CcVariant::CcVariant(uint8  uiToCopy)
{
  set(uiToCopy);
}

CcVariant::CcVariant(uint16 uiToCopy)
{
  set(uiToCopy);
}

CcVariant::CcVariant(uint32 uiToCopy)
{
  set(uiToCopy);
}

CcVariant::CcVariant(uint64 uiToCopy)
{
  set(uiToCopy);
}

CcVariant::CcVariant(int8  iToCopy)
{
  set(iToCopy);
}

CcVariant::CcVariant(int16 iToCopy)
{
  set(iToCopy);
}

CcVariant::CcVariant(int32 iToCopy)
{
  set(iToCopy);
}

CcVariant::CcVariant(int64 iToCopy)
{
  set(iToCopy);
}

CcVariant::CcVariant(float fToCopy)
{
  set(fToCopy);
}

CcVariant::CcVariant(double dToCopy)
{
  set(dToCopy);
}

CcVariant::CcVariant(const CcVariant &copy)
{
  *this = std::move(copy);
}

CcVariant::CcVariant(const CcByteArray& oToCopy)
{
  m_eType = EVariantType::ByteArray;
  m_Data.ByteArray = new CcByteArray(oToCopy); 
  CCMONITORNEW(m_Data.ByteArray);
}

CcVariant::CcVariant(const CcString& sToCopy)
{
  m_eType = EVariantType::String;
  m_Data.String = new CcString(sToCopy); 
  CCMONITORNEW(m_Data.String);
}

CcVariant::CcVariant(const char* pcToCopy)
{
  m_eType = EVariantType::String;
  m_Data.String = new CcString(pcToCopy); 
  CCMONITORNEW(m_Data.String);
}

#ifdef WIN32
CcVariant::CcVariant(VARIANT &winVariant, VARENUM winVariantType)
{
  set(winVariant, winVariantType);
}
#endif

CcVariant::~CcVariant( void )
{
  clear();
}

void CcVariant::clear(void)
{
  switch (m_eType) {
    case EVariantType::String:
      // delete String
      CCMONITORDELETE(m_Data.String); 
      delete m_Data.String;
      break;
    case EVariantType::ByteArray:
      // delete String
      CCMONITORDELETE(m_Data.String); 
      delete m_Data.ByteArray;
      break;
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Int8:
    case EVariantType::Uint8:
    case EVariantType::Int16:
    case EVariantType::Uint16:
    case EVariantType::Int32:
    case EVariantType::Uint32:
    case EVariantType::Int64:
    case EVariantType::Uint64:
    case EVariantType::Size:
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::DateTime:
    case EVariantType::Pointer:
    default:
      // setting uin64 to zero for memset 0
      m_Data.ui64Data = 0;
      break;
  };
  m_eType = EVariantType::NoType;
}

bool CcVariant::getBool(bool *bOk) const
{
  bool bRet = false;
  bool bSuccess = false;
  switch (m_eType)
  {
    case EVariantType::Bool:
      bRet = m_Data.bData;
      bSuccess = true;
      break;
    case EVariantType::NoType:
      break;
    case EVariantType::Int8:
      if (m_Data.i8Data != 0)
        bRet = true;
      break;
    case EVariantType::Uint8:
      if (m_Data.ui8Data != 0)
        bRet = true;
      break;
    case EVariantType::Int16:
      if (m_Data.i16Data != 0)
        bRet = true;
      break;
    case EVariantType::Uint16:
      if (m_Data.ui16Data != 0)
        bRet = true;
      break;
    case EVariantType::Int32:
      if (m_Data.i32Data != 0)
        bRet = true;
      break;
    case EVariantType::Uint32:
      if (m_Data.ui32Data != 0)
        bRet = true;
      break;
    case EVariantType::Int64:
      if (m_Data.i64Data != 0)
        bRet = true;
      break;
    case EVariantType::Uint64:
      if (m_Data.ui64Data != 0)
        bRet = true;
      break;
    case EVariantType::Size:
      if (m_Data.Size != 0)
        bRet = true;
      break;
    case EVariantType::Float:
      if(m_Data.Float > 0)
        bRet = true;
      break;
    case EVariantType::Double:
      if (m_Data.Double > 0)
        bRet = true;
      break;
    case EVariantType::String:
      {
        bRet = CcStringUtil::getBoolFromStirng(*m_Data.String, bOk);
      }
      break;
    case EVariantType::DateTime:
      if (m_Data.Time != 0)
        bRet = true;
      break;
    case EVariantType::Pointer:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return bRet;
}

int8 CcVariant::getInt8(bool *bOk) const
{
  int8 iRet=0;
  bool bSuccess = false;
  switch (m_eType)
  {
    case EVariantType::Bool:
      iRet = (int8)m_Data.bData;
      break;
    case EVariantType::NoType:
      break;
    case EVariantType::Int8:
      iRet = m_Data.i8Data;
      bSuccess = true;
      break;
    case EVariantType::Uint8:
      iRet = (int8)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      iRet = (int8)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      iRet = (int8)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      iRet = (int8)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      iRet = (int8)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      iRet = (int8)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      iRet = (int8)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      iRet = (int8)m_Data.Size;
      break;
    case EVariantType::Float:
      iRet = (int8)m_Data.Float;
      break;
    case EVariantType::Double:
      iRet = (int8)m_Data.Double;
      break;
    case EVariantType::String:
      {
        iRet = m_Data.String->toInt8(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      iRet = (int8)m_Data.Time->getTimestampUs();
      break;
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return iRet;
}

uint8 CcVariant::getUint8(bool *bOk) const
{
  uint8 uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (uint8)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = (uint8)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = m_Data.ui8Data;
      bSuccess = true;
      break;
    case EVariantType::Int16:
      uiRet = (uint8)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (uint8)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (uint8)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (uint8)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (uint8)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (uint8)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (uint8)m_Data.Size;
      break;
    case EVariantType::Float:
      uiRet = (uint8)m_Data.Float;
      break;
    case EVariantType::Double:
      uiRet = (uint8)m_Data.Double;
    case EVariantType::String:
      {
        uiRet = m_Data.String->toUint8(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      uiRet = (uint8) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

int16 CcVariant::getInt16(bool *bOk) const
{
  int16 iRet=0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      iRet = (int16)m_Data.bData;
      break;
    case EVariantType::NoType:
      iRet = 0;
      break;
    case EVariantType::Int8:
      iRet = (int16)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      iRet = (int16)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      iRet = m_Data.i16Data;
      bSuccess = true;
      break;
    case EVariantType::Uint16:
      iRet = (int16)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      iRet = (int16)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      iRet = (int16)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      iRet = (int16)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      iRet = (int16)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      iRet = (int16)m_Data.Size;
      break;
    case EVariantType::Float:
      iRet = (int16)m_Data.Float;
      break;
    case EVariantType::Double:
      iRet = (int16)m_Data.Double;
    case EVariantType::String:
      {
        iRet = m_Data.String->toInt16(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      iRet = (int16) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return iRet;
}

uint16 CcVariant::getUint16(bool *bOk) const
{
  uint16 uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (uint16)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = (uint16)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (uint16)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (uint16)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = m_Data.ui16Data;
      bSuccess = true;
      break;
    case EVariantType::Int32:
      uiRet = (uint16)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (uint16)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (uint16)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (uint16)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (uint16)m_Data.Size;
      break;
    case EVariantType::Float:
      uiRet = (uint16)m_Data.Float;
      break;
    case EVariantType::Double:
      uiRet = (uint16)m_Data.Double;
    case EVariantType::String:
      {
        uiRet = m_Data.String->toUint16(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      uiRet = (uint16) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

int32 CcVariant::getInt32(bool *bOk) const
{
  int32 iRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      iRet = (int32)m_Data.bData;
      break;
    case EVariantType::NoType:
      iRet = 0;
      break;
    case EVariantType::Int8:
      iRet = (int32)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      iRet = (int32)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      iRet = (int32)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      iRet = (int32)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      iRet = m_Data.i32Data;
      bSuccess = true;
      break;
    case EVariantType::Uint32:
      iRet = (int32)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      iRet = (int32)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      iRet = (int32)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      iRet = (int32)m_Data.Size;
      break;
    case EVariantType::Float:
      iRet = (int32)m_Data.Float;
      break;
    case EVariantType::Double:
      iRet = (int32)m_Data.Double;
      break;
    case EVariantType::String:
      {
        iRet = m_Data.String->toInt32(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      iRet = (int32) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return iRet;
}

uint32 CcVariant::getUint32(bool *bOk) const
{
  uint32 uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (uint32)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = (uint32)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (uint32)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (uint32)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (uint32)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (uint32)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = m_Data.ui32Data;
      bSuccess = true;
      break;
    case EVariantType::Int64:
      uiRet = (uint32)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (uint32)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (uint32)m_Data.Size;
      break;
    case EVariantType::Float:
      uiRet = (uint32)m_Data.Float;
      break;
    case EVariantType::Double:
      uiRet = (uint32)m_Data.Double;
      break;
    case EVariantType::String:
      uiRet = m_Data.String->toUint32(&bSuccess);
      break;
    case EVariantType::DateTime:
      uiRet = (uint32) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

int64 CcVariant::getInt64(bool *bOk) const
{
  int64 iRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      iRet = (int64)m_Data.bData;
      break;
    case EVariantType::NoType:
      iRet = 0;
      break;
    case EVariantType::Int8:
      iRet = (int64)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      iRet = (int64)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      iRet = (int64)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      iRet = (int64)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      iRet = (int64)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      iRet = (int64)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      iRet = m_Data.i64Data;
      bSuccess = true;
      break;
    case EVariantType::Uint64:
      iRet = (int64)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      iRet = (int64)m_Data.Size;
      break;
    case EVariantType::Float:
      iRet = (int64)m_Data.Float;
      break;
    case EVariantType::Double:
      iRet = (int64)m_Data.Double;
      break;
    case EVariantType::String:
      {
        iRet = m_Data.String->toInt64(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      iRet = (int64) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return iRet;
}

uint64 CcVariant::getUint64(bool *bOk) const
{
  uint64 uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (uint64)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = (uint64)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (uint64)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (uint64)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (uint64)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (uint64)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (uint64)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (uint64)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = m_Data.ui64Data;
      bSuccess = true;
      break;
    case EVariantType::Size:
      uiRet = (uint64)m_Data.Size;
      break;
    case EVariantType::Float:
      uiRet = (uint64)m_Data.Float;
      break;
    case EVariantType::Double:
      uiRet = (uint64)m_Data.Double;
      break;
    case EVariantType::String:
      {
        uiRet = m_Data.String->toUint64(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      uiRet = (uint64) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

int CcVariant::getInt(bool *bOk) const
{
  int iRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      iRet = (int)m_Data.bData;
      break;
    case EVariantType::NoType:
      iRet = 0;
      break;
    case EVariantType::Int8:
      iRet = (int)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      iRet = (int)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      iRet = (int)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      iRet = (int)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      iRet = (int)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      iRet = (int)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      iRet = (int)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      iRet = (int)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      iRet = (int)m_Data.Size;
      break;
    case EVariantType::Float:
      iRet = (int)m_Data.Float;
      break;
    case EVariantType::Double:
      iRet = (int)m_Data.Double;
      break;
    case EVariantType::String:
      {
        iRet = (int)m_Data.String->toInt64(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      iRet = (int) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return iRet;
}

uint CcVariant::getUint(bool *bOk) const
{
  uint uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (uint)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = (uint)m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (uint)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (uint)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (uint)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (uint)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (uint)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (uint)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (uint)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (uint)m_Data.Size;
      break;
    case EVariantType::Float:
      uiRet = (uint)m_Data.Float;
      break;
    case EVariantType::Double:
      uiRet = (uint)m_Data.Double;
      break;
    case EVariantType::String:
      {
        uiRet = (uint)m_Data.String->toUint64(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      uiRet = (uint) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

size_t CcVariant::getSize(bool *bOk) const
{
  size_t uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (size_t)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (size_t)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (size_t)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (size_t)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (size_t)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (size_t)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (size_t)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (size_t)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (size_t)m_Data.Size;
      bSuccess = true;
      break;
    case EVariantType::Float:
      uiRet = (size_t)m_Data.Float;
      break;
    case EVariantType::Double:
      uiRet = (size_t)m_Data.Double;
      break;
    case EVariantType::String:
      {
        uiRet = m_Data.String->toUint64(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      uiRet = (size_t) m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

float CcVariant::getFloat(bool *bOk) const
{
  float uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (float)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (float)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (float)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (float)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (float)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (float)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (float)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (float)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (float)m_Data.Size;
      bSuccess = true;
      break;
    case EVariantType::Float:
      uiRet = m_Data.Float;
      bSuccess = true;
      break;
    case EVariantType::Double:
      uiRet = (float)m_Data.Double;
      bSuccess = true;
      break;
    case EVariantType::String:
      uiRet = m_Data.String->toFloat(&bSuccess);
      break;
    case EVariantType::DateTime:
      uiRet = (float)m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if (bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

double CcVariant::getDouble(bool *bOk) const
{
  double uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (double)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (double)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (double)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (double)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (double)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (double)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (double)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (double)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (double)m_Data.Size;
      bSuccess = true;
      break;
    case EVariantType::Float:
      uiRet = (double)m_Data.Float;
      bSuccess = true;
      break;
    case EVariantType::Double:
      uiRet = m_Data.Double;
      bSuccess = true;
      break;
    case EVariantType::String:
      uiRet = m_Data.String->toDouble(&bSuccess);
      break;
    case EVariantType::DateTime:
      uiRet = (double)m_Data.Time->getTimestampUs();
      break;
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if (bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

CcDateTime CcVariant::getTime(bool *bOk) const
{
  CcDateTime uiRet = 0;
  bool bSuccess = false;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      uiRet = (time_t)m_Data.bData;
      break;
    case EVariantType::NoType:
      uiRet = 0;
      break;
    case EVariantType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case EVariantType::Uint8:
      uiRet = (time_t)m_Data.ui8Data;
      break;
    case EVariantType::Int16:
      uiRet = (time_t)m_Data.i16Data;
      break;
    case EVariantType::Uint16:
      uiRet = (time_t)m_Data.ui16Data;
      break;
    case EVariantType::Int32:
      uiRet = (time_t)m_Data.i32Data;
      break;
    case EVariantType::Uint32:
      uiRet = (time_t)m_Data.ui32Data;
      break;
    case EVariantType::Int64:
      uiRet = (time_t)m_Data.i64Data;
      break;
    case EVariantType::Uint64:
      uiRet = (time_t)m_Data.ui64Data;
      break;
    case EVariantType::Size:
      uiRet = (time_t)m_Data.Size;
      break;
    case EVariantType::String:
      {
        uiRet = (time_t)m_Data.String->toUint64(&bSuccess);
      }
      break;
    case EVariantType::DateTime:
      uiRet = *m_Data.Time;
      bSuccess = true;
      break;
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::Pointer:
    case EVariantType::ByteArray:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

CcString CcVariant::getString(bool *bOk) const
{
  CcString sRet;
  bool bSuccess = true;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::Bool:
      if (m_Data.bData == false)
        sRet = "0";
      else
        sRet = "1";
      break;
    case EVariantType::NoType:
      bSuccess = true;
      bSuccess=false;
      break;
    case EVariantType::Int8:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i8Data);
      break;
    case EVariantType::Uint8:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui8Data);
      break;
    case EVariantType::Int16:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i16Data);
      break;
    case EVariantType::Uint16:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui16Data);
      break;
    case EVariantType::Int32:
      sRet = CcString::fromNumber(m_Data.i32Data);
      break;
    case EVariantType::Uint32:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui32Data);
      break;
    case EVariantType::Int64:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i64Data);
      break;
    case EVariantType::Uint64:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui64Data);
      break;
    case EVariantType::Size:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.Size);
      break;
    case EVariantType::Float:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.Float);
      break;
    case EVariantType::Double:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.Double);
      break;
    case EVariantType::String:
      {
        bSuccess = true;
        sRet = *m_Data.String;
      }
      break;
    case EVariantType::ByteArray:
      {
        bSuccess = true;
        sRet.append(*m_Data.ByteArray);
      }
      break;
    case EVariantType::DateTime:
      sRet = CcString::fromNumber(m_Data.Time->getTimestampUs());
      bSuccess = true;
      break;
    case EVariantType::Pointer:
    default:
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return sRet;
}

CcByteArray CcVariant::getByteArray(bool *bOk) const
{
  CcByteArray oRet;
  bool bSuccess = true;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::ByteArray:
    {
      bSuccess = true;
      oRet = *m_Data.String;
      break;
    }
    case EVariantType::String:
    {
      bSuccess = true;
      oRet = (*m_Data.String).getByteArray();
      break;
    }
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Int8:
    case EVariantType::Uint8:
    case EVariantType::Int16:
    case EVariantType::Uint16:
    case EVariantType::Int32:
    case EVariantType::Uint32:
    case EVariantType::Int64:
    case EVariantType::Uint64:
    case EVariantType::Size:
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::DateTime:
    case EVariantType::Pointer:
    default:
      break;
  }
  if (bOk != nullptr)
    *bOk = bSuccess;
  return oRet;
}

void* CcVariant::getVoid(bool *bOk) const
{
  void* oRet = nullptr;
  bool bSuccess = true;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    case EVariantType::ByteArray:
    case EVariantType::String:
    case EVariantType::Pointer:
    {
      bSuccess = true;
      oRet = m_Data.Pointer;
      break;
    }
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Int8:
    case EVariantType::Uint8:
    case EVariantType::Int16:
    case EVariantType::Uint16:
    case EVariantType::Int32:
    case EVariantType::Uint32:
    case EVariantType::Int64:
    case EVariantType::Uint64:
    case EVariantType::Size:
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::DateTime:
    default:
      break;
  }
  if (bOk != nullptr)
    *bOk = bSuccess;
  return oRet;
}

#ifdef WIN32
VARIANT CcVariant::getWinVariant(VARENUM& winVariantType)
{
  // @todo implementation for Windows variant output
  VARIANT vRet;
  CCUNUSED(winVariantType);
  memset(&vRet, 0, sizeof(vRet));
  return vRet;
}
#endif

void CcVariant::set(bool bVal)
{
  if(m_eType != EVariantType::Bool)
  {
    clear();
  }
  m_Data.bData = bVal;
  m_eType = EVariantType::Bool;
}

void CcVariant::set(int8 i8Val)
{
  if(m_eType != EVariantType::Int8)
  {
    clear();
  }
  m_Data.i8Data = i8Val;
  m_eType = EVariantType::Int8;
}

void CcVariant::set(uint8 ui8Val)
{
  if(m_eType != EVariantType::Uint8)
  {
    clear();
  }
  m_Data.ui8Data = ui8Val;
  m_eType = EVariantType::Uint8;
}

void CcVariant::set(int16 i16Val)
{
  if(m_eType != EVariantType::Uint16)
  {
    clear();
  }
  m_Data.i16Data = i16Val;
  m_eType = EVariantType::Int16;
}

void CcVariant::set(uint16 ui16Val)
{
  if(m_eType != EVariantType::Uint16)
  {
    clear();
  }
  m_Data.ui16Data = ui16Val;
  m_eType = EVariantType::Uint16;
}

void CcVariant::set(int32 i32Val)
{
  if(m_eType != EVariantType::Int32)
  {
    clear();
  }
  m_Data.i32Data = i32Val;
  m_eType = EVariantType::Int32;
}

void CcVariant::set(uint32 ui32Val)
{
  if(m_eType != EVariantType::Uint32)
  {
    clear();
  }
  m_Data.ui32Data = ui32Val;
  m_eType = EVariantType::Uint32;
}

void CcVariant::set(int64 i64Val)
{
  if(m_eType != EVariantType::Int64)
  {
    clear();
  }
  m_Data.i64Data = i64Val;
  m_eType = EVariantType::Int64;
}

void CcVariant::set(uint64 ui64Val)
{
  if(m_eType != EVariantType::Uint64)
  {
    clear();
  }
  m_Data.ui64Data = ui64Val;
  m_eType = EVariantType::Uint64;
}

void CcVariant::set(float fVal)
{
  if(m_eType != EVariantType::Float)
  {
    clear();
  }
  m_Data.Float = fVal;
  m_eType = EVariantType::Float;
}

void CcVariant::set(double dVal)
{
  if (m_eType != EVariantType::Double)
  {
    clear();
  }
  m_Data.Double = dVal;
  m_eType = EVariantType::Double;
}

void CcVariant::set(const char* val)
{
  if (m_eType != EVariantType::String)
  {
    clear();
  }
  m_eType = EVariantType::String;
  m_Data.String = new CcString(); 
  CCMONITORNEW(m_Data.String);
  *m_Data.String = val;
}

void CcVariant::set(const CcString& val)
{
  if (m_eType != EVariantType::String)
  {
    clear();
  }
  m_eType = EVariantType::String;
  m_Data.String = new CcString(); 
  CCMONITORNEW(m_Data.String);
  *m_Data.String = val;
}

void CcVariant::set(const CcByteArray& val)
{
  if (m_eType != EVariantType::ByteArray)
  {
    clear();
  }
  m_eType = EVariantType::ByteArray;
  m_Data.ByteArray = new CcByteArray(); CCMONITORNEW(m_Data.ByteArray);
  *m_Data.ByteArray = val;
}

void CcVariant::set(const CcDateTime& val)
{
  if (m_eType != EVariantType::DateTime)
  {
    clear();
  }
  m_eType = EVariantType::ByteArray;
  m_Data.Time = new CcDateTime(val); 
  CCMONITORNEW(m_Data.ByteArray);
}

void CcVariant::set(void* val)
{
  if (m_eType != EVariantType::Pointer)
  {
    clear();
  }
  m_eType = EVariantType::Pointer;
  m_Data.Pointer = val;
}

bool CcVariant::isInt() const
{
  bool bRet = false;
  switch (m_eType)
  {
    case EVariantType::Int8:
    case EVariantType::Int16:
    case EVariantType::Int32:
    case EVariantType::Int64:
      bRet = true;
      break;
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Uint8:
    case EVariantType::Uint16:
    case EVariantType::Uint32:
    case EVariantType::Uint64:
    case EVariantType::Size:
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::DateTime:
    case EVariantType::Pointer:
    default:
      break;
  }
  return bRet;
}

bool CcVariant::isUint() const
{
  bool bRet = false;
  switch (m_eType)
  {
    case EVariantType::Uint8:
    case EVariantType::Uint16:
    case EVariantType::Uint32:
    case EVariantType::Uint64:
    case EVariantType::Size:
      bRet = true;
      break;
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Int8:
    case EVariantType::Int16:
    case EVariantType::Int32:
    case EVariantType::Int64:
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::DateTime:
    case EVariantType::Pointer:
    default:
      break;
  }
  return bRet;
}

bool CcVariant::isFloat() const
{
  bool bRet = false;
  switch (m_eType)
  {
    case EVariantType::Double:
    case EVariantType::Float:
      bRet = true;
      break;
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Uint8:
    case EVariantType::Int8:
    case EVariantType::Uint16:
    case EVariantType::Int16:
    case EVariantType::Uint32:
    case EVariantType::Int32:
    case EVariantType::Uint64:
    case EVariantType::Int64:
    case EVariantType::Size:
    case EVariantType::DateTime:
    case EVariantType::Pointer:
    default:
      break;
  }
  return bRet;
}

bool CcVariant::isString() const
{
  bool bRet = false;
  switch (m_eType)
  {
    case EVariantType::String:
      bRet = true;
      break;
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Uint8:
    case EVariantType::Int8:
    case EVariantType::Uint16:
    case EVariantType::Int16:
    case EVariantType::Uint32:
    case EVariantType::Int32:
    case EVariantType::Uint64:
    case EVariantType::Int64:
    case EVariantType::Size:
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::DateTime:
    case EVariantType::Pointer:
    default:
      break;
  }
  return bRet;
}

bool CcVariant::isByteArray() const
{
  bool bRet = false;
  switch (m_eType)
  {
    case EVariantType::ByteArray:
      bRet = true;
      break;
    case EVariantType::NoType:
    case EVariantType::Bool:
    case EVariantType::Uint8:
    case EVariantType::Int8:
    case EVariantType::Uint16:
    case EVariantType::Int16:
    case EVariantType::Uint32:
    case EVariantType::Int32:
    case EVariantType::Uint64:
    case EVariantType::Int64:
    case EVariantType::Size:
    case EVariantType::Float:
    case EVariantType::Double:
    case EVariantType::DateTime:
    case EVariantType::Pointer:
    default:
      break;
  }
  return bRet;
}

bool CcVariant::convert(EVariantType eType)
{
  bool bSuccess = false;
  switch (eType)
  {
    case EVariantType::Bool:
    {
      bool bConv = getBool(&bSuccess);
      if(bSuccess) set(bConv);
      break;
    }
    case EVariantType::Int8:
    {
      int8 bConv = getInt8(&bSuccess);
      if (bSuccess) set(bConv);
      break;
    }
    case EVariantType::Uint8:
    {
      uint8 oConv = getUint8(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Int16:
    {
      int16 oConv = getInt16(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Uint16:
    {
      uint16 oConv = getUint16(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Int32:
    {
      int32 oConv = getInt32(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Uint32:
    {
      uint32 oConv = getUint32(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Int64:
    {
      int64 oConv = getInt64(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Uint64:
    {
      uint64 oConv = getUint64(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Size:
    {
      size_t oConv = getSize(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Float:
    {
      float oConv = getFloat(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::Double:
    {
      double oConv = getDouble(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::DateTime:
    {
      CcDateTime oConv = getTime(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case EVariantType::String:
    {
      CcString oConv = getString(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case EVariantType::ByteArray:
    {
      CcByteArray oConv = getByteArray(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case EVariantType::Pointer:
    default:
      break;
  }
  return bSuccess;
}

bool CcVariant::operator==(const CcVariant& oToCompare) const
{
  bool bSuccess = false;
  if (oToCompare.m_eType == m_eType)
  {
    switch (m_eType)
    {
      case EVariantType::String:
        if (*oToCompare.m_Data.String == *m_Data.String)
          bSuccess = true;
        break;
      case EVariantType::ByteArray:
        if (*oToCompare.m_Data.ByteArray == *m_Data.ByteArray)
          bSuccess = true;
        break;
      default:
        if (oToCompare.m_Data.ui64Data == m_Data.ui64Data)
          bSuccess = true;
    }
  }
  return bSuccess;
}

CcVariant& CcVariant::operator=(const CcVariant& oToCopy)
{
  m_eType = oToCopy.getType();
  switch (m_eType)
  {
    case EVariantType::String:
      m_Data.String = new CcString(*oToCopy.m_Data.String); 
      CCMONITORNEW(m_Data.String);
      break;
    case EVariantType::DateTime:
      *m_Data.Time = oToCopy.getTime();
      break;
    default:
      m_Data.ui64Data = oToCopy.m_Data.ui64Data;
      break;
  }
  return *this;
}

CcVariant& CcVariant::operator=(CcVariant&& oToMove)
{
  if (this != &oToMove)
  {
    clear();
    m_Data = oToMove.m_Data;
    m_eType = oToMove.m_eType;
    oToMove.m_eType = EVariantType::NoType;
    oToMove.m_Data.i64Data = 0;
  }
  return *this;
}

#ifdef WIN32
void CcVariant::set(VARIANT &winVariant, VARENUM winVariantType)
{
  switch (winVariantType)
  {
    case VT_DATE:
    case VT_BSTR:
      /// @todo string conversion, see functions above
    case VT_BOOL:
      set(winVariant.boolVal);
      break;
    case VT_I1:
      set(winVariant.cVal);
      break;
    case VT_UI1:
      set(winVariant.bVal);
      break;
    case VT_UI2:
      set(winVariant.uiVal);
      break;
    case VT_UI4:
      set(winVariant.uintVal);
      break;
    case VT_I8:
      set(winVariant.cVal);
      break;
    case VT_UI8:
      set(winVariant.bVal);
      break;
    case VT_INT:
      set(winVariant.intVal);
      break;
    case VT_UINT:
      set(winVariant.uintVal);
      break;
    default:
      CCDEBUG("CcVariant: Unkown Conversion form WINVARIANT-type: " + CcString::fromNumber(winVariantType));
  }
}
#endif
