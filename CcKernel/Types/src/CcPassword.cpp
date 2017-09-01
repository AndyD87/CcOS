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
 * @brief     Implemtation of class CcPassword
 */
#include "CcPassword.h"
#include "Hash/CcSha256.h"

namespace CcPasswordStrings
{
  const char ClearText[]  = "ClearText";
  const char SHA256[]     = "SHA256";
  const char SHA512[]     = "SHA512";
  const char MD5[]        = "MD5";
}

CcPassword::CcPassword( void )
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

CcPassword::~CcPassword( void )
{
}

CcPassword::CcPassword(const CcString& sPassword)
{
  m_sPassword = sPassword;
}

CcPassword::CcPassword(const CcString& sPassword, EPasswordType eType)
{
  setPassword(sPassword, eType);
}

void CcPassword::setPassword(const CcString& sPassword, EPasswordType eType)
{
  m_sPassword = sPassword;
  m_eType = eType;
}

bool CcPassword::setType(EPasswordType eType)
{
  bool bRet = false;
  if (m_eType == EPasswordType::ClearText)
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

EPasswordType CcPassword::getTypeByString(const CcString& sType, bool& bConvOk)
{
  EPasswordType eType = EPasswordType::ClearText;
  bConvOk = true;
  if (sType.compare(CcPasswordStrings::ClearText, ESensitivity::CaseInsensitiv))
  {
    eType = EPasswordType::ClearText;
  }
  else if (sType.compare(CcPasswordStrings::MD5, ESensitivity::CaseInsensitiv))
  {
    eType = EPasswordType::MD5;
  }
  else if (sType.compare(CcPasswordStrings::SHA256, ESensitivity::CaseInsensitiv))
  {
    eType = EPasswordType::SHA256;
  }
  else if (sType.compare(CcPasswordStrings::SHA512, ESensitivity::CaseInsensitiv))
  {
    eType = EPasswordType::SHA512;
  }
  else
  {
    bConvOk = false;
  }
  return eType;
}

CcString CcPassword::getTypeAsString(EPasswordType eType)
{
  CcString sRet;
  switch (eType)
  {
    case EPasswordType::MD5:
      sRet = CcPasswordStrings::MD5;
      break;
    case EPasswordType::SHA256:
      sRet = CcPasswordStrings::SHA256;
      break;
    case EPasswordType::SHA512:
      sRet = CcPasswordStrings::SHA512;
      break;
    default:
      sRet = "ClearText";
  }
  return sRet;
}

CcPassword& CcPassword::operator=(const CcPassword& oToCopy)
{
  m_sPassword = oToCopy.m_sPassword;
  m_eType = oToCopy.m_eType;
  return *this;
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

bool CcPassword::operator==(const CcPassword& oToCompare) const
{
  bool bRet = false;
  bRet = m_sPassword == oToCompare.m_sPassword;
  return bRet;
}
