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
 * @page      Types
 * @subpage   CcTable
 *
 * @page      CcTable
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTable
 */
#ifndef _CcTable_H_
#define _CcTable_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcList.h"
#include "CcTableRow.h"
#include "CcStringList.h"

#ifdef WIN32
template class  CcKernelSHARED CcList<CcTableRow>;
#endif

/**
 * @brief Storage Class for Tables
 *        it operates with a fixed size of Columns, and varios rows.
 */
class CcKernelSHARED  CcTable : public CcList<CcTableRow>
{
public:
  /**
   * @brief Constructor
   */
  CcTable( size_t columns = 0 );
  

  /**
   * @brief CopyConstructor
   */
  CcTable(const CcTable& oToCopy);

  /**
   * @brief MoveConstructor
   */
  CcTable(CcTable&& oToMove);

  /**
   * @brief Destructor
   */
  ~CcTable(void );

  void setColumnCount(size_t count);

  /**
   * @brief Set Name of Columns stored in Table
   * @param slColNames
   */
  void setColumnNames(const CcStringList& slColNames);

  /**
   * @brief Set Name of Columns stored in Table
   * @param slColNames
   */
  void setColumnName(size_t nr, const CcString& sColName);
  
  size_t getColumnId(const CcString& sColName) const;

  /**
   * @brief Set Name of Columns stored in Table
   * @param slColNames
   */
  const CcStringList& getColumnNames() const {return m_ColNames;}

  /**
   * @brief Set Name of Columns stored in Table
   * @param slColNames
   */
  const CcString& getColumnName(size_t nr) const {return m_ColNames[nr];}

  /**
   * @brief Get Number of Colums in List
   * @return Number of Colums in Table
   */
  size_t getColumnCount(void) const{return m_Columns;}

  /**
   * @brief Get Number of Rows in List,
   *        returns the same like size()
   * @return Number of Rows in Table
   */
  size_t rows(void) const{return size();}

  void clear(void);

  size_t columnfind(size_t uiColId, const CcVariant& oValue) const;
  size_t columnfind(const CcString& sName, const CcVariant& oValue) const;

  inline void addRow(const CcTableRow &rowToAdd)
  { append(rowToAdd);}
  CcVariant getData(size_t col, size_t row) const;
  CcVariant getData(const CcString& colName, size_t row) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcTable& oToCompare) const
    { return ( m_Columns == oToCompare.m_Columns && m_ColNames == oToCompare.m_ColNames); }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcTable& oToCompare) const
    { return ( m_Columns != oToCompare.m_Columns || m_ColNames != oToCompare.m_ColNames); }

  CcTable& operator=(CcTable&& oToMove);
  CcTable& operator=(const CcTable& oToCopy);
private:
  CcStringList m_ColNames; //!< Name for columns to search for if association is required.
  size_t       m_Columns;  //!< Number ofColums
};

#endif /* _CcTable_H_ */
