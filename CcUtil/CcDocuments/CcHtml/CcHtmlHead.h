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
 * @subpage   CcHtmlHead
 *
 * @page      CcHtmlHead
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHtmlHead
 **/
#ifndef _CcHtmlHead_H_
#define _CcHtmlHead_H_

#include "CcHtml.h"
#include "CcHtmlNode.h"
#include "CcHtmlNodeList.h"

/**
 * @brief Class impelmentation
 */
class CcDocumentsSHARED CcHtmlHead : public CcHtmlNode
{
public:
  /**
   * @brief Constructor
   */
  CcHtmlHead(CcHtmlNode* pParent);

  /**
   * @brief Destructor
   */
  virtual ~CcHtmlHead();

  void setTitle(const CcString& sTitle);
  void setIcon(const CcString& sIcon);
  void setCharset(const CcString& sCharset);

  CcHtmlNode* addRelCanonical(const CcString& sLink);
  CcHtmlNode* addMeta(const CcString& sName, const CcString& sContent = "");
  CcHtmlNode* addStyleSheet(const CcString& sLink);
  CcHtmlNode* addScript(const CcString& sLink);
private: // Types
  CcHtmlNode* findMetaByName(const CcString& sName);
};

#endif /* _CcHtmlHead_H_ */
