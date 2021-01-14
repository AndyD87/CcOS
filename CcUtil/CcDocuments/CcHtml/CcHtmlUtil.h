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
 * @brief     Class CcHtmlUtil
 **/
#ifndef H_CcHtmlUtil_H_
#define H_CcHtmlUtil_H_

#include "CcBase.h"
#include "CcHtml.h"
#include "CcHtmlNode.h"

/**
 * @brief This Class is a Static Class. It is used to Operate on @ref CcHTmlNode CcHtmlNodes.
 */
class CcDocumentsSHARED CcHtmlUtil 
{
public:
  /**
   * @brief Find a XmlNode by searching with Name and Attributes
   * @param pNode: Node to search recursively in
   * @param sName: Name of Target Node
   * @param Attributes: Attribute List wich names and values have to match
   * @param nr: skip until number is reached
   * @return Pointer to Node if found or NULL if not;
   */
  static CcHtmlNode& findNodeByAttributes(CcHtmlNode& pNode, const CcString& sName,
                                          CcVector<CcHtmlAttribute*> &Attributes, size_t nr = 0);
private:
  /**
   * @brief Constructor
   */
  CcHtmlUtil();

  /**
   * @brief Destructor
   */
  ~CcHtmlUtil() = default;
};

#endif // H_CcHtmlUtil_H_
