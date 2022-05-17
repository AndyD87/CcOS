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
 * @brief     Class CcHash
 */
#include "Hash/CcHash.h"
#include "Hash/CcCrc32.h"
#include "Hash/CcMd5.h"
#include "Hash/CcSha256.h"
#include "Hash/CcSqlEnDecode.h"
#include "CcGlobalStrings.h"
#include "CcStatic.h"

CcHash::CcHash(EHashType eHashType) :
  m_eHashType(eHashType),
  m_pHashObject(nullptr)
{
  setHashType(eHashType);
}

CcHash::~CcHash()
{
  CCDELETE(m_pHashObject);
}

CcHash& CcHash::generate(const void* pData, size_t uiSize)
{
  if (m_pHashObject != nullptr)
  {
    m_pHashObject->generate(pData, uiSize);
  }
  return *this;
}

CcHash& CcHash::append(const void *pData, size_t uiSize)
{
  if (m_pHashObject != nullptr)
  {
    m_pHashObject->append(pData, uiSize);
  }
  return *this;
}

CcHash& CcHash::finalize(const void* pData, size_t uiSize)
{
  if (m_pHashObject != nullptr)
  {
    m_pHashObject->finalize(pData, uiSize);
  }
  return *this;
}

const CcByteArray& CcHash::getValue()
{
  if (m_pHashObject != nullptr)
  {
    return m_pHashObject->getValue();
  }
  return CcStatic::getConstNullRef<CcByteArray>();
}

IHash& CcHash::setKey(const void* pData, size_t uiSize)
{
  if(m_pHashObject)
    m_pHashObject->setKey(pData, uiSize);
  return *this;
}

const CcByteArray& CcHash::encode(const void* pData, size_t uiSize)
{
  if(m_pHashObject)
    return m_pHashObject->encode(pData, uiSize);
  return CcByteArray::getEmpty();
}

const CcByteArray &CcHash::decode(const void* pData, size_t uiSize)
{
  if(m_pHashObject)
    return m_pHashObject->decode(pData, uiSize);
  return CcByteArray::getEmpty();
}

bool CcHash::setHashType(EHashType eHashType)
{
  bool bRet = false;
  CCDELETE(m_pHashObject);
  switch (eHashType)
  {
    case EHashType::Crc32:
      CCNEW(m_pHashObject, CcCrc32);
      m_eHashType = eHashType;
      break;
    case EHashType::Md5:
      CCNEW(m_pHashObject, CcMd5);
      m_eHashType = eHashType;
      break;
    case EHashType::Sha256:
      CCNEW(m_pHashObject, CcSha256);
      m_eHashType = eHashType;
      break;
    case EHashType::SqlEnDecode:
      CCNEW(m_pHashObject, CcSqlEnDecode);
      m_eHashType = eHashType;
      break;
    case EHashType::Unknown:
      break;
  }
  if(m_pHashObject)
    bRet = true;
  return bRet;
}

bool CcHash::setHashType(const CcString& sHashType)
{
  bool bRet = false;
  if (CcGlobalStrings::Types::Hash::Crc32 == sHashType.getLower())
  {
    setHashType(EHashType::Crc32);
  }
  else if (CcGlobalStrings::Types::Hash::Md5 == sHashType.getLower())
  {
    setHashType(EHashType::Md5);
  }
  else if (CcGlobalStrings::Types::Hash::Sha256 == sHashType.getLower())
  {
    setHashType(EHashType::Sha256);
  }
  return bRet;
}
