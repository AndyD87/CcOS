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
 * @brief     Implementation of Class CcSqlResult
 */
#include "CcSqlResult.h"

CcSqlResult::CcSqlResult(size_t numCols):
  CcTable(numCols)
{
}

CcSqlResult::~CcSqlResult()
{
}

CcSqlResult& CcSqlResult::operator=(CcSqlResult&& oToMove)
{
  if (this != &oToMove)
  {
    CcTable::operator=(CCMOVE(oToMove));
    m_iErrorCode = oToMove.m_iErrorCode;
    m_sErrorMsg = CCMOVE(oToMove.m_sErrorMsg);
    m_uiLastInsertId = oToMove.m_uiLastInsertId;
  }
  return *this;
}

CcSqlResult& CcSqlResult::operator=(const CcSqlResult& oToCopy)
{
  CcTable::operator=(oToCopy);
  m_iErrorCode = oToCopy.m_iErrorCode;
  m_sErrorMsg = oToCopy.m_sErrorMsg;
  m_uiLastInsertId = oToCopy.m_uiLastInsertId;
  return *this;
}

CcSqlResult::CcSqlResult(const CcSqlResult& oToCopy):
  CcTable(oToCopy)
{
  m_iErrorCode = oToCopy.m_iErrorCode;
  m_sErrorMsg = oToCopy.m_sErrorMsg;
  m_uiLastInsertId = oToCopy.m_uiLastInsertId;
}

CcSqlResult::CcSqlResult(CcSqlResult&& oToMove) :
  CcTable(CCMOVE(oToMove))
{
  operator=(CCMOVE(oToMove));
}
