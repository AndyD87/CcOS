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
 * @brief     Class CcTable
 */
#pragma once

#include "CcBase.h"
#include "CcList.h"
#include "CcTableRow.h"
#include "CcStringList.h"

#ifdef _MSC_VER
template class  CcKernelSHARED CcList<CcTableRow>;
#endif

class IIo;

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
  ~CcTable() = default;

  /**
   * @brief Get index of columen by name
   * @param sColName: Name of columen to search for
   * @return Index of column or SIZE_MAX if not found
   */
  size_t getColumnId(const CcString& sColName) const;

  /**
   * @brief Get name of columen on specific index
   * @param nr: Index of column to get name from
   * @return Name of column or empty if not available
   */
  const CcString& getColumnName(size_t nr) const {return m_ColNames[nr];}

  /**
   * @brief Get data from specified index.
   * @param col: Column of value
   * @param row: Row of value
   * @return Target value or invalid variant if not available.
   * @todo Work with references!
   */
  CcVariant getData(size_t col, size_t row) const;

  /**
   * @brief Get data from specified index.
   * @param colName: Name of column to get value from
   * @param row: Row of value
   * @return Target value or invalid variant if not available.
   * @todo Work with references!
   */
  CcVariant getData(const CcString& colName, size_t row) const;

  //! @return Get names of columns as list
  const CcStringList& getColumnNames() const {return m_ColNames;}
  //! @return Number of Colums in Table
  size_t getColumnCount() const{return m_Columns;}
  //! @brief Get Number of Rows in Table
  size_t rows() const{return size();}

  //! @param count: Set count of Columns stored in Table
  void setColumnCount(size_t count);
  //! @param slColNames: Set Name of Columns stored in Table
  void setColumnNames(const CcStringList& slColNames);

  /**
   * @brief Set Name of Column at specific index stored in Table
   * @param nr: Index of column
   * @param sColName: New name for column
   */
  void setColumnName(size_t nr, const CcString& sColName);

  /**
   * @brief Clear all rows and columns in table
   */
  void clear();

  /**
   * @brief Search for row where value in column matches with requested
   * @param uiColId:  Column index to search in each row
   * @param oValue:   Value to compare to find row
   * @return Matching row or SIZE_MAX if not found
   */
  size_t columnFind(size_t uiColId, const CcVariant& oValue) const;

  /**
   * @brief Search for row where value in column matches with requested
   * @param sName:    Target column to search in each row
   * @param oValue:   Value to compare to find row
   * @return Matching row or SIZE_MAX if not found
   */
  size_t columnFind(const CcString& sName, const CcVariant& oValue) const;

  /**
   * @brief Add row to table, by copy content. A value for each column should be available.
   *        If not, to much values will be cut, to less will be initialized with default.
   * @param rowToAdd: Row to add to table.
   */
  void addRow(const CcTableRow &rowToAdd);

  /**
   * @brief Add row to table, by moving content. A value for each column should be available.
   *        If not, to much values will be cut, to less will be initialized with default.
   * @param rowToAdd: Row to add to table.
   */
  void addRow(CcTableRow &&rowToAdd);

  /**
   * @brief Create new row at the end of table with default values
   */
  inline void newRow()
  { append(CcTableRow(*this)); }

  /**
   * @brief Print current table to output stream. Each value is getting called with toString(),
   * @param rStream:        Target output stream
   * @param uiMinSellWidth: Set minimum cell with if required
   */
  void printCli(IIo &rStream, size_t uiMinSellWidth = 0);

  /**
   * @brief Move data from another table to this
   * @param oToMove:  Table to move data from
   * @return Handle to this
   */
  CcTable& operator=(CcTable&& oToMove);

  /**
   * @brief Copy data from another table to this
   * @param oToCopy:  Table to copy data from
   * @return Handle to this
   */
  CcTable& operator=(const CcTable& oToCopy);
private:
  CcStringList m_ColNames; //!< Name for columns to search for if association is required.
  size_t       m_Columns;  //!< Number ofColums
};
