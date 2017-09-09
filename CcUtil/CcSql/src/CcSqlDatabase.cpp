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
 * @file      CcSqlDatabase
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcSQL
 */
#include "CcSqlDatabase.h"
#ifdef CCOS_THIRDPARTY_SQLITE
  #include "CcSqlite.h"
#endif
#ifdef WIN32
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
      m_pDatabase = new CcSqlite; 
      CCMONITORNEW(m_pDatabase);
#else
      m_pDatabase = nullptr;
#endif
      break;
#ifdef WIN32
    case ESqlDatabaseType::WmiInterface:
      m_pDatabase = new CcWmiInterface; 
      CCMONITORNEW(m_pDatabase);
#else
      m_pDatabase = nullptr;
#endif
  }
}

CcSqlDatabase::CcSqlDatabase(CcISqlDatabase *fromAbstract)
{
  m_pDatabase = fromAbstract;
}

CcSqlDatabase::~CcSqlDatabase( void )
{
}
