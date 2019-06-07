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
 * @page      CcHtml
 * @subpage   CcHtmlDocument
 *
 * @page      CcHtmlDocument
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHtmlDocument
 */
#ifndef _CcHtmlDocument_H_
#define _CcHtmlDocument_H_

#include "CcBase.h"
#include "CcHtml.h"
#include "CcString.h"
#include "CcHtmlNode.h"

/**
 * @brief Html Document Manager, it can parse or generate HTML-Documents.
 */
class CcDocumentsSHARED CcHtmlDocument 
{
public:
  /**
   * @brief Construct Class with HtmlNode as base.
   * @param node: Html Node
   */
  CcHtmlDocument() = default;

  /**
   * @brief Construct Class with HtmlNode as base.
   * @param node: Html Node
   */
  CcHtmlDocument(CcHtmlNode *node);

  /**
   * @brief Construct Class with a Text as Base.
   * @param String: String containing a HTML-Document
   */
  CcHtmlDocument(const CcString& String);

  /**
   * @brief Destructor
   */
  ~CcHtmlDocument();

  /**
   * @brief Parse a Document wich is represented as String.
   * @param String: Document as String.
   */
  void parseDocument(const CcString& String);

  /**
   * @brief Generate a HtmlDocument by Content
   * @param bIntend: use Intends for inserted Nodes
   * @return Document as referenced String.
   */
  CcString getHtmlDocument(bool bIntend = false);

  /**
   * @brief Get current root node. If this Method is return NULL,
   *        and a Document was parsed, the Document was not in a correct
   *        Html-Format.
   * @return Pointer to root-Node or NULL if no root is set
   */
  CcHtmlNode* getRootNode()
    { return m_pRootNode; }

private: // Methods
  /**
   * @brief Find next Node in content.
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Found node or NULL if not
   */
  CcHtmlNode* findNode(const CcString& String, size_t &startPos);

  /**
   * @brief Parse Content of a within a Xml-tag
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Found node or NULL if tag has errors
   */
  CcHtmlNode* parseInnerTag(const CcString& String, size_t &startPos);

  /**
   * @brief Find Attributes within a node-tag
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Attribute as new node with OpenTag-Flag, or NULL if not found
   */
  CcHtmlAttribute* findAttribute(const CcString& String, size_t &startPos);

  void createRootNode();
  void removeRootNode();
private:
  bool m_bContentValid    = false;   //!< Is current Content valid or has something changed.
  bool m_bRootOwner      = false;   //!< Parsing documents requires a new node wich has to be deleted at end.
  CcHtmlNode *m_pRootNode  = nullptr; //!< Root Node of Document
};

#endif /* _CcHtmlDocument_H_ */
