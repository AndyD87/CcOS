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
 * @page      CcXml
 * @subpage   CcXmlDocument
 *
 * @page      CcXmlDocument
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcXmlDocument
 */
#ifndef _CcXmlDocument_H_
#define _CcXmlDocument_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcString.h"
#include "CcXmlNode.h"

/**
 * @brief Html Document Manager, it can parse or generate HTML-Documents.
 */
class CcDocumentsSHARED CcXmlDocument {
public:

  /**
   * @brief Default Constructor, with no data to parse.
   *        Use parseDocument to get data from String, or manipulate rootNode to get a xml string.
   */
  CcXmlDocument() = default;

  /**
   * @brief Construct Class with HtmlNode as base.
   * @param node: Html Node
   */
  CcXmlDocument(CcXmlNode& node);

  /**
   * @brief Construct Class with a Text as Base.
   * @param String: String containing a HTML-Document
   */
  CcXmlDocument(const CcString& String);

  /**
   * @brief Destructor
   */
  ~CcXmlDocument();

  /**
   * @brief Parse a Document wich is represented as String.
   * @param String: Document as String.
   */
  bool parseDocument(const CcString& String);

  /**
   * @brief Generate a XmlDocument by Content
   * @param bIntend: use Intends for inserted Nodes
   * @return Document as referenced String.
   */
  CcString& getDocument(bool bIntend = false);

  void writeInnerXml(const CcXmlNode& oNode);

  /**
   * 
   */
  CcXmlNode& rootNode()
    { return m_RootNode; }
private: // Methods
  /**
   * @brief Append Intends to current Content
   * @return void
   */
  void writeIntend();

  /**
   * @brief Depending on m_bIntend, write a new line.
   * @return void
   */
  void writeNewLine();

  /**
   * @brief Find next Node in content.
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Found node or NULL if not
   */
   bool findNode(const CcString& String, size_t &startPos, CcXmlNode& outNode);

  /**
   * @brief Find Attributes within a node-tag
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Attribute as new node with OpenTag-Flag, or NULL if not found
   */
  bool findAttribute(const CcString& String, size_t &startPos, CcXmlNode& outNode);

  /**
   * @brief Parse Content of a within a Xml-tag
   * @param String: Content String
   * @param startPos: offset for beginning of search
   * @return Found node or NULL if tag has errors
   */
  bool parseInnerTag(const CcString& String, size_t &startPos, CcXmlNode& outNode);

private:
  CcXmlNode m_RootNode = CcXmlNode(EXmlNodeType::Node);                 //!< Root Node of Document
  CcString  m_sContent;                 //!< Content as String
  bool      m_bIntend        = false;   //!< Write Xml in an Human readable format
  uint16    m_uiIntendLevel  = 0;       //!< Representing the current level of subnodes for output content
};

#endif /* _CcXmlDocument_H_ */
