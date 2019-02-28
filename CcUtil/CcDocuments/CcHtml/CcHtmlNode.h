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
 * @subpage   CcHtmlNode
 *
 * @page      CcHtmlNode
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHtmlNode
 */
#ifndef _CcHtmlNode_H_
#define _CcHtmlNode_H_

#include "CcBase.h"
#include "CcHtml.h"
#include "CcString.h"
#include "CcList.h"
#include "CcHtmlAttribute.h"
#include "CcHtmlTypes.h"

// Forward declaration
class CcHtmlNodeList;
class CcHtmlNode;

#ifdef WIN32
template class CcDocumentsSHARED CcList<CcHtmlAttribute*>;
template class CcDocumentsSHARED CcList<CcHtmlNode*>;
#endif

/**
 * @brief HtmlNode with different Types
 *        It is representing a common node Name and Value, or it
 *        can also be a Comment or Doctype.
 *        For further use, different types, like Headlines or phrases, will be supported,
 *        so it's possible to use their special features and auto-setup attributes for example.
 */
class CcDocumentsSHARED CcHtmlNode : public CcList<CcHtmlNode*>
{
public:
  //! Types as enum a HtmlNode can have.
  typedef enum{
    eString,
    eComment,
    eDoctype,
    eAttribute,
    eNode,
    eA,
    eB,
    eP,
    eBr,
    eHtml,
    eHead,
    eBody,
    eForm,
    eIframe,
    eTable,
    eTr,
    eTd,
    eTbody,
    eScript,
    eMeta,
    eH1,
    eH2,
    eH3
  }eType;

  /**
   * @brief Constructor
   */
  CcHtmlNode(eType Type = eNode);

  /**
   * @brief Destructor
   */
  ~CcHtmlNode();

  /**
   * @brief Set name of Node
   * @param sName: new Name as String
   */
  inline void setName(const CcString &sName)
    { m_sName = sName; }

  /**
   * @brief Set Value of Node
   * @param sValue: Value of Node
   */
  inline void setValue(const CcString &sValue)
    { m_sValue = sValue; }

  /**
   * @brief Set Parent-Node for recursive selecting.
   * @param pParent: Parent Node.
   */
  inline void setParent(CcHtmlNode* pParent)
    { m_pParent = pParent; }

  /**
   * @brief Set new Type to Node
   * @param Type: new Type
   * @todo transform between the types
   */
  inline void setType(eType Type)
    { m_Type = Type; }

  /**
   * @brief Get type of current content
   * @return Type as enum.
   */
  inline eType getType()
    { return m_Type; }

  /**
   * @brief Set Node to an open tag.
   *        All subnodes are getting deleted.
   * @param bOpenTag
   */
  inline void setOpenTag(bool bOpenTag)
    { m_bIsOpenTag = bOpenTag; }

  /**
   * @brief Get bool if OpenTag-Flag is set.
   * @return true if XmlNode is an OpenTag.
   */
  inline bool getOpenTag()
    { return m_bIsOpenTag; }

  /**
   * @brief Get name of this node.
   * @return Name as string
   */
  inline CcString &getName()
    { return m_sName; }

  /**
   * @brief Get currently set Value.
   * @return value as string
   * @todo Obsolete
   */
  inline CcString getValue()
    { return m_sValue; }

  /**
   * @brief Get List of Attributes stored in List
   * @return Attribues as Vector-List
   */
  inline CcList<CcHtmlAttribute*>& getAttributeList()
    { return m_lAttributes; }

  /**
   * @brief Add a Sub-Node to this node
   * @param Node: Pointer to new node
   */
  inline void addNode(CcHtmlNode *Node)
    { append(Node); }

  /**
   * @brief Delete a subnode at a given position from this list.
   *        Node will not be deleted, just removed from List.
   * @param iNode: position of node to delete.
   */
  inline void delNode(size_t iNode)
    { remove(iNode); }

  /**
   * @brief Get number of Attributes stored in this Node
   * @return Count of Attributes
   */
  inline size_t getAttributeCount()
    { return m_lAttributes.size(); }

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
  CcHtmlNode* getNode(const CcString& sName, size_t nr = 0);

  /**
   * @brief Get all sub-nodes in this node as NodeList.
   * @param sName: nodes can be filtered by name.
   * @return Nodes as NodeList
   */
  CcHtmlNodeList getNodeList(const CcString& sName);

  /**
   * @brief Get an Attribute by name
   * @param sName: Name to search for
   * @return Pointer to Attribute or null if not found
   */
  CcHtmlAttribute* getAttribute(const CcString& sName);

  /**
   * @brief Get all data and subdata of this Node in HTML-Format
   * @return String with HTML-Data
   */
  CcString innerHtml();

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
  void addAttribute(CcHtmlAttribute *Attribute);

private: //Methods
  /**
   * @brief Set String of Name by Type
   * @param Type: type to Set.
   */
  void setNameByType(eType Type);

private:
  eType m_Type;       //!< Current type of this Node
  bool m_bIsOpenTag;  //!< Is this node an OpenTag node?
  CcString m_sName;   //!< Name of current node
  CcString m_sValue;  //!< Value stored in this node @todo obsolete
  CcHtmlNode *m_pParent;  //!< Parent Node
  CcList<CcHtmlAttribute*> m_lAttributes; //!< Attribute-List.
};

#endif /* _CcHtmlNode_H_ */
