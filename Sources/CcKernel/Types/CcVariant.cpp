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
 * @brief     Implementation of Class CcVariant
 */
#include "CcVariant.h"
#include "CcStringUtil.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcUrl.h"
#include "CcDateTime.h"
#include "CcVersion.h"
#include "CcUuid.h"
#include "CcGlobalStrings.h"
#include "CcIp.h"
#include "IIo.h"
#include "CcVariantList.h"
#include "CcStringList.h"
#include "CcStatic.h"
#ifdef WINDOWS
  #include "propvarutil.h"

   // Compiler specific settings
  #ifdef _MSC_VER
    #pragma warning(default : 4061) // Enable unhandled enum warning
  #endif

#endif // WINDOWS

CcVariant CcVariant::oNullVariant;

CcVariant::CcVariant():
  m_eType(CcVariant::EType::NoType)
{
  m_Data.Pointer = nullptr;
}

CcVariant::CcVariant(CcVariant::EType eType)
{
  convert(eType);
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

CcVariant::CcVariant(const CcVariant &oToCopy)
{
  operator=(oToCopy);
}

CcVariant::CcVariant(const CcDateTime& oToCopy)
{
  m_eType = CcVariant::EType::DateTime;
  CCNEW(m_Data.DateTime, CcDateTime, oToCopy);
}

CcVariant::CcVariant(const CcStringList& oToCopy)
{
  m_eType = CcVariant::EType::StringList;
  CCNEW(m_Data.StringList, CcStringList, oToCopy);
}

CcVariant::CcVariant(const CcByteArray& oToCopy)
{
  m_eType = CcVariant::EType::ByteArray;
  CCNEW(m_Data.ByteArray, CcByteArray, oToCopy);
}

CcVariant::CcVariant(const CcString& sToCopy)
{
  m_eType = CcVariant::EType::String;
  CCNEW(m_Data.String, CcString, sToCopy);
}

CcVariant::CcVariant(const char* pcToCopy)
{
  if (pcToCopy == nullptr)
  {
    m_eType = CcVariant::EType::NoType;
    m_Data.i64Data = 0;
  }
  else
  {
    m_eType = CcVariant::EType::String;
    CCNEW(m_Data.String, CcString, pcToCopy);
  }
}

CcVariant::CcVariant(const CcVariantList& oToCopy)
{
  m_eType = CcVariant::EType::VariantList;
  CCNEW(m_Data.VariantList, CcVariantList, oToCopy);
}

CcVariant::CcVariant(const CcVersion& oVersion)
{
  m_eType = CcVariant::EType::Version;
  CCNEW(m_Data.Version, CcVersion, oVersion);
}

CcVariant::CcVariant(const CcUuid& oToCopy)
{
  m_eType = CcVariant::EType::Uuid;
  CCNEW(m_Data.Uuid, CcUuid, oToCopy);
}

CcVariant::CcVariant(const CcIp& oIp)
{
  m_eType = CcVariant::EType::Ip;
  CCNEW(m_Data.Ip, CcIp, oIp);
}

#ifdef WINDOWS
CcVariant::CcVariant(VARIANT &winVariant, VARENUM winVariantType)
{
  set(winVariant, winVariantType);
}
#endif

CcVariant::~CcVariant()
{
  clear();
}

void CcVariant::clear()
{
  switch (m_eType) 
  {
    default:
      // Force compiler warning here for unhandled types
      m_Data.Pointer = nullptr;
      break;
    case CcVariant::EType::ByteArray:
      CCDELETE(m_Data.ByteArray);
      break;
    case CcVariant::EType::DateTime:
      CCDELETE(m_Data.DateTime);
      break;
    case CcVariant::EType::String:
      CCDELETE(m_Data.String);
      break;
    case CcVariant::EType::StringList:
      CCDELETE(m_Data.StringList);
      break;
    case CcVariant::EType::VariantList:
      CCDELETE(m_Data.VariantList);
      break;
    case CcVariant::EType::Version:
      CCDELETE(m_Data.Version);
      break;
    case CcVariant::EType::Uuid:
      CCDELETE(m_Data.Uuid);
      break;
    case CcVariant::EType::Ip:
      CCDELETE(m_Data.Ip);
      break;
    case CcVariant::EType::NoType:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      CCFALLTHROUGH;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Int8:
      CCFALLTHROUGH;
    case CcVariant::EType::Uint8:
      CCFALLTHROUGH;
    case CcVariant::EType::Int16:
      CCFALLTHROUGH;
    case CcVariant::EType::Uint16:
      CCFALLTHROUGH;
    case CcVariant::EType::Int32:
      CCFALLTHROUGH;
    case CcVariant::EType::Uint32:
      CCFALLTHROUGH;
    case CcVariant::EType::Int64:
      CCFALLTHROUGH;
    case CcVariant::EType::Uint64:
      CCFALLTHROUGH;
    case CcVariant::EType::Size:
      CCFALLTHROUGH;
    case CcVariant::EType::Float:
      CCFALLTHROUGH;
    case CcVariant::EType::Double:
      CCFALLTHROUGH;
    case CcVariant::EType::Pointer:
      break;
  };
  m_Data.ui64Data = 0;
  m_eType = CcVariant::EType::NoType;
}

bool CcVariant::getBool(bool *bOk) const
{
  bool bRet = false;
  bool bSuccess = false;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      bRet = m_Data.bData;
      bSuccess = true;
      break;
    case CcVariant::EType::NoType:
      break;
    case CcVariant::EType::Int8:
      if (m_Data.i8Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Uint8:
      if (m_Data.ui8Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Int16:
      if (m_Data.i16Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Uint16:
      if (m_Data.ui16Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Int32:
      if (m_Data.i32Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Uint32:
      if (m_Data.ui32Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Int64:
      if (m_Data.i64Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Uint64:
      if (m_Data.ui64Data != 0)
        bRet = true;
      break;
    case CcVariant::EType::Size:
      if (m_Data.Size != 0)
        bRet = true;
      break;
    case CcVariant::EType::Float:
      if(m_Data.Float > 0)
        bRet = true;
      break;
    case CcVariant::EType::Double:
      if (m_Data.Double > 0)
        bRet = true;
      break;
    case CcVariant::EType::String:
      {
        bRet = (*m_Data.String).toBool(bOk);
      }
      break;
    case CcVariant::EType::DateTime:
      if (m_Data.DateTime != 0)
        bRet = true;
      break;
    case CcVariant::EType::Pointer:
      if (m_Data.Pointer)
        bRet = true;
      break;
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
    case CcVariant::EType::ByteArray:
      bRet = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      iRet = static_cast<int8>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      break;
    case CcVariant::EType::Int8:
      iRet = m_Data.i8Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Uint8:
      iRet = static_cast<int8>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      iRet = static_cast<int8>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      iRet = static_cast<int8>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      iRet = static_cast<int8>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      iRet = static_cast<int8>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      iRet = static_cast<int8>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      iRet = static_cast<int8>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      iRet = static_cast<int8>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      iRet = static_cast<int8>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      iRet = static_cast<int8>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      iRet = m_Data.String->toInt8(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      iRet = static_cast<int8>(m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<uint8>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = static_cast<uint8>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      uiRet = m_Data.ui8Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<uint8>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = static_cast<uint8>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<uint8>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = static_cast<uint8>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<uint8>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = static_cast<uint8>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<uint8>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      uiRet = static_cast<uint8>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      uiRet = static_cast<uint8>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->toUint8(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<uint8>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      iRet = static_cast<int16>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      iRet = 0;
      break;
    case CcVariant::EType::Int8:
      iRet = static_cast<int16>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      iRet = static_cast<int16>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      iRet = m_Data.i16Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Uint16:
      iRet = static_cast<int16>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      iRet = static_cast<int16>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      iRet = static_cast<int16>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      iRet = static_cast<int16>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      iRet = static_cast<int16>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      iRet = static_cast<int16>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      iRet = static_cast<int16>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      iRet = static_cast<int16>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      {
        iRet = m_Data.String->toInt16(&bSuccess);
      }
      break;
    case CcVariant::EType::DateTime:
      iRet = static_cast<int16>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<uint16>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = static_cast<uint16>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      uiRet = static_cast<uint16>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<uint16>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = m_Data.ui16Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<uint16>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = static_cast<uint16>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<uint16>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = static_cast<uint16>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<uint16>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      uiRet = static_cast<uint16>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      uiRet = static_cast<uint16>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->toUint16(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<uint16>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      iRet = static_cast<int32>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      iRet = 0;
      break;
    case CcVariant::EType::Int8:
      iRet = static_cast<int32>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      iRet = static_cast<int32>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      iRet = static_cast<int32>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      iRet = static_cast<int32>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      iRet = m_Data.i32Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Uint32:
      iRet = static_cast<int32>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      iRet = static_cast<int32>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      iRet = static_cast<int32>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      iRet = static_cast<int32>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      iRet = static_cast<int32>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      iRet = static_cast<int32>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      iRet = m_Data.String->toInt32(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      iRet = static_cast<int32>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<uint32>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = static_cast<uint32>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      uiRet = static_cast<uint32>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<uint32>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = static_cast<uint32>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<uint32>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = m_Data.ui32Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<uint32>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = static_cast<uint32>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<uint32>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      uiRet = static_cast<uint32>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      uiRet = static_cast<uint32>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->toUint32(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<uint32>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      iRet = static_cast<int64>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      iRet = 0;
      break;
    case CcVariant::EType::Int8:
      iRet = static_cast<int64>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      iRet = static_cast<int64>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      iRet = static_cast<int64>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      iRet = static_cast<int64>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      iRet = static_cast<int64>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      iRet = static_cast<int64>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      iRet = m_Data.i64Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Uint64:
      iRet = static_cast<int64>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      iRet = static_cast<int64>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      iRet = static_cast<int64>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      iRet = static_cast<int64>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      iRet = m_Data.String->toInt64(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      iRet = static_cast<int64>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<uint64>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = static_cast<uint64>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      uiRet = static_cast<uint64>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<uint64>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = static_cast<uint64>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<uint64>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = static_cast<uint64>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<uint64>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = m_Data.ui64Data;
      bSuccess = true;
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<uint64>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      uiRet = static_cast<uint64>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      uiRet = static_cast<uint64>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      {
        uiRet = m_Data.String->toUint64(&bSuccess);
      }
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<uint64>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      iRet = static_cast<int>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      iRet = 0;
      break;
    case CcVariant::EType::Int8:
      iRet = static_cast<int>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      iRet = static_cast<int>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      iRet = static_cast<int>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      iRet = static_cast<int>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      iRet = static_cast<int>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      iRet = static_cast<int>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      iRet = static_cast<int>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      iRet = static_cast<int>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      iRet = static_cast<int>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      iRet = static_cast<int>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      iRet = static_cast<int>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      {
        iRet = static_cast<int>(m_Data.String->toInt64(&bSuccess));
      }
      break;
    case CcVariant::EType::DateTime:
      iRet = static_cast<int>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<uint>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = static_cast<uint>(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      uiRet = static_cast<uint>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<uint>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = static_cast<uint>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<uint>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = static_cast<uint>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<uint>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = static_cast<uint>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<uint>(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      uiRet = static_cast<uint>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      uiRet = static_cast<uint>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      {
        uiRet = static_cast<uint>(m_Data.String->toUint64(&bSuccess));
      }
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<uint>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<size_t>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case CcVariant::EType::Uint8:
      uiRet = static_cast<size_t>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<size_t>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = static_cast<size_t>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<size_t>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = static_cast<size_t>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<size_t>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = static_cast<size_t>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<size_t>(m_Data.Size);
      bSuccess = true;
      break;
    case CcVariant::EType::Float:
      uiRet = static_cast<size_t>(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      uiRet = static_cast<size_t>(m_Data.Double);
      break;
    case CcVariant::EType::String:
      {
        uiRet = static_cast<size_t>(m_Data.String->toUint64(&bSuccess));
      }
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<size_t>( m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<float>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case CcVariant::EType::Uint8:
      uiRet = static_cast<float>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<float>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = static_cast<float>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<float>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = static_cast<float>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<float>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = static_cast<float>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<float>(m_Data.Size);
      bSuccess = true;
      break;
    case CcVariant::EType::Float:
      uiRet = m_Data.Float;
      bSuccess = true;
      break;
    case CcVariant::EType::Double:
      uiRet = static_cast<float>(m_Data.Double);
      bSuccess = true;
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->toFloat(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<float>(m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = static_cast<double>(m_Data.bData);
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case CcVariant::EType::Uint8:
      uiRet = static_cast<double>(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      uiRet = static_cast<double>(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      uiRet = static_cast<double>(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      uiRet = static_cast<double>(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      uiRet = static_cast<double>(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      uiRet = static_cast<double>(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      uiRet = static_cast<double>(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      uiRet = static_cast<double>(m_Data.Size);
      bSuccess = true;
      break;
    case CcVariant::EType::Float:
      uiRet = static_cast<double>(m_Data.Float);
      bSuccess = true;
      break;
    case CcVariant::EType::Double:
      uiRet = m_Data.Double;
      bSuccess = true;
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->toDouble(&bSuccess);
      break;
    case CcVariant::EType::DateTime:
      uiRet = static_cast<double>(m_Data.DateTime->getTimestampUs());
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
    case CcVariant::EType::StringList:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = (time_t)m_Data.bData;
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Int8:
      uiRet = m_Data.i8Data;
      break;
    case CcVariant::EType::Uint8:
      uiRet = (time_t)m_Data.ui8Data;
      break;
    case CcVariant::EType::Int16:
      uiRet = (time_t)m_Data.i16Data;
      break;
    case CcVariant::EType::Uint16:
      uiRet = (time_t)m_Data.ui16Data;
      break;
    case CcVariant::EType::Int32:
      uiRet = (time_t)m_Data.i32Data;
      break;
    case CcVariant::EType::Uint32:
      uiRet = (time_t)m_Data.ui32Data;
      break;
    case CcVariant::EType::Int64:
      uiRet = (time_t)m_Data.i64Data;
      break;
    case CcVariant::EType::Uint64:
      uiRet = (time_t)m_Data.ui64Data;
      break;
    case CcVariant::EType::Size:
      uiRet = (time_t)m_Data.Size;
      break;
    //case CcVariant::EType::String:
    //  {
    //    uiRet = (time_t)m_Data.String->toUint64(&bSuccess);
    //  }
    //  break;
    case CcVariant::EType::DateTime:
      uiRet = *m_Data.DateTime;
      bSuccess = true;
      break;
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Ip:
    case CcVariant::EType::String:
      bSuccess = false;
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return uiRet;
}

CcStringList CcVariant::getStringList(bool *bOk) const
{
  CcStringList sRet;
  bool bSuccess = true;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      if (m_Data.bData == false)
        sRet = CcGlobalStrings::Numbers::i0;
      else
        sRet = CcGlobalStrings::Numbers::i1;
      break;
    case CcVariant::EType::NoType:
      bSuccess = true;
      bSuccess=false;
      break;
    case CcVariant::EType::Int8:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      sRet = CcString::fromNumber(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      bSuccess = true;
      sRet = CcString::fromSize(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.Double);
      break;
    case CcVariant::EType::ByteArray:
      {
        bSuccess = true;
        sRet.append(*m_Data.ByteArray);
      }
      break;
    case CcVariant::EType::String:
      {
        bSuccess = true;
        sRet.append(*m_Data.String);
      }
      break;
    case CcVariant::EType::StringList:
      sRet = *m_Data.StringList;
      bSuccess = true;
      break;
    case CcVariant::EType::DateTime:
      sRet = CcString::fromNumber(m_Data.DateTime->getTimestampUs());
      bSuccess = true;
      break;
    case CcVariant::EType::Version:
      sRet = m_Data.Version->getVersionString();
      bSuccess = true;
      break;
    case CcVariant::EType::Uuid:
      sRet = m_Data.Uuid->getUuidString();
      bSuccess = true;
      break;
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Ip:
      bSuccess = false;
      break;
  }
  if(bOk != nullptr)
    *bOk = bSuccess;
  return sRet;
}

CcString CcVariant::getString(bool *bOk) const
{
  CcString sRet;
  bool bSuccess = true;
  // Check fo correct value in Storage
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      if (m_Data.bData == false)
        sRet = CcGlobalStrings::Numbers::i0;
      else
        sRet = "1";
      break;
    case CcVariant::EType::NoType:
      bSuccess = true;
      bSuccess=false;
      break;
    case CcVariant::EType::Int8:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i8Data);
      break;
    case CcVariant::EType::Uint8:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui8Data);
      break;
    case CcVariant::EType::Int16:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i16Data);
      break;
    case CcVariant::EType::Uint16:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui16Data);
      break;
    case CcVariant::EType::Int32:
      sRet = CcString::fromNumber(m_Data.i32Data);
      break;
    case CcVariant::EType::Uint32:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui32Data);
      break;
    case CcVariant::EType::Int64:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.i64Data);
      break;
    case CcVariant::EType::Uint64:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.ui64Data);
      break;
    case CcVariant::EType::Size:
      bSuccess = true;
      sRet = CcString::fromSize(m_Data.Size);
      break;
    case CcVariant::EType::Float:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.Float);
      break;
    case CcVariant::EType::Double:
      bSuccess = true;
      sRet = CcString::fromNumber(m_Data.Double);
      break;
    case CcVariant::EType::String:
      {
        bSuccess = true;
        sRet = *m_Data.String;
      }
      break;
    case CcVariant::EType::ByteArray:
      {
        bSuccess = true;
        sRet.append(*m_Data.ByteArray);
      }
      break;
    case CcVariant::EType::DateTime:
      sRet = CcString::fromNumber(m_Data.DateTime->getTimestampUs());
      bSuccess = true;
      break;
    case CcVariant::EType::Version:
      sRet = m_Data.Version->getVersionString();
      bSuccess = true;
      break;
    case CcVariant::EType::Uuid:
      sRet = m_Data.Uuid->getUuidString();
      bSuccess = true;
      break;
    case CcVariant::EType::Pointer:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Ip:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::ByteArray:
    {
      bSuccess = true;
      oRet = *m_Data.String;
      break;
    }
    case CcVariant::EType::String:
    {
      bSuccess = true;
      oRet = (*m_Data.String).getByteArray();
      break;
    }
    case CcVariant::EType::NoType:
    case CcVariant::EType::Switch:
    case CcVariant::EType::Bool:
    case CcVariant::EType::Int8:
    case CcVariant::EType::Uint8:
    case CcVariant::EType::Int16:
    case CcVariant::EType::Uint16:
    case CcVariant::EType::Int32:
    case CcVariant::EType::Uint32:
    case CcVariant::EType::Int64:
    case CcVariant::EType::Uint64:
    case CcVariant::EType::Size:
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
    case CcVariant::EType::DateTime:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Ip:
      bSuccess = false;
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
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::String:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Pointer:
    {
      bSuccess = true;
      oRet = m_Data.Pointer;
      break;
    }
    case CcVariant::EType::NoType:
    case CcVariant::EType::Bool:
    case CcVariant::EType::Switch:
    case CcVariant::EType::Int8:
    case CcVariant::EType::Uint8:
    case CcVariant::EType::Int16:
    case CcVariant::EType::Uint16:
    case CcVariant::EType::Int32:
    case CcVariant::EType::Uint32:
    case CcVariant::EType::Int64:
    case CcVariant::EType::Uint64:
    case CcVariant::EType::Size:
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
    case CcVariant::EType::DateTime:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::Ip:
      bSuccess = false;
      break;
  }
  if (bOk != nullptr)
    *bOk = bSuccess;
  return oRet;
}

CcVariantList CcVariant::getVariantList(bool *bOk) const
{
  if (m_eType == EType::VariantList)
  {
    if (bOk)
      *bOk = true;
    return *m_Data.VariantList;
  }
  else
  {
    CcVariantList oList;
    oList.append(*this);
    if (bOk)
      *bOk = false;
    return oList;
  }
}

CcVersion CcVariant::getVersion(bool *bOk) const
{
  if (m_eType == EType::Version)
  {
    if (bOk)
      *bOk = true;
    return *m_Data.Version;
  }
  if (m_eType == EType::String)
  {
    CcVersion oVersion;
    bool bSuccess = oVersion.setVersionString(*m_Data.String);
    if (bOk)
      *bOk = bSuccess;
    return *m_Data.Version;
  }
  else
  {
    if (bOk)
      *bOk = false;
    return CcVersion();
  }
}

CcUuid CcVariant::getUuid(bool *bOk) const
{
  if (m_eType == EType::Uuid)
  {
    if (bOk)
      *bOk = true;
    return *m_Data.Uuid;
  }
  if (m_eType == EType::String)
  {
    CcUuid oVersion;
    bool bSuccess = oVersion.setUuid(*m_Data.String);
    if (bOk)
      *bOk = bSuccess;
    return *m_Data.Uuid;
  }
  else
  {
    if (bOk)
      *bOk = false;
    return CcUuid();
  }
}

CcIp CcVariant::getIp(bool *bOk) const
{
  if (m_eType == EType::Ip)
  {
    if (bOk)
      *bOk = true;
    return *m_Data.Ip;
  }
  if (m_eType == EType::String)
  {
    CcIp oVersion;
    bool bSuccess = oVersion.setIp(*m_Data.String);
    if (bOk)
      *bOk = bSuccess;
    return *m_Data.Ip;
  }
  else
  {
    if (bOk)
      *bOk = false;
    return CcIp();
  }
}

#ifdef WINDOWS
VARIANT CcVariant::getWinVariant(VARENUM& winVariantType)
{
  // @todo implementation for Windows variant output
  VARIANT vRet;
  CCUNUSED(winVariantType);
  memset(&vRet, 0, sizeof(vRet));
  return vRet;
}
#endif

size_t CcVariant::writeData(void* pBuffer, size_t uiBufferSize) const
{
  size_t uiRet = SIZE_MAX;
  bool bCopy = true;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = 1;
      break;
    case CcVariant::EType::Int8:
      uiRet = 1;
      break;
    case CcVariant::EType::Uint8:
      uiRet = 1;
      break;
    case CcVariant::EType::Int16:
      uiRet = 2;
      break;
    case CcVariant::EType::Uint16:
      uiRet = 2;
      break;
    case CcVariant::EType::Int32:
      uiRet = 4;
      break;
    case CcVariant::EType::Uint32:
      uiRet = 4;
      break;
    case CcVariant::EType::Int64:
      uiRet = 8;
      break;
    case CcVariant::EType::Uint64:
      uiRet = 8;
      break;
    case CcVariant::EType::Size:
      uiRet = sizeof(size_t);
      break;
    case CcVariant::EType::Float:
      uiRet = sizeof(float);
      break;
    case CcVariant::EType::Double:
      uiRet = sizeof(double);
      break;
    case CcVariant::EType::DateTime:
      uiRet = sizeof(CcDateTime);
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->length();
      bCopy = false;
      if (pBuffer != nullptr)
      {
        if (uiBufferSize >= uiRet)
        {
          CcStatic::memcpy(pBuffer, m_Data.String->getCharString(), uiRet);
        }
      }
      break;
    case CcVariant::EType::ByteArray:
      uiRet = m_Data.ByteArray->size();
      bCopy = false;
      if (pBuffer != nullptr)
      {
        if (uiBufferSize >= uiRet)
        {
          m_Data.ByteArray->read(pBuffer, uiRet);
        }
      }
      break;
    case CcVariant::EType::Pointer:
      uiRet = sizeof(void*);
      break;
    case CcVariant::EType::Version:
      uiRet = sizeof(CcVersion);
      bCopy = false;
      if (pBuffer != nullptr)
      {
        if (uiBufferSize >= uiRet)
        {
          CcStatic::memcpy(pBuffer, m_Data.Version, uiRet);
        }
      }
      break;
    case CcVariant::EType::Uuid:
      uiRet = sizeof(CcUuid);
      bCopy = false;
      if (pBuffer != nullptr)
      {
        if (uiBufferSize >= uiRet)
        {
          CcStatic::memcpy(pBuffer, m_Data.Uuid, uiRet);
        }
      }
      break;
    case CcVariant::EType::Ip:
      uiRet = sizeof(CcIp);
      bCopy = false;
      if (pBuffer != nullptr)
      {
        if (uiBufferSize >= uiRet)
        {
          CcStatic::memcpy(pBuffer, m_Data.Ip, uiRet);
        }
      }
      break;
    case CcVariant::EType::VariantList:
    case CcVariant::EType::StringList:
      // do nothing here
      break;
  }
  if (bCopy   &&
      uiRet != SIZE_MAX &&
      pBuffer != nullptr)
  {
    if (uiBufferSize >= uiRet)
    {
      CcStatic::memcpy(pBuffer, &m_Data, uiRet);
    }
  }
  return uiRet;
}

size_t CcVariant::writeData(IIo& oIo) const
{
  size_t uiRet = SIZE_MAX;
  bool bCopy = true;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = 1;
      break;
    case CcVariant::EType::Int8:
      uiRet = 1;
      break;
    case CcVariant::EType::Uint8:
      uiRet = 1;
      break;
    case CcVariant::EType::Int16:
      uiRet = 2;
      break;
    case CcVariant::EType::Uint16:
      uiRet = 2;
      break;
    case CcVariant::EType::Int32:
      uiRet = 4;
      break;
    case CcVariant::EType::Uint32:
      uiRet = 4;
      break;
    case CcVariant::EType::Int64:
      uiRet = 8;
      break;
    case CcVariant::EType::Uint64:
      uiRet = 8;
      break;
    case CcVariant::EType::Size:
      uiRet = sizeof(size_t);
      break;
    case CcVariant::EType::Float:
      uiRet = sizeof(float);
      break;
    case CcVariant::EType::Double:
      uiRet = sizeof(double);
      break;
    case CcVariant::EType::DateTime:
      uiRet = sizeof(CcDateTime);
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->length();
      bCopy = false;
      oIo.write(m_Data.String->getCharString(), uiRet);
      break;
    case CcVariant::EType::ByteArray:
      uiRet = m_Data.ByteArray->size();
      bCopy = false;
      oIo.write(m_Data.ByteArray->getArray(), uiRet);
      break;
    case CcVariant::EType::Pointer:
      uiRet = sizeof(void*);
      break;
    case CcVariant::EType::Version:
      uiRet = sizeof(CcVersion);
      bCopy = false;
      oIo.write(m_Data.Version, uiRet);
      break;
    case CcVariant::EType::Uuid:
      uiRet = sizeof(CcUuid);
      bCopy = false;
      oIo.write(m_Data.Uuid, uiRet);
      break;
    case CcVariant::EType::Ip:
      uiRet = sizeof(CcIp);
      bCopy = false;
      oIo.write(m_Data.Ip, uiRet);
      break;
    case CcVariant::EType::VariantList:
    case CcVariant::EType::StringList:
      // Do nothing here
      break;
  }
  if (bCopy   &&
      uiRet != SIZE_MAX)
  {
    oIo.write(&m_Data, uiRet);
  }
  return uiRet;
}

size_t CcVariant::getWriteDataSize() const
{
  size_t uiRet = SIZE_MAX;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::NoType:
      uiRet = 0;
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
      uiRet = 1;
      break;
    case CcVariant::EType::Int8:
      uiRet = 1;
      break;
    case CcVariant::EType::Uint8:
      uiRet = 1;
      break;
    case CcVariant::EType::Int16:
      uiRet = 2;
      break;
    case CcVariant::EType::Uint16:
      uiRet = 2;
      break;
    case CcVariant::EType::Int32:
      uiRet = 4;
      break;
    case CcVariant::EType::Uint32:
      uiRet = 4;
      break;
    case CcVariant::EType::Int64:
      uiRet = 8;
      break;
    case CcVariant::EType::Uint64:
      uiRet = 8;
      break;
    case CcVariant::EType::Size:
      uiRet = sizeof(size_t);
      break;
    case CcVariant::EType::Float:
      uiRet = sizeof(float);
      break;
    case CcVariant::EType::Double:
      uiRet = sizeof(double);
      break;
    case CcVariant::EType::DateTime:
      uiRet = sizeof(CcDateTime);
      break;
    case CcVariant::EType::String:
      uiRet = m_Data.String->length();
      break;
    case CcVariant::EType::ByteArray:
      uiRet = m_Data.ByteArray->size();
      break;
    case CcVariant::EType::Pointer:
      uiRet = sizeof(void*);
      break;
    case CcVariant::EType::Version:
      uiRet = sizeof(CcVersion);
      break;
    case CcVariant::EType::Uuid:
      uiRet = sizeof(CcUuid);
      break;
    case CcVariant::EType::Ip:
      uiRet = sizeof(CcIp);
      break;
    case CcVariant::EType::VariantList:
    case CcVariant::EType::StringList:
      // Do nothing here;
      break;
  }
  return uiRet;
}

void CcVariant::set(bool bVal)
{
  if(m_eType != CcVariant::EType::Bool)
  {
    clear();
  }
  m_Data.bData = bVal;
  m_eType = CcVariant::EType::Bool;
}

void CcVariant::set(int8 i8Val)
{
  if(m_eType != CcVariant::EType::Int8)
  {
    clear();
  }
  m_Data.i8Data = i8Val;
  m_eType = CcVariant::EType::Int8;
}

void CcVariant::set(uint8 ui8Val)
{
  if(m_eType != CcVariant::EType::Uint8)
  {
    clear();
  }
  m_Data.ui8Data = ui8Val;
  m_eType = CcVariant::EType::Uint8;
}

void CcVariant::set(int16 i16Val)
{
  if(m_eType != CcVariant::EType::Uint16)
  {
    clear();
  }
  m_Data.i16Data = i16Val;
  m_eType = CcVariant::EType::Int16;
}

void CcVariant::set(uint16 ui16Val)
{
  if(m_eType != CcVariant::EType::Uint16)
  {
    clear();
  }
  m_Data.ui16Data = ui16Val;
  m_eType = CcVariant::EType::Uint16;
}

void CcVariant::set(int32 i32Val)
{
  if(m_eType != CcVariant::EType::Int32)
  {
    clear();
  }
  m_Data.i32Data = i32Val;
  m_eType = CcVariant::EType::Int32;
}

void CcVariant::set(uint32 ui32Val)
{
  if(m_eType != CcVariant::EType::Uint32)
  {
    clear();
  }
  m_Data.ui32Data = ui32Val;
  m_eType = CcVariant::EType::Uint32;
}

void CcVariant::set(int64 i64Val)
{
  if(m_eType != CcVariant::EType::Int64)
  {
    clear();
  }
  m_Data.i64Data = i64Val;
  m_eType = CcVariant::EType::Int64;
}

void CcVariant::set(uint64 ui64Val)
{
  if(m_eType != CcVariant::EType::Uint64)
  {
    clear();
  }
  m_Data.ui64Data = ui64Val;
  m_eType = CcVariant::EType::Uint64;
}

void CcVariant::set(float fVal)
{
  if(m_eType != CcVariant::EType::Float)
  {
    clear();
  }
  m_Data.Float = fVal;
  m_eType = CcVariant::EType::Float;
}

void CcVariant::set(double dVal)
{
  if (m_eType != CcVariant::EType::Double)
  {
    clear();
  }
  m_Data.Double = dVal;
  m_eType = CcVariant::EType::Double;
}

void CcVariant::set(const char* val)
{
  if (m_eType != CcVariant::EType::String)
  {
    clear();
    m_eType = CcVariant::EType::String;
    CCNEW(m_Data.String, CcString, val);
  }
  else
  {
    *m_Data.String = val;
  }
}

void CcVariant::set(const CcStringList& val)
{
  if (m_eType != CcVariant::EType::String)
  {
    clear();
    m_eType = CcVariant::EType::StringList;
    CCNEW(m_Data.StringList, CcStringList, val);
  }
  else
  {
    *m_Data.StringList = val;
  }
}

void CcVariant::set(const CcString& val)
{
  if (m_eType != CcVariant::EType::String)
  {
    clear();
    m_eType = CcVariant::EType::String;
    CCNEW(m_Data.String, CcString, val);
  }
  else
  {
    *m_Data.String = val;
  }
}

void CcVariant::set(const CcByteArray& val)
{
  if (m_eType != CcVariant::EType::ByteArray)
  {
    clear();
    m_eType = CcVariant::EType::ByteArray;
    CCNEW(m_Data.ByteArray, CcByteArray, val);
  }
  else
  {
    *m_Data.ByteArray = val;
  }
}

void CcVariant::set(const CcDateTime& val)
{
  if (m_eType != CcVariant::EType::DateTime)
  {
    clear();
    m_eType = CcVariant::EType::DateTime;
    CCNEW(m_Data.DateTime, CcDateTime, val);
  }
  else
  {
    *m_Data.DateTime = val;
  }
}

void CcVariant::set(const CcVariantList& val)
{
  if (m_eType != CcVariant::EType::VariantList)
  {
    clear();
    m_eType = CcVariant::EType::VariantList;
    CCNEW(m_Data.VariantList, CcVariantList, val);
  }
  else
  {
    *m_Data.VariantList = val;
  }
}

void CcVariant::set(const CcVersion& val)
{
  if (m_eType != CcVariant::EType::Version)
  {
    clear();
    m_eType = CcVariant::EType::Version;
    CCNEW(m_Data.Version, CcVersion, val);
  }
  else
  {
    *m_Data.Version = val;
  }
}

void CcVariant::set(const CcUuid& val)
{
  if (m_eType != CcVariant::EType::Uuid)
  {
    clear();
    m_eType = CcVariant::EType::Uuid;
    CCNEW(m_Data.Uuid, CcUuid, val);
  }
  else
  {
    *m_Data.Uuid = val;
  }
}

void CcVariant::set(const CcIp& val)
{
  if (m_eType != CcVariant::EType::Ip)
  {
    clear();
    m_eType = CcVariant::EType::Ip;
    CCNEW(m_Data.Ip, CcIp, val);
  }
  else
  {
    *m_Data.Ip = val;
  }
}

void CcVariant::set(void* val)
{
  if (m_eType != CcVariant::EType::Pointer)
  {
    clear();
  }
  m_eType = CcVariant::EType::Pointer;
  m_Data.Pointer = val;
}

void CcVariant::setSize(size_t uiSizeVal)
{
  if(m_eType != CcVariant::EType::Size)
  {
    clear();
  }
  m_Data.Size = uiSizeVal;
  m_eType = CcVariant::EType::Size;
}

bool CcVariant::isInt() const
{
  bool bRet = false;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Int8:
    case CcVariant::EType::Int16:
    case CcVariant::EType::Int32:
    case CcVariant::EType::Int64:
      bRet = true;
      break;
    case CcVariant::EType::Uint8:
      if(m_Data.ui8Data <= (UINT8_MAX >> 1))
        bRet = true;
      break;
    case CcVariant::EType::Uint16:
      if(m_Data.ui16Data <= (UINT16_MAX >> 1))
        bRet = true;
      break;
    case CcVariant::EType::Uint32:
      if(m_Data.ui32Data <= (UINT32_MAX >> 1))
        bRet = true;
      break;
    case CcVariant::EType::Uint64:
      if(m_Data.ui64Data <= (UINT64_MAX >> 1))
        bRet = true;
      break;
    case CcVariant::EType::NoType:
    case CcVariant::EType::Switch:
    case CcVariant::EType::Bool:
    case CcVariant::EType::Size:
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
    case CcVariant::EType::DateTime:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Ip:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::String:
      bRet = false;
      break;
  }
  return bRet;
}

bool CcVariant::isUint() const
{
  bool bRet = false;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
    case CcVariant::EType::Bool:
    case CcVariant::EType::Uint8:
    case CcVariant::EType::Uint16:
    case CcVariant::EType::Uint32:
    case CcVariant::EType::Uint64:
    case CcVariant::EType::Size:
      bRet = true;
      break;
    case CcVariant::EType::Int8:
      if(m_Data.i8Data > 0)
        bRet = true;
      break;
    case CcVariant::EType::Int16:
      if(m_Data.i16Data > 0)
        bRet = true;
      break;
    case CcVariant::EType::Int32:
      if(m_Data.i32Data > 0)
        bRet = true;
      break;
    case CcVariant::EType::Int64:
      if(m_Data.i64Data > 0)
        bRet = true;
      break;
    case CcVariant::EType::NoType:
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
    case CcVariant::EType::DateTime:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Ip:
    case CcVariant::EType::ByteArray:
    case CcVariant::EType::String:
      bRet = false;
      break;
  }
  return bRet;
}

bool CcVariant::isFloat() const
{
  bool bRet = false;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Double:
    case CcVariant::EType::Float:
      bRet = true;
      break;
    case CcVariant::EType::NoType:
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
    case CcVariant::EType::Uint8:
    case CcVariant::EType::Int8:
    case CcVariant::EType::Uint16:
    case CcVariant::EType::Int16:
    case CcVariant::EType::Uint32:
    case CcVariant::EType::Int32:
    case CcVariant::EType::Uint64:
    case CcVariant::EType::Int64:
    case CcVariant::EType::Size:
    case CcVariant::EType::DateTime:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Ip:
    case CcVariant::EType::String:
    case CcVariant::EType::ByteArray:
      bRet = false;
      break;
  }
  return bRet;
}

bool CcVariant::isString() const
{
  bool bRet = false;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::String:
      bRet = true;
      break;
    case CcVariant::EType::NoType:
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
    case CcVariant::EType::Uint8:
    case CcVariant::EType::Int8:
    case CcVariant::EType::Uint16:
    case CcVariant::EType::Int16:
    case CcVariant::EType::Uint32:
    case CcVariant::EType::Int32:
    case CcVariant::EType::Uint64:
    case CcVariant::EType::Int64:
    case CcVariant::EType::Size:
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
    case CcVariant::EType::DateTime:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::StringList:
    case CcVariant::EType::Ip:
    case CcVariant::EType::ByteArray:
      bRet = false;
      break;
  }
  return bRet;
}

bool CcVariant::isByteArray() const
{
  bool bRet = false;
  switch (m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::ByteArray:
      bRet = true;
      break;
    case CcVariant::EType::NoType:
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
    case CcVariant::EType::Uint8:
    case CcVariant::EType::Int8:
    case CcVariant::EType::Uint16:
    case CcVariant::EType::Int16:
    case CcVariant::EType::Uint32:
    case CcVariant::EType::Int32:
    case CcVariant::EType::Uint64:
    case CcVariant::EType::Int64:
    case CcVariant::EType::Size:
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
    case CcVariant::EType::DateTime:
    case CcVariant::EType::Pointer:
    case CcVariant::EType::Version:
    case CcVariant::EType::VariantList:
    case CcVariant::EType::Uuid:
    case CcVariant::EType::StringList:
    case CcVariant::EType::String:
    case CcVariant::EType::Ip:
      bRet = false;
      break;
  }
  return bRet;
}

bool CcVariant::convert(CcVariant::EType eType)
{
  bool bSuccess = false;
  switch (eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::Switch:
      CCFALLTHROUGH;
    case CcVariant::EType::Bool:
    {
      bool bConv = getBool(&bSuccess);
      if(bSuccess) set(bConv);
      break;
    }
    case CcVariant::EType::Int8:
    {
      int8 bConv = getInt8(&bSuccess);
      if (bSuccess) set(bConv);
      break;
    }
    case CcVariant::EType::Uint8:
    {
      uint8 oConv = getUint8(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Int16:
    {
      int16 oConv = getInt16(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Uint16:
    {
      uint16 oConv = getUint16(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Int32:
    {
      int32 oConv = getInt32(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Uint32:
    {
      uint32 oConv = getUint32(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Int64:
    {
      int64 oConv = getInt64(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Uint64:
    {
      uint64 oConv = getUint64(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Size:
    {
      size_t oConv = getSize(&bSuccess);
      if (bSuccess) setSize(oConv);
      break;
    }
    case CcVariant::EType::Float:
    {
      float oConv = getFloat(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::Double:
    {
      double oConv = getDouble(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::DateTime:
    {
      CcDateTime oConv = getTime(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    case CcVariant::EType::StringList:
    {
      CcStringList oConv = getStringList(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case CcVariant::EType::String:
    {
      CcString oConv = getString(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case CcVariant::EType::ByteArray:
    {
      CcByteArray oConv = getByteArray(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case CcVariant::EType::VariantList:
    {
      CcVariantList oConv = getVariantList(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case CcVariant::EType::Version:
    {
      CcVersion oConv = getVersion(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case CcVariant::EType::Uuid:
    {
      CcUuid oConv = getUuid(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case CcVariant::EType::Ip:
    {
      CcIp oConv = getIp(&bSuccess);
      if (bSuccess) set(oConv);
      break;
    }
    break;
    case CcVariant::EType::NoType:
    case CcVariant::EType::Pointer:
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
      default:
        // Force compiler warning here for unhandled types
        break;
      case CcVariant::EType::String:
        if (*oToCompare.m_Data.String == *m_Data.String)
          bSuccess = true;
        break;
      case CcVariant::EType::DateTime:
        if (*oToCompare.m_Data.DateTime == *m_Data.DateTime)
          bSuccess = true;
        break;
      case CcVariant::EType::ByteArray:
        if (*oToCompare.m_Data.ByteArray == *m_Data.ByteArray)
          bSuccess = true;
        break;
      case CcVariant::EType::Version:
        if (*oToCompare.m_Data.Version == *m_Data.Version)
          bSuccess = true;
        break;
      case CcVariant::EType::Uuid:
        if (*oToCompare.m_Data.Uuid == *m_Data.Uuid)
          bSuccess = true;
        break;
      case CcVariant::EType::Ip:
        if (*oToCompare.m_Data.Ip == *m_Data.Ip)
          bSuccess = true;
        break;
      case CcVariant::EType::NoType:
      case CcVariant::EType::Switch:
      case CcVariant::EType::Bool:
      case CcVariant::EType::Uint8:
      case CcVariant::EType::Int8:
      case CcVariant::EType::Uint16:
      case CcVariant::EType::Int16:
      case CcVariant::EType::Uint32:
      case CcVariant::EType::Int32:
      case CcVariant::EType::Uint64:
      case CcVariant::EType::Int64:
      case CcVariant::EType::Size:
      case CcVariant::EType::Float:
      case CcVariant::EType::Double:
        if (oToCompare.m_Data.ui64Data == m_Data.ui64Data)
          bSuccess = true;
        break;
      case CcVariant::EType::VariantList:
        if (*m_Data.VariantList == *oToCompare.m_Data.VariantList)
          bSuccess = true;
        break;
      case CcVariant::EType::StringList:
        if (*m_Data.StringList == *oToCompare.m_Data.StringList)
          bSuccess = true;
        break;
      case CcVariant::EType::Pointer:
        if (m_Data.Pointer == oToCompare.m_Data.Pointer)
          bSuccess = true;
        break;
    }
  }
  return bSuccess;
}

CcVariant& CcVariant::operator=(CcVariant&& oToMove)
{
  if (this != &oToMove)
  {
    clear();
    m_Data = oToMove.m_Data;
    m_eType = oToMove.m_eType;
    oToMove.m_eType = CcVariant::EType::NoType;
    oToMove.m_Data.i64Data = 0;
  }
  return *this;
}

CcVariant& CcVariant::operator=(const CcVariant& oToCopy)
{
  switch (oToCopy.m_eType)
  {
    default:
      // Force compiler warning here for unhandled types
      break;
    case CcVariant::EType::String:
      set(*oToCopy.m_Data.String);
      break;
    case CcVariant::EType::StringList:
      set(*oToCopy.m_Data.VariantList);
      break;
    case CcVariant::EType::ByteArray:
      set(*oToCopy.m_Data.ByteArray);
      break;
    case CcVariant::EType::VariantList:
      set(*oToCopy.m_Data.VariantList);
      break;
    case CcVariant::EType::Version:
      set(*oToCopy.m_Data.Version);
      break;
    case CcVariant::EType::Uuid:
      set(*oToCopy.m_Data.Uuid);
      break;
    case CcVariant::EType::Ip:
      set(*oToCopy.m_Data.Ip);
      break;
    case CcVariant::EType::NoType:
    case CcVariant::EType::Switch:
    case CcVariant::EType::Bool:
    case CcVariant::EType::Uint8:
    case CcVariant::EType::Int8:
    case CcVariant::EType::Uint16:
    case CcVariant::EType::Int16:
    case CcVariant::EType::Uint32:
    case CcVariant::EType::Int32:
    case CcVariant::EType::Uint64:
    case CcVariant::EType::Int64:
    case CcVariant::EType::Size:
    case CcVariant::EType::Float:
    case CcVariant::EType::Double:
      m_eType = oToCopy.getType();
      m_Data.ui64Data = oToCopy.m_Data.ui64Data;
      break;
    case CcVariant::EType::Pointer:
      set(oToCopy.m_Data.Pointer);
      break;
    case CcVariant::EType::DateTime:
      set(*oToCopy.m_Data.DateTime);
      break;
  }
  return *this;
}

#ifdef WINDOWS

#define CcVariant_SetElementWithObject(TYPE,OBJECT)               \
          {                                                       \
            SAFEARRAY* pSafeArray = V_ARRAY(&winVariant);         \
            TYPE element;                                         \
            long i = static_cast<long>(uiIndex);                  \
            if(S_OK == SafeArrayGetElement(pSafeArray, &i, (void*)&element)) \
            {  set(OBJECT(element)); bConversionOk = true; } \
          }
#define CcVariant_SetElement(TYPE) CcVariant_SetElementWithObject(TYPE,)

bool CcVariant::set(VARIANT& winVariant, VARENUM winVariantType, size_t uiIndex)
{
  bool bConversionOk = false;
  #ifdef _MSC_VER
    // Disable unhandled warning here
    #pragma warning(disable : 4061)
  #endif
  switch (winVariantType)
  {
    case VT_BOOL:
      if (uiIndex == SIZE_MAX)
        set(winVariant.boolVal);
      else
        CcVariant_SetElement(VARIANT_BOOL)
      break;
    case VT_I1:
      if (uiIndex == SIZE_MAX)
        set(winVariant.cVal);
      else
        CcVariant_SetElement(CHAR)
      break;
    case VT_UI1:
      if (uiIndex == SIZE_MAX)
        set(winVariant.bVal);
      else
        CcVariant_SetElement(BYTE)
      break;
    case VT_I2:
      if (uiIndex == SIZE_MAX)
        set(winVariant.iVal);
      else
        CcVariant_SetElement(SHORT)
      break;
    case VT_UI2:
      set(winVariant.uiVal);
      break;
    case VT_I4:
      set(winVariant.lVal);
      break;
    case VT_UI4:
      set(winVariant.ulVal);
      break;
    case VT_I8:
      set(winVariant.llVal);
      break;
    case VT_UI8:
      set(winVariant.ullVal);
      break;
    case VT_INT:
      set(winVariant.intVal);
      break;
    case VT_UINT:
      set(winVariant.uintVal);
      break;
    case VARENUM::VT_R4:
      set(winVariant.fltVal);
      break;
    case VARENUM::VT_R8:
      set(winVariant.dblVal);
      break;
    case VT_BSTR:
      if (uiIndex == SIZE_MAX)
        set(CcString(winVariant.bstrVal));
      else
        CcVariant_SetElementWithObject(BSTR, CcString)
      break;
    case VT_DATE:
    {
      SYSTEMTIME sysTime;
      FILETIME   fileTime;
      VariantTimeToSystemTime(winVariant.dblVal, &sysTime);
      SystemTimeToFileTime(&sysTime, &fileTime);
      CcDateTime oTime(0);
      SInt64Converter oConverter;
      oConverter.ui32.H = fileTime.dwHighDateTime;
      oConverter.ui32.L = fileTime.dwLowDateTime;
      oTime.setFiletime(oConverter.ui64);
      break;
    }
    default:
      if (IS_FLAG_SET(winVariantType, VT_ARRAY))
      {
        CcVariantList oVariants;
        CcVariant oVariant;
        size_t uiCounter = 0;
        winVariantType = (VARENUM)((INT)winVariantType - (INT)VARENUM::VT_ARRAY);
        while (oVariant.set(winVariant, winVariantType, uiCounter))
        {
          oVariants.append(oVariant);
          oVariant.clear();
          uiCounter++;
        }
        set(oVariants);
      }
      else
      {
        CCDEBUG("CcVariant: Unkown Conversion form WINVARIANT-type: " + CcString::fromNumber(winVariantType));
      }
  }
  #ifdef _MSC_VER
    // Reenable unhandled warning here
    #pragma warning(default : 4061)
  #endif
  return bConversionOk;
}
#endif

const CcString g_sNoType          ("notype");
const CcString g_sBool            ("bool");
const CcString g_sSwitch          ("switch");
const CcString g_sInt8            ("int8");
const CcString g_sUint8           ("uint8");
const CcString g_sInt16           ("int16");
const CcString g_sUint16          ("uint16");
const CcString g_sInt32           ("int32");
const CcString g_sUint32          ("uint32");
const CcString g_sInt64           ("int64");
const CcString g_sUint64          ("uint64");
const CcString g_sSize            ("size");
const CcString g_sFloat           ("float");
const CcString g_sDouble          ("double");
const CcString g_sDateTime        ("datetime");
const CcString g_sString          ("string");
const CcString g_sByteArray       ("bytearray");
const CcString g_sStringList      ("stringlist");
const CcString g_sPointer         ("pointer");
const CcString g_sVariantList     ("variantlist");
const CcString g_sVersion         ("version");
const CcString g_sUuid            ("uuid");
const CcString g_sIp              ("ip");

const CcString& CcVariant::getStringFromType(CcVariant::EType eType)
{
  switch (eType)
  {
    case EType::NoType:
      return g_sNoType;
    case EType::Bool:
      return g_sBool;
    case EType::Switch:
      return g_sSwitch;
    case EType::Int8:
      return g_sInt8;
    case EType::Uint8:
      return g_sUint8;
    case EType::Int16:
      return g_sInt16;
    case EType::Uint16:
      return g_sUint16;
    case EType::Int32:
      return g_sInt32;
    case EType::Uint32:
      return g_sUint32;
    case EType::Int64:
      return g_sInt64;
    case EType::Uint64:
      return g_sUint64;
    case EType::Size:
      return g_sSize;
    case EType::Float:
      return g_sFloat;
    case EType::Double:
      return g_sDouble;
    case EType::DateTime:
      return g_sDateTime;
    case EType::String:
      return g_sString;
    case EType::ByteArray:
      return g_sByteArray;
    case EType::StringList:
      return g_sStringList;
    case EType::Pointer:
      return g_sPointer;
    case EType::VariantList:
      return g_sVariantList;
    case EType::Version:
      return g_sNoType;
    case EType::Uuid:
      return g_sUuid;
    case EType::Ip:
      return g_sIp;
  }
  return CcGlobalStrings::Empty;
}

CcVariant::EType CcVariant::getTypeFromString(const CcString& sType)
{
  CcString sLower(sType.getLower());
  if(g_sNoType            .compare(sLower)) return EType::NoType     ;
  else if(g_sBool         .compare(sLower)) return EType::Bool       ;
  else if(g_sSwitch       .compare(sLower)) return EType::Switch     ;
  else if(g_sInt8         .compare(sLower)) return EType::Int8       ;
  else if(g_sUint8        .compare(sLower)) return EType::Uint8      ;
  else if(g_sInt16        .compare(sLower)) return EType::Int16      ;
  else if(g_sUint16       .compare(sLower)) return EType::Uint16     ;
  else if(g_sInt32        .compare(sLower)) return EType::Int32      ;
  else if(g_sUint32       .compare(sLower)) return EType::Uint32     ;
  else if(g_sInt64        .compare(sLower)) return EType::Int64      ;
  else if(g_sUint64       .compare(sLower)) return EType::Uint64     ;
  else if(g_sSize         .compare(sLower)) return EType::Size       ;
  else if(g_sFloat        .compare(sLower)) return EType::Float      ;
  else if(g_sDouble       .compare(sLower)) return EType::Double     ;
  else if(g_sDateTime     .compare(sLower)) return EType::DateTime   ;
  else if(g_sString       .compare(sLower)) return EType::String     ;
  else if(g_sByteArray    .compare(sLower)) return EType::ByteArray  ;
  else if(g_sStringList   .compare(sLower)) return EType::StringList ;
  else if(g_sPointer      .compare(sLower)) return EType::Pointer    ;
  else if(g_sVariantList  .compare(sLower)) return EType::VariantList;
  else if(g_sVersion      .compare(sLower)) return EType::Version    ;
  else if(g_sUuid         .compare(sLower)) return EType::Uuid       ;
  else if(g_sIp           .compare(sLower)) return EType::Ip         ;
  return EType::NoType;
}
