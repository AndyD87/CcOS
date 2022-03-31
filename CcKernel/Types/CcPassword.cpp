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
 * @brief     Implemtation of class CcPassword
 */
#include "CcPassword.h"
#include "Hash/CcMd5.h"
#include "Hash/CcSha256.h"
#include "CcGlobalStrings.h"

CcPassword::CcPassword( const CcPassword& oToCopy )
{
  operator=(oToCopy);
}

CcPassword::CcPassword( CcPassword&& oToMove )
{
  operator=(CCMOVE(oToMove));
}

CcPassword::CcPassword(const CcString& sPassword)
{
  setPassword(sPassword);
}

CcPassword::CcPassword(const CcString& sPassword, EHashType eType)
{
  setPassword(sPassword, eType);
}

CcPassword::~CcPassword()
{
  // Overwrite old string and than clean it.
  m_sPassword.clearSave();
}

CcPassword& CcPassword::operator=(CcPassword&& oToMove)
{
  if(this != &oToMove)
  {
    m_sPassword = CCMOVE(oToMove.m_sPassword);
    m_eType = CCMOVE(oToMove.m_eType);
  }
  return *this;
}

CcPassword& CcPassword::operator=(const CcPassword& oToCopy)
{
  setPassword(oToCopy.m_sPassword, oToCopy.m_eType);
  return *this;
}

bool CcPassword::operator==(const CcPassword& oToCompare) const
{
  bool bRet = false;
  bRet = (m_sPassword == oToCompare.m_sPassword && m_eType == oToCompare.m_eType);
  return bRet;
}

void CcPassword::setPassword(const CcString& sPassword, EHashType eType)
{
  // Overwrite old string and than clean it.
  m_sPassword.clearSave();
  m_sPassword = sPassword;
  // Keep passord save
  m_sPassword.mlock();
  m_eType = eType;
}

bool CcPassword::setType(EHashType eType)
{
  bool bRet = false;
  switch(m_eType)
  {
    case EHashType::Md5:
      CCFALLTHROUGH;
    case EHashType::Sha256:
      CCFALLTHROUGH;
    case EHashType::Unknown:
      bRet = true;
      m_eType = eType;
      break;
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
  }
  return sRet;
}

CcStatus CcPassword::generatePassword(const CcString& sPassword, EHashType eType)
{
  CcStatus oStatus;
  #ifndef GENERIC
    CcHash oHashObject;
    if(oHashObject.setHashType(eType))
    {
      oHashObject.generate(sPassword.getCharString(), sPassword.length());
      setPassword(oHashObject.getValue().getHexString(), eType);
    }
    else
    {
      oStatus = EStatus::NotSupported;
    }
  #else
    CCUNUSED(sPassword);
    CCUNUSED(eType);
  #endif
  return oStatus;
}
