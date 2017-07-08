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
 * @subpage   CcSqlite
 *
 * @page      CcSqlite
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSqlite
 */
#ifndef CcSqlite_H_
#define CcSqlite_H_

#include "CcBase.h"
#include "CcISqlDatabase.h"
#include "CcSqlResult.h"
#include "CcSharedPointer.h"
#include <memory>

struct sqlite3;

/**
 * @brief Button for GUI Applications
 */
class CcSqlSHARED CcSqlite : public CcISqlDatabase
{
public:
  /**
   * @brief Constructor
   */
  CcSqlite( void );

  /**
   * @brief Destructor
   */
  virtual ~CcSqlite( void );

  bool open() override;
  CcSqlResult query(const CcString& queryString) override;
  bool close() override;
  bool tableExists(const CcString& sTableName);

  bool beginTransaction();
  bool commitTransaction();
  bool endTransaction();

  static CcString escapeString(const CcString& sToEscape);

  bool getForeignKey();
  void setForeignKey(bool bEnable);

  size_t getLastInsertId();

private:
  void addRow(void* pData, int argc, char **argv, char **azColName);
  static int sqliteCallback(void *pData, int argc, char **argv, char **azColName);

private:
  sqlite3* m_Sqlite;
};

#endif /* CcSqlite_H_ */
