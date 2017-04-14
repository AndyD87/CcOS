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
 * @brief     Implementation of Class CcUserList
 */
#include "CcUserList.h"

CcUserList::CcUserList( void )
{
}

CcUserList::CcUserList(const CcUserList& oToCopy)
{
  append(oToCopy);
  m_CurrentUser = oToCopy.m_CurrentUser;
}


CcUserList::~CcUserList( void )
{
}

CcUser* CcUserList::findUser(const CcString& Username)
{
  CcUser *pRet = nullptr;
  for (size_t i = 0; i < size(); i++)
  {
    if (at(i)->getUserName() == Username)
    {
      pRet = at(i);
    }
  }
  return pRet;
}

CcUser* CcUserList::findUserPassword(const CcString& Username, const CcString& Password)
{
  CcUser *pRet = nullptr;
  for (size_t i = 0; i < size(); i++)
  {
    if (at(i)->isUser(Username) && at(i)->login(Password))
    {
      pRet = at(i);
    }
  }
  return pRet;
}

bool CcUserList::setCurrentUser(const CcString& Username)
{
  CcUser* tempUser = findUser(Username);
  if (tempUser != nullptr)
  {
    m_CurrentUser = tempUser;
    return true;
  }
  return false;
}