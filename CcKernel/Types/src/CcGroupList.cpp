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
 * @brief     Implementation of Class CcGroupList
 */
#include "CcGroupList.h"

CcGroupList::CcGroupList( void )
{
}

CcGroupList::CcGroupList(const CcGroupList& oToCopy) :
  CcList<CcGroup>(oToCopy)
{
  m_CurrentUser = oToCopy.m_CurrentUser;
}


CcGroupList::~CcGroupList( void )
{
}

CcGroup& CcGroupList::findGroup(const CcString& Username)
{
  for (size_t i = 0; i < size(); i++)
  {
    if (at(i).getName() == Username)
    {
      return at(i);
    }
  }
  return CCNULLREF(CcGroup);
}


const CcGroup& CcGroupList::findGroup(const CcString& Username) const
{
  CcGroupHandle pRet = nullptr;
  for (size_t i = 0; i < size(); i++)
  {
    if (at(i).getName() == Username)
    {
      return at(i);
    }
  }
  return CCNULLREF(CcGroup);
}
