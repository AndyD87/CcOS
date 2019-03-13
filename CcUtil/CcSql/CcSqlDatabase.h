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
 * @page      CcSql
 * @subpage   CcSqlDatabase
 *
 * @page      CcSqlDatabase
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSqlDatabase
 */
#ifndef _CcSqlDatabase_H_
#define _CcSqlDatabase_H_

#include "CcBase.h"
#include "CcSql.h"
#include "ISqlDatabase.h"

/**
 * @brief Database Class for Handling all Known Databases defined in eSQLDatabaseType
 */
class CcSqlSHARED CcSqlDatabase : public ISqlDatabase
{
public:
  /**
   * @brief Constructor
   */
  CcSqlDatabase();

  /**
   * @brief Constructor
   */
  CcSqlDatabase(ESqlDatabaseType type);

  /**
   * @brief Constructor
   */
  CcSqlDatabase( ISqlDatabase *fromAbstract);

  /**
   * @brief Destructor
   */
  virtual ~CcSqlDatabase();

  /**
   * @brief Open the previously set connection to Database.
   * @return true if connection successfully established
   */
  inline CcStatus open()
    { return m_pDatabase->open(); }

  /**
   * @brief Close connection to database and clean up if required.
   * @return true if connection successfully closed
   */
  inline CcStatus close()
    { return m_pDatabase->close(); }

  /**
   * @brief Send a SQL-Query to Database
   * @param queryString: Query as String
   * @return Return value is a @ref CcSqlResult Table with all
   *         data from query.
   *          If Request Failed CcSqlResult is empty and getError() is !=0
   */
  inline CcSqlResult query(const CcString& queryString)
    { return m_pDatabase->query(queryString); }

  /**
   * @brief Set the Connection Parameter like localhost or ip-address.
   * @param connection: Connection Value as String.
   */
  inline void setConnection(const CcString& connection)
    { m_pDatabase->setConnection(connection); }

  /**
   * @brief Set a Username to login to Database.
   * @param username: Username
   */
  inline void setUsername(const CcString& username)
    { m_pDatabase->setUsername(username); }

  /**
   * @brief Set the Password to login to Databse.
   * @param password
   */
  inline void setPassword(const CcString& password)
    { m_pDatabase->setPassword(password); }

  /**
   * @brief Set Name of Database to select at beginning.
   * @param database: Name of Database.
   */
  inline void setDatabase(const CcString& database)
    { m_pDatabase->setDatabase(database); }

  /**
   * @brief Get currently set connection of Database.
   * @return Reference to containg Connection string.
   */
  inline CcString& getConnection()
    { return m_pDatabase->getConnection(); }

  /**
   * @brief Get currently set Username of Database.
   * @return Reference to containg Username string.
   */
  inline CcString& getUsername()
    { return m_pDatabase->getUsername(); }

  /**
   * @brief Get currently set Password of Database.
   * @return Reference to containg Password string.
   */
  inline CcString& getPassword()
    { return m_pDatabase->getPassword(); }

  /**
   * @brief Get currently set name of Database, wich is selected.
   * @return Reference to containg Database string.
   */
  inline CcString& getDatabase()
    { return m_pDatabase->getDatabase(); }

public:
  ISqlDatabase* m_pDatabase = nullptr; //!< Database linked to on constructor.
};

#endif /* _CcSqlDatabase_H_ */
