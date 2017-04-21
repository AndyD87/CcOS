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
 * @subpage   CcSqlResult
 *
 * @page      CcSqlResult
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSqlResult
 */
#ifndef CcSqlResult_H_
#define CcSqlResult_H_

#include "CcBase.h"
#include "CcSql.h"
#include "CcTable.h"

/**
 * @brief Storage Class for a Sql-Result.
 *        It is representing a Table with stored Data returned from an Request.
 */
class CcSqlSHARED CcSqlResult : public CcTable
{
public:
  /**
   * @brief Constructor
   */
  CcSqlResult( size_t numCols = 0 );

  /**
  * @brief CopyConstructor
  */
  CcSqlResult(const CcSqlResult& oToCopy);

  /**
  * @brief MoveConstructor
  */
  CcSqlResult(CcSqlResult&& oToMove);

  /**
   * @brief Destructor
   */
  virtual ~CcSqlResult( void );

  inline bool error() const
    { return m_iErrorCode != 0; }
  inline bool ok() const
    { return m_iErrorCode == 0; }
  int32 getErrorCode() const
    { return m_iErrorCode; }
  const CcString& getErrorMessage() const
    { return m_sErrorMsg; }
  size_t getLastInsertId()
    { return m_uiLastInsertId;}
  
  void setErrorCode(int32 iErrorCode)
    { m_iErrorCode = iErrorCode; }
  void setErrorMessage(const CcString& sError)
    { m_sErrorMsg = sError; }
  void setLastInsertId(size_t uiLastInsertID)
    { m_uiLastInsertId = uiLastInsertID; }


  CcSqlResult& operator=(const CcSqlResult& oToCopy);
  CcSqlResult& operator=(CcSqlResult&& oToMove);

private:
  int32 m_iErrorCode=0;
  CcString m_sErrorMsg;
  size_t m_uiLastInsertId = 0;
};

#endif /* CcSqlResult_H_ */
