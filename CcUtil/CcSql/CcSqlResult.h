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
 * @brief     Class CcSqlResult
 */
#pragma once

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
  virtual ~CcSqlResult();

  /**
   * @brief Move data from other table to this
   * @param oToMove: Result to move from
   * @return Handle to this
   */
  CcSqlResult& operator=(CcSqlResult&& oToMove);

  /**
   * @brief Copy data from other table to this
   * @param oToCopy: Result to copy from
   * @return Handle to this
   */
  CcSqlResult& operator=(const CcSqlResult& oToCopy);

  //! @return True if error occured
  inline bool error() const
  { return m_iErrorCode != 0; }
  //! @return True if error no error occured
  inline bool ok() const
  { return m_iErrorCode == 0; }
  //! @return Get last error code
  int32 getErrorCode() const
  { return m_iErrorCode; }
  //! @return Get message from last error
  const CcString& getErrorMessage() const
  { return m_sErrorMsg; }
  //! @return Get last inserted id if one was set
  uint64 getLastInsertId()
  { return m_uiLastInsertId;}
  
  //! @param iErrorCode: Overwrite current error code with this
  void setErrorCode(int32 iErrorCode)
  { m_iErrorCode = iErrorCode; }
  //! @param sError: Overwrite current error message with this
  void setErrorMessage(const CcString& sError)
  { m_sErrorMsg = sError; }
  //! @param uiLastInsertID: Overwrite current last id with this
  void setLastInsertId(uint64 uiLastInsertID)
  { m_uiLastInsertId = uiLastInsertID; }

private:
  int32 m_iErrorCode=0;
  CcString m_sErrorMsg;
  uint64 m_uiLastInsertId = 0;
};
