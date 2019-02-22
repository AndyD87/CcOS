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
 * @brief     Implementation of Class CcUser
 *
 * @todo: - use Password with a crypto lib
 */
#include "CcUser.h"

CcUser::CcUser( const CcString& Username ) :
  m_sUsername(Username)
{
}

bool CcUser::isUser(const CcString& Username) const
{
  return  m_sUsername.compare(Username, ESensitivity::CaseInsensitiv);
}

bool CcUser::login(const CcPassword& oPassword)
{
  bool bSuccess = false;
  if (m_oPassword == oPassword)
  {
    bSuccess = true;
  }
  return bSuccess;
}

void CcUser::setUserName(const CcString& sUserName)
{
  m_sUsername = sUserName;
}

void CcUser::setPassword(const CcPassword& oPassword)
{
  m_oPassword = oPassword;
}

void CcUser::setPassword(const CcString& sPassword, EHashType eType)
{
  m_oPassword = CcPassword(sPassword, eType);
}

void CcUser::setMail(const CcString& sMail)
{
  m_sMail = sMail;
}

void CcUser::setHomeDir(const CcString& sHomeDir)
{
  m_sHomeDir = sHomeDir;
}

const CcString& CcUser::getUserName(void) const
{
  return m_sUsername;
}

const CcString& CcUser::getMail(void) const
{
  return m_sMail;
}

const CcString& CcUser::getHomeDir(void) const
{
  return m_sHomeDir;
}

bool CcUser::operator==(const CcUser& oToCompare) const
{
  bool bRet = false;
  if (m_sUsername == oToCompare.m_sUsername &&
      m_oPassword == oToCompare.m_oPassword)
  {
    bRet = true;
  }

  return bRet;
}
