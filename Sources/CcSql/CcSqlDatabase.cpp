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
 * @brief     Implementation of Class CcSQL
 */
#include "CcSqlDatabase.h"
#ifdef CCOS_THIRDPARTY_SQLITE
  #include "CcSqlite.h"
#endif
#ifdef WINDOWS
#include "CcWmiInterface.h"
#endif

CcSqlDatabase::CcSqlDatabase()
{
}

CcSqlDatabase::CcSqlDatabase(ESqlDatabaseType type)
{
  switch(type)
  {
    case ESqlDatabaseType::Sqlite:
#ifdef CCOS_THIRDPARTY_SQLITE
      CCNEW(m_pDatabase, CcSqlite);
#else
      m_pDatabase = nullptr;
#endif
      break;
#if defined(WINDOWS) && !defined(__GNUC__)
    case ESqlDatabaseType::WmiInterface:
      CCNEW(m_pDatabase, CcWmiInterface);
#else
      m_pDatabase = nullptr;
#endif
  }
}

CcSqlDatabase::CcSqlDatabase(ISqlDatabase *fromAbstract)
{
  m_pDatabase = fromAbstract;
}

CcSqlDatabase::~CcSqlDatabase()
{
}
