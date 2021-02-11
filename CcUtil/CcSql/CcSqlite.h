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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSqlite
 */
#ifndef H_CcSqlite_H_
#define H_CcSqlite_H_

#include "CcBase.h"
#include "ISqlDatabase.h"
#include "CcSqlResult.h"
#include "CcSharedPointer.h"
#include <memory>

struct sqlite3;

/**
 * @brief Button for GUI Applications
 */
class CcSqlSHARED CcSqlite : public ISqlDatabase
{
public:
  /**
   * @brief Constructor
   */
  CcSqlite();

  /**
   * @brief Destructor
   */
  virtual ~CcSqlite();

  CcStatus open() override;
  CcSqlResult query(const CcString& queryString) override;
  CcStatus close() override;

  /**
   * @brief Test if table exists
   * @param sTableName: Table to query for
   * @return True if tabl was found in db
   */
  bool tableExists(const CcString& sTableName);

  /**
   * @brief Start transaction on Database
   * @return True if transaction successfully started
   */
  bool beginTransaction();

  /**
   * @brief Commit current transaction and write data to database
   * @return True if commit succeeded
   */
  bool commitTransaction();

  /**
   * @brief Finish transaction
   * @return True if transaction was closed successfully
   */
  bool endTransaction();

  /**
   * @brief Escape string to avoid database corrupten.
   *        All function signs will be escaped.
   * @param sToEscape: String to escape
   * @return Escaped string.
   */
  static CcString escapeString(const CcString& sToEscape);

  //! @return Get auto increment id from last INSERT
  uint64 getLastInsertId();
  //! @return True if foreigen key check is enabled
  bool getForeignKey();

  //! @param bEnable: True if foreigen key has to be enabled
  void setForeignKey(bool bEnable);

private:
  void addRow(void* pData, int argc, char **argv, char **azColName);
  static int sqliteCallback(void *pData, int argc, char **argv, char **azColName);

private:
  sqlite3* m_Sqlite = nullptr;
};

#endif // H_CcSqlite_H_
