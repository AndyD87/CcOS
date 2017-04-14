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
 * @page      CcISqlDatabase
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcISqlDatabase
 */
#ifndef CcISqlDatabase_H_
#define CcISqlDatabase_H_

#include "CcBase.h"
#include "CcSql.h"
#include "CcString.h"
#include "CcSqlResult.h"

/// Type of Database to connect to.
enum class ESqlDatabaseType
{
  Sqlite = 0,    //!< Type of Database is @ref CcSqlite
#ifdef WIN32
  WmiInterface,  //!< Type of Database is @ref CcWmiInterface
#endif
};

/**
 * @brief Abstract Class with functions all Databases must have.
 */
class CcSqlSHARED CcISqlDatabase
{
public:
  /**
   * @brief Constructor
   */
  CcISqlDatabase( void );

  /**
   * @brief Destructor
   */
  virtual ~CcISqlDatabase( void );

  /**
   * @brief Open connection to Database, Connection-Settings must be set previously
   *        Must be overloaded by inheriting Class.
   * @return true if connection successfully established
   */
  virtual bool open()=0;

  /**
   * @brief Close Connection to Database
   *        Must be overloaded by inheriting Class.
   * @return true if connection successfully closed
   */
  virtual bool close() = 0;

  /**
   * @brief Send a query to Database
   *        Must be overloaded by inheriting Class.
   * @param queryString: SQL-Request as String
   * @return Table with content
   */
  virtual CcSqlResult query(const CcString& queryString) = 0;

  /**
   * @brief Set the Connection Parameter like localhost or ip-address.
   *        Can be overloaded by inheriting Class, otherwise it stores
   *        it's value simply to member.
   * @param connection: Connection Value as String.
   */
   void setConnection(const CcString& connection);

  /**
   * @brief Set a Username to login to Database.
   * @param username: Username
   */
   void setUsername(const CcString& username);

  /**
   * @brief Set the Password to login to Databse.
   * @param password
   */
   void setPassword(const CcString& password);

  /**
   * @brief Set Name of Database to select at beginning.
   * @param database: Name of Database.
   */
  void setDatabase(const CcString& database);

  /**
   * @brief Get currently set connection of Database.
   * @return Reference to containg Connection string.
   */
  CcString& getConnection(void);

  /**
   * @brief Get currently set Username of Database.
   * @return Reference to containg Username string.
   */
  CcString& getUsername(void);

  /**
   * @brief Get currently set Password of Database.
   * @return Reference to containg Password string.
   */
  CcString& getPassword(void);

  /**
   * @brief Get currently set name of Database, wich is selected.
   * @return Reference to containg Database string.
   */
  CcString& getDatabase(void);

protected:
  CcString m_Connection; //!< Connection as String
  CcString m_Username;   //!< Username as String
  CcString m_Password;   //!< Password as String
  CcString m_Database;   //!< Database as String
};

#endif /* CcISqlDatabase_H_ */
