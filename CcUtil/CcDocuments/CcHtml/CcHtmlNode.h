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
 * @brief     Class CcHtmlNode
 */
#ifndef H_CcHtmlNode_H_
#define H_CcHtmlNode_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcVector.h"
#include "CcString.h"
#include "CcSharedPointer.h"
#include "CcGlobalStrings.h"
#include "CcHtmlAttribute.h"

// Forward declaration
class CcHtmlNodeList;
class CcHtmlNodeListIterator;

#ifdef _MSC_VER
template class CcDocumentsSHARED CcVector<CcHtmlAttribute>;
#endif

/**
 * @brief HtmlNode with different Types
 *        It is representing a common node Name and Value, or it
 *        can also be a Comment or Doctype.
 *        For further use, different types, like Headlines or phrases, will be supported,
 *        so it's possible to use their special features and auto-setup attributes for example.
 */
class CcDocumentsSHARED CcHtmlNode
{
public:
  //! Types as enum a HtmlNode can have.
  enum class EType
  {
    Unknown = 0,
    Node,
    String,
    Comment,
    Doctype,
    HtmlVersion,
  };

  /**
   * @brief Constructor
   */
  CcHtmlNode(const CcHtmlNode& oToCopy)
    { operator=(oToCopy); }

  /**
   * @brief Constructor
   */
  CcHtmlNode(EType eType = EType::Node);

  /**
   * @brief Constructor
   */
  CcHtmlNode(const CcString& sData, EType Type = EType::Node);

  /**
   * @brief Destructor
   */
  ~CcHtmlNode();

  CcHtmlNode& operator=(const CcHtmlNode& oToCopy);
  bool operator==(const CcHtmlNode& oToCompare) const;
  inline bool operator!=(const CcHtmlNode& oToCompare) const
    { return !operator==(oToCompare); }


  /**
   * @brief Set name of Node
   * @param sName: new Name as String
   */
  void setName(const CcString& sName);

  /**
   * @brief Set Value of Node
   * @param sValue: Value of Node
   */
  void setInnerText(const CcString& sValue);

  /**
   * @brief Set new Type to Node
   * @param eType: new Type
   * @todo transform between the types
   */
  void setType(EType eType);

  void setIdAttribute(const CcString& sId);
  void setClassAttribute(const CcString& sClass);
  void setNameAttribute(const CcString& sName);

  /**
   * @brief Get type of current content
   * @return Type as enum.
   */
  EType getType();

  /**
    * @brief Set Node to an open tag.
    *        All subnodes are getting deleted.
    * @param bOpenTag
    */
  void setOpenTag(bool bOpenTag);

  /**
   * @brief Get bool if OpenTag-Flag is set.
   * @return true if HtmlNode is an OpenTag.
   */
  bool getOpenTag();

  /**
   * @brief Get name of this node.
   * @return Name as string
   */
  CcString &getName();
  
  /**
   * @brief Get name of this node.
   * @return Name as string
   */
  const CcString& getName() const;

  /**
   * @brief Get List of Attributes stored in List
   * @return Attribues as Vector-List
   */
  CcVector<CcHtmlAttribute>& getAttributeList();

  void clear();

  size_t size() const;
  CcHtmlNode& at(size_t i) const;
  bool remove(size_t iIndex);
  bool remove(const CcString& sName, size_t iIndex);
  CcHtmlNode& append(const CcHtmlNode& oAppend);
  CcHtmlNode& append(CcHtmlNode&& oAppend);

  /**
   * @brief Get number of Attributes stored in this Node
   * @return Count of Attributes
   */
  inline size_t getAttributeCount()
    { return getAttributeList().size(); }

  /**
   * @brief Get current count of Subnodes in this Node.
   * @return Number of Nodes
   */
  inline size_t getNodeCount()
    { return size(); }

  /**
   * @brief Get Single Node from List by Name
   * @param sName: Name of required Node
   * @param nr: If more than one Node with the same name is stored,
   *            the required node can be selected by it's index
   * @return Target Node or NULL if not found
   */
  CcHtmlNode& getNode(const CcString& sName, size_t nr = 0);
  
  CcHtmlNode& getLastAddedNode();
  
  bool isNull() const;
  inline bool isNotNull() const
    { return !isNull(); }

  /**
   * @brief Create a new named HtmlNode.
   *        The created node will be deleted if this not is getting delted.
   * @param sName: Name of new Node
   * @return New Node
   */
  CcHtmlNode& createNode(const CcString& sName = CcGlobalStrings::Empty);

  /**
   * @brief Create a new named HtmlNode.
   *        The created node will be deleted if this not is getting delted.
   * @param sName: Name of new Node
   * @return New Node
   */
  CcHtmlNode& createString(const CcString& sContent = CcGlobalStrings::Empty);

  /**
   * @brief Get Single Node from List by Name or create if not existing
   * @param sName: Name of required Node
   * @return Target Node
   */
  CcHtmlNode& getOrCreateNode(const CcString& sName);

  /**
   * @brief Get all sub-nodes in this node as NodeList.
   * @param sName: nodes can be filtered by name.
   * @return Nodes as NodeList
   */
  CcHtmlNodeList getNodeList(const CcString& sName, bool bRecursive = false);

  /**
   * @brief Get an Attribute by name
   * @param sName: Name to search for
   * @return Pointer to Attribute or null if not found
   */
  CcHtmlAttribute& getAttribute(const CcString& sName);

  /**
   * @brief Create a new attribute.
   *        This attribute will be deleted if this node is deleted.
   * @param sName: Name to create
   * @return Pointer to Attribute
   */
  CcHtmlAttribute& createAttribute(const CcString& sName);

  /**
   * @brief Get an Attribute by name or create if not existing
   * @param sName: Name to search for
   * @return Pointer to Attribute
   */
  CcHtmlAttribute& getOrCreateAttribute(const CcString& sName);

  /**
   * @brief Get all Elements within this node in HTML-Format
   * @return String with HTML-Data
   */
  CcString innerHtml();

  /**
   * @brief Get all data and subdata of this Node in HTML-Format
   * @return String with HTML-Data
   */
  CcString outerHtml(void);

  /**
   * @brief Get all strings stored in this Node.
   *        It appends all Strings from Sub-Nodes too.
   *        Otherwise than innerHtml, Data is not formated and HTML-Values are
   *        excluded.
   * @return Text as String.
   */
  CcString innerText();

  /**
   * @brief Add a Attribute to List
   * @param Attribute: Attribute to add.
   */
  void addAttribute(const CcHtmlAttribute& Attribute);

  CcHtmlNodeListIterator begin();
  CcHtmlNodeListIterator end();
  static CcHtmlNode& getNullNode();
private:
  class CPrivate;
private:
  CPrivate* m_pPrivate = nullptr;

  static CcHtmlNode s_oNullNode;
};

#endif // H_CcHtmlNode_H_
