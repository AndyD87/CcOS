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
#ifndef H_CcHtmlDocument_H_
#define H_CcHtmlDocument_H_

#include "CcBase.h"
#include "CcHtml.h"
#include "CcString.h"
#include "CcHtmlNode.h"

class IIo;

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
  CcHtmlDocument(const CcHtmlNode& rNode);

  /**
   * @brief Construct Class with a Text as Base.
   * @param String: String containing a HTML-Document
   */
  CcHtmlDocument(const CcString& String);

  /**
   * @brief Destructor
   */
  ~CcHtmlDocument() = default;

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
   * @brief Generate a HtmlDocument by Content and write to io device
   * @param rStream: Target output device or string stream.
   * @param bIntend: use Intends for inserted Nodes
   */
  void writeHtmlDocument(IIo& rStream, bool bIntend = false);

  /**
   * @brief Get current root node. If this Method is return NULL,
   *        and a Document was parsed, the Document was not in a correct
   *        Html-Format.
   * @return Pointer to root-Node or NULL if no root is set
   */
  CcHtmlNode& getRootNode()
    { return m_pRootNode; }

private: // Methods
  /**
   * @brief Find next Node in content.
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Found node or NULL if not
   */
  bool findNode(const CcString& String, size_t &startPos, CcHtmlNode& rOutNode);

  /**
   * @brief Parse Content of a within a Xml-tag
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Found node or NULL if tag has errors
   */
  void parseInnerTag(const CcString& String, size_t &startPos, CcHtmlNode& rOutNode);

  /**
   * @brief Find Attributes within a node-tag
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Attribute as new node with OpenTag-Flag, or NULL if not found
   */
  bool findAttribute(const CcString& String, size_t &startPos, CcHtmlAttribute& rOutAttribute);

  void innerHtml(CcHtmlNode& pNode, IIo& rStream, bool bIntend);
  void outerHtml(CcHtmlNode& pNode, IIo& rStream, bool bIntend);
  void writeIntends(IIo& rStream);
private:
  bool m_bContentValid    = false;  //!< Is current Content valid or has something changed.
  CcHtmlNode m_pRootNode;           //!< Root Node of Document
  size_t m_uiIntendLevel = 0;
  size_t m_uiIntendSize = 2;
};

#endif // H_CcHtmlDocument_H_
