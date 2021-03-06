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
 * @brief     Implementation of Class CcTableRow
 */
#include "CcTableRow.h"
#include "CcTable.h"
#include "CcStringList.h"
#include "CcStatic.h"

CcTableRow::CcTableRow(CcTable& Table) :
  CcVariantList(Table.getColumnCount()),
  m_pPartOfTable(&Table)
{
}

CcTableRow::CcTableRow(const CcTableRow& oToCopy):
  CcVariantList(oToCopy)
{
  m_pPartOfTable = oToCopy.m_pPartOfTable;
}

CcTableRow::CcTableRow(CcTableRow&& oToMove)
{
  operator=(CCMOVE(oToMove));
}

void CcTableRow::setParentTable(CcTable* rParentTable)
{
  m_pPartOfTable = rParentTable;
}

CcTableRow& CcTableRow::operator=(CcTableRow && oToMove)
{
  if (this != &oToMove)
  {
    CcVariantList::operator=(CCMOVE(oToMove));
    m_pPartOfTable = oToMove.m_pPartOfTable;
  }
  return *this;
}

CcTableRow& CcTableRow::operator=(const CcTableRow & oToCopy)
{
  CcVariantList::operator=(oToCopy);
  m_pPartOfTable = oToCopy.m_pPartOfTable;
  return *this;
}

const CcVariant& CcTableRow::operator[](const CcString& sColumnName) const
{
  size_t uiCnt = 0;
  for (const CcString& sListColumns : m_pPartOfTable->getColumnNames())
  {
    if (sListColumns == sColumnName)
      return operator[](uiCnt);
    uiCnt++;
  }
  return CcStatic::getConstNullRef<CcVariant>();
}

CcVariant& CcTableRow::operator[](const CcString& sColumnName)
{
  size_t uiCnt = 0;
  for (const CcString& sListColumns : m_pPartOfTable->getColumnNames())
  {
    if (sListColumns == sColumnName)
      return operator[](uiCnt);
    uiCnt++;
  }
  return CcStatic::getNullRef<CcVariant>();
}

bool CcTableRow::operator==(const CcTableRow& oToCompare) const
{
  bool bRet = false;
  if (m_pPartOfTable == oToCompare.m_pPartOfTable)
  {

    bRet = true;
  }

  return bRet;
}
