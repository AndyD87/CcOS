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
 * @brief     Implementation of Class ISqlDatabase
 */
#include "ISqlDatabase.h"
#include "CcKernel.h"
#include "CcEvent.h"
#include "CcEventHandler.h"

ISqlDatabase::ISqlDatabase()
{
  CcKernel::getShutdownHandler().append(NewCcEventType(ISqlDatabase, void, ISqlDatabase::shutdownEvent, this));
}

ISqlDatabase::~ISqlDatabase()
{
  CcKernel::getShutdownHandler().removeObject(this);
}

void ISqlDatabase::setConnection(const CcString& connection)
{
  m_Connection = connection;
}

void ISqlDatabase::setUsername(const CcString&  username)
{
  m_Username = username;
}

void ISqlDatabase::setPassword(const CcString&  password)
{
  m_Password = password;
}

void ISqlDatabase::setDatabase(const CcString&  database)
{
  m_Database = database;
}

CcString& ISqlDatabase::getConnection()
{
  return m_Connection;
}

CcString& ISqlDatabase::getUsername()
{
  return m_Username;
}

CcString& ISqlDatabase::getPassword()
{
  return m_Password;
}

CcString& ISqlDatabase::getDatabase()
{
  return m_Database;
}

void ISqlDatabase::shutdownEvent(void* pParam)
{
  CCUNUSED(pParam);
  if (m_bCloseOnExit)
  {
    close();
  }
}
