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
 * @subpage   CcHtmlScript
 *
 * @page      CcHtmlScript
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHtmlScript
 **/
#ifndef _CcHtmlScript_H_
#define _CcHtmlScript_H_

#include "CcHtml.h"
#include "CcHtmlNode.h"

/**
 * @brief Class impelmentation
 */
class CcDocumentsSHARED CcHtmlScript : public CcHtmlNode
{
public:
  /**
   * @brief Constructor
   */
  CcHtmlScript(CcHtmlNode* pParent);

  /**
   * @brief Destructor
   */
  virtual ~CcHtmlScript();

  void setContent(const CcString& sContent)
    { m_oContent.setInnerText(sContent); }

private:
  CcHtmlNode m_oContent;
};

#endif /* _CcHtmlScript_H_ */
