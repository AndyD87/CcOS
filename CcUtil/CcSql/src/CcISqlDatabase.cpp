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
 * @brief     Implementation of Class CcISqlDatabase
 */
#include "CcISqlDatabase.h"
#include "CcKernel.h"
#include "CcEvent.h"
#include "CcEventHandler.h"

CcISqlDatabase::CcISqlDatabase(void )
{
  CcKernel::getShutdownHandler().add(NewCcEvent(CcISqlDatabase, void, CcISqlDatabase::shutdownEvent, this));
}

CcISqlDatabase::~CcISqlDatabase(void )
{
  CcKernel::getShutdownHandler().removeObject(this);
}

void CcISqlDatabase::setConnection(const CcString& connection)
{
  m_Connection = connection;
}

void CcISqlDatabase::setUsername(const CcString&  username)
{
  m_Username = username;
}

void CcISqlDatabase::setPassword(const CcString&  password)
{
  m_Password = password;
}

void CcISqlDatabase::setDatabase(const CcString&  database)
{
  m_Database = database;
}

CcString& CcISqlDatabase::getConnection(void)
{
  return m_Connection;
}

CcString& CcISqlDatabase::getUsername(void)
{
  return m_Username;
}

CcString& CcISqlDatabase::getPassword(void)
{
  return m_Password;
}

CcString& CcISqlDatabase::getDatabase(void)
{
  return m_Database;
}

void CcISqlDatabase::shutdownEvent(void* pParam)
{
  CCUNUSED(pParam);
  if (m_bCloseOnExit)
  {
    close();
  }
}
