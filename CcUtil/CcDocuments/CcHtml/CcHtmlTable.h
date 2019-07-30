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
 * @page      CcUtil
 * @subpage   CcHtmlTable
 *
 * @page      CcHtmlTable
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHtmlTable
 **/
#ifndef H_CcHtmlTable_H_
#define H_CcHtmlTable_H_

#include "CcHtml.h"
#include "CcHtmlNode.h"
#include "CcList.h"

class CcDocumentsSHARED CcHtmlTableCell : public CcHtmlNode
{
public:
  CcHtmlTableCell(CcHtmlNode* pParent = nullptr) :
    CcHtmlNode(pParent, "td")
  {
  }

  void setColspan(size_t uiSize);
};

#ifdef _MSC_VER
template class CcDocumentsSHARED CcList<CcHtmlTableCell>;
#endif

class CcDocumentsSHARED CcHtmlTableRow : public CcHtmlNode
{
public:
  CcHtmlTableRow(CcHtmlNode* pParent = nullptr, size_t uiCols = 0) :
    CcHtmlNode(pParent, "tr")
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
class CcDocumentsSHARED CcHtmlTable : public CcHtmlNode
{
public:
  /**
   * @brief Constructor
   */
  CcHtmlTable(CcHtmlNode* pParent, size_t uiCols);

  /**
   * @brief Destructor
   */
  virtual ~CcHtmlTable();

  CcHtmlTableRow& createRow();

private:
  size_t m_uiColumns = 0;
  CcList<CcHtmlTableRow> m_oRows;
};

#endif /* H_CcHtmlTable_H_ */
