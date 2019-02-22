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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcTable
 */
#include "CcTable.h"
#include "CcStringList.h"

CcTable::CcTable(size_t columns ):
  m_Columns(columns)
{
  setColumnCount(columns);
}

CcTable::CcTable(const CcTable& oToCopy) :
  CcList<CcTableRow>(oToCopy)
{
  operator=(oToCopy);
  m_Columns = oToCopy.m_Columns;
  m_ColNames = oToCopy.m_ColNames;
  for (CcTableRow& oRow : *this) oRow.setParentTable(this);
}

CcTable::CcTable(CcTable&& oToMove)
{
  *this = oToMove;
}

void CcTable::setColumnCount(size_t count)
{
  if (m_Columns < count)
  {
    for (size_t i = m_ColNames.size(); i < count; i++)
    {
      CcString sName("");
      m_ColNames.append(sName);
    }
  }
  else
  {
    for (size_t i = m_Columns; i > count; i--)
    {
      m_ColNames.remove(i-1);
    }
  }
  m_Columns = count;
}

void CcTable::setColumnNames(const CcStringList& slColNames)
{
  m_ColNames.clear();
  for(const CcString sName : slColNames)
  {
    m_ColNames.append(sName);
  }
}

void CcTable::setColumnName(size_t nr, const CcString& sColName)
{
  m_ColNames.at(nr) = sColName;
}

size_t CcTable::getColumnId(const CcString& sColName) const
{
  return m_ColNames.findString(sColName);
}

void CcTable::clear(void)
{
  CcList<CcTableRow>::clear();
  m_ColNames.clear();
  m_Columns=0;
}

size_t CcTable::columnfind(size_t uiColId, const CcVariant& oValue) const
{
  size_t uiCnt = 0;
  for (CcTableRow& oRow : *this)
  {
    if (oValue == oRow[uiColId])
      return uiCnt;
    else
      uiCnt ++;
  }
  return SIZE_MAX;
}

size_t CcTable::columnfind(const CcString& sName, const CcVariant& oValue) const
{
  size_t uiColumnId = getColumnId(sName);
  return columnfind(uiColumnId, oValue);
}

CcVariant CcTable::getData(size_t col, size_t row) const
{
  return at(row).at(col);
}

CcVariant CcTable::getData(const CcString& colName, size_t row) const
{
  return getData(getColumnId(colName), row);
}

CcTable& CcTable::operator=(CcTable&& oToMove)
{
  if (this != &oToMove)
  {
    CcList<CcTableRow>::operator=(std::move(oToMove));
    m_Columns = oToMove.m_Columns;
    m_ColNames = std::move(oToMove.m_ColNames);
    for (CcTableRow& oRow : *this) oRow.setParentTable(this);
  }
  return *this;
}

CcTable & CcTable::operator=(const CcTable & oToCopy)
{
  CcList<CcTableRow>::operator=(oToCopy);
  m_Columns = oToCopy.m_Columns;
  m_ColNames = oToCopy.m_ColNames;
  for (CcTableRow& oRow : *this) oRow.setParentTable(this);
  return *this;
}
