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
 * @brief     Implemtation of class CcPassword
 */
#include "CcPassword.h"
#include "Hash/CcSha256.h"
#include "CcGlobalStrings.h"

CcPassword::CcPassword(void )
{
}

CcPassword::CcPassword( const CcPassword& oToCopy )
{
  operator=(oToCopy);
}

CcPassword::CcPassword( CcPassword&& oToMove )
{
  operator=(std::move(oToMove));
}

CcPassword::~CcPassword(void )
{
}

CcPassword::CcPassword(const CcString& sPassword)
{
  m_sPassword = sPassword;
}

CcPassword::CcPassword(const CcString& sPassword, EHashType eType)
{
  setPassword(sPassword, eType);
}

void CcPassword::setPassword(const CcString& sPassword, EHashType eType)
{
  m_sPassword = sPassword;
  m_eType = eType;
}

bool CcPassword::setType(EHashType eType)
{
  bool bRet = false;
  if (m_eType == EHashType::Unknown)
  {
    bRet = true;
    m_eType = eType;
  }
  return bRet;
}

bool CcPassword::setType(const CcString& sType)
{
  bool bRet;
  m_eType = getTypeByString(sType, bRet);
  return setType(m_eType);
}

EHashType CcPassword::getTypeByString(const CcString& sType, bool& bConvOk)
{
  EHashType eType = EHashType::Unknown;
  bConvOk = true;
  if (CcGlobalStrings::Types::Hash::Md5 == sType.getLower())
  {
    eType = EHashType::Md5;
  }
  else if (CcGlobalStrings::Types::Hash::Sha256 == sType.getLower())
  {
    eType = EHashType::Sha256;
  }
  else
  {
    eType = EHashType::Unknown;
    bConvOk = false;
  }
  return eType;
}

CcString CcPassword::getTypeAsString(EHashType eType)
{
  CcString sRet;
  switch (eType)
  {
    case EHashType::Md5:
      sRet = CcGlobalStrings::Types::Hash::Md5;
      break;
    case EHashType::Sha256:
      sRet = CcGlobalStrings::Types::Hash::Sha256;
      break;
    default:
      sRet = "ClearText";
  }
  return sRet;
}

CcPassword& CcPassword::operator=(CcPassword&& oToMove)
{
  if(this != &oToMove)
  {
    m_sPassword = std::move(oToMove.m_sPassword);
    m_eType = std::move(oToMove.m_eType);
  }
  return *this;
}

CcPassword& CcPassword::operator=(const CcPassword& oToCopy)
{
  m_sPassword = oToCopy.m_sPassword;
  m_eType = oToCopy.m_eType;
  return *this;
}

bool CcPassword::operator==(const CcPassword& oToCompare) const
{
  bool bRet = false;
  bRet = m_sPassword == oToCompare.m_sPassword;
  return bRet;
}
