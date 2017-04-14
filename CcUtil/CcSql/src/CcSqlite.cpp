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
 * @brief     Implementation of Class CcSqlite
 */
#include "CcSqlite.h"
#include "CcKernel.h"
#include "stdio.h"
#include "sqlite3.h"

typedef struct
{
  CcSqlite* pTargetDatabase;
  bool bFirstRequest;
  CcSqlResult oResult;
}CcSqliteRequest;

CcSqlite::CcSqlite( void )
{
}

CcSqlite::~CcSqlite( void )
{
}

bool CcSqlite::open()
{
  int iState;
  sqlite3* pSqlite;
  iState = sqlite3_open(m_Database.getCharString(), &pSqlite);
  if( iState != SQLITE_OK )
  {
    CCDEBUG(CcString("Can't open database: ") + sqlite3_errmsg((sqlite3*) m_Sqlite));
    sqlite3_close((sqlite3*) m_Sqlite);
    return(false);
  }
  else
  {
    m_Sqlite = pSqlite;
    // default enable foreign key
    if (getForeignKey() == false)
    {
      setForeignKey(true);
    }
    return true;
  }
}

CcSqlResult CcSqlite::query(const CcString& queryString)
{
  int iState;
  char *zErrMsg = 0;
  CcSqliteRequest sRequest;
  sRequest.bFirstRequest = true;
  sRequest.pTargetDatabase = this;
  iState = sqlite3_exec((sqlite3*) m_Sqlite, queryString.getCharString(), &CcSqlite::sqliteCallback, (void*) &sRequest, &zErrMsg);
  if( iState != SQLITE_OK )
  {
    CcString sErrorMsg(zErrMsg);
    sRequest.oResult.setErrorCode(iState);
    sRequest.oResult.setErrorMessage(sErrorMsg);
  }
  else
  {
    sRequest.oResult.setLastInsertId(getLastInsertId());
  }
  return sRequest.oResult;
}

bool CcSqlite::close(
  )
{
  sqlite3_close((sqlite3*) m_Sqlite);
  return false;
}

bool CcSqlite::tableExists(const CcString& sTableName)
{
  bool bRet = false;
  CcString sQuery = "PRAGMA table_info(`";
  sQuery << sTableName << "`)";
  CcSqlResult sResult = query(sQuery);
  if (sResult.size() > 0)
    bRet = true;
  return bRet;
}

bool CcSqlite::beginTransaction()
{
  CcString sQuery = "BEGIN TRANSACTION";
  CcSqlResult sResult = query(sQuery);
  return !sResult.error();
}

bool CcSqlite::commitTransaction()
{
  CcString sQuery = "COMMIT TRANSACTION";
  CcSqlResult sResult = query(sQuery);
  return !sResult.error();
}

bool CcSqlite::endTransaction()
{
  CcString sQuery = "END TRANSACTION";
  CcSqlResult sResult = query(sQuery);
  return !sResult.error();
}

CcString CcSqlite::escapeString(const CcString& sToEscape)
{
  char *pString = sqlite3_mprintf("%q", sToEscape.getCharString());
  CcString sRet = pString;
  sqlite3_free(pString);
  return sRet;
}

void CcSqlite::setForeignKey(bool bEnable)
{
  if (bEnable)
  {
    query("PRAGMA foreign_keys = ON;");
  }
  else
  {
    query("PRAGMA foreign_keys = OFF;");
  }
}

bool CcSqlite::getForeignKey()
{
  bool bRet = false;
  CcSqlResult oResult = query("PRAGMA foreign_keys;");
  if (oResult.size() > 0)
  {
    bRet = oResult[0][0].getBool();
  }
  return bRet;
}

size_t CcSqlite::getLastInsertId()
{
  return sqlite3_last_insert_rowid((sqlite3*)m_Sqlite);
}

int CcSqlite::sqliteCallback(void *pData, int argc, char **argv, char **azColName)
{
  ((CcSqliteRequest*) pData)->pTargetDatabase->addRow(pData, argc, argv, azColName);
  return 0;
}

void CcSqlite::addRow(void* pData, int argc, char **argv, char **azColName)
{
  CcSqliteRequest* pRequest = static_cast<CcSqliteRequest*>(pData);
  if (argc > 0)
  {
    if (pRequest->bFirstRequest == true)
    {
      pRequest->bFirstRequest=false;
      pRequest->oResult.setColumnCount(argc);
      for (int i = 0; i < argc; i++)
      {
        pRequest->oResult.setColumnName(i, azColName[i]);
      }
    }
    CcTableRow newRow(pRequest->oResult);
    for (int i = 0; i < argc; i++)
    {
      if (argv[i] == nullptr)
      {
        newRow[i].set((void*)nullptr);
      }
      else
      {
        newRow[i].set(argv[i]);
      }
    }
    pRequest->oResult.addRow(newRow);
  }
}
