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
 * @brief     Class CcHtmlTable
 **/
#pragma once

#include "Html/CcHtml.h"
#include "Html/CcHtmlNode.h"
#include "Html/IHtmlImpl.h"
#include "CcList.h"

/**
 * @brief Basic cell fort html tables
 */
class CcDocumentsSHARED CcHtmlTableCell : public IHtmlImpl
{
public:
  /**
   * @brief Setup existing node as html *td*
   * @param rNode: Node to handle as html cell
   */
  CcHtmlTableCell(CcHtmlNode& rNode = CcHtmlNode::getNullNode()) :
    IHtmlImpl(rNode, "td")
  {
  }

  /**
   * @brief Set number of columns to span in this cell
   * @param uiSize: Span size
   */
  void setColspan(size_t uiSize);
};

#ifdef _MSC_VER
template class CcDocumentsSHARED CcList<CcHtmlTableCell>;
#endif

/**
 * @brief Setup html node with *tr* attributes
 */
class CcDocumentsSHARED CcHtmlTableRow : public IHtmlImpl
{
public:
  /**
   * @brief Create row node
   * @param rNode:  Node to handle as html table row
   * @param uiCols: Number of columns to setup
   */
  CcHtmlTableRow(CcHtmlNode& rNode = CcHtmlNode::getNullNode(), size_t uiCols = 0) :
    IHtmlImpl(rNode, "tr")
  {
    CCUNUSED(uiCols);
  }
private:
  CcList<CcHtmlTableCell> m_oRows;
};

#ifdef _MSC_VER
template class CcDocumentsSHARED CcList<CcHtmlTableRow>;
#endif

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcHtmlTable : public IHtmlImpl
{
public:
  /**
   * @brief Create table on existing html node
   * @param rNode:  Node to setup as html *table*
   * @param uiCols: Number of colums for table.
   */
  CcHtmlTable(CcHtmlNode& rNode, size_t uiCols);

  /**
   * @brief Cleanup table
   */
  virtual ~CcHtmlTable();

  /**
   * @brief Create a row for table, and store it in list.
   * @return Handle to crated table row.
   */
  CcHtmlTableRow& createRow();

private:
  size_t m_uiColumns = 0;
  CcList<CcHtmlTableRow> m_oRows;
};
