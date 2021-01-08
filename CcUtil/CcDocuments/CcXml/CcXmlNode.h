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
 * @subpage   CcXmlNode
 *
 * @page      CcXmlNode
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcXmlNode
 */
#ifndef H_CcXmlNode_H_
#define H_CcXmlNode_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcVector.h"
#include "CcString.h"
#include "CcSharedPointer.h"

class CcXmlNodeList;
class CcXmlNodeListIterator;

#ifdef _MSC_VER
template class CcDocumentsSHARED CcSharedPointer<CcXmlNodeList>;
#endif

/**
 * @brief XmlNode with different Types
 *        It is representing a common node Name and Value, or it
 *        can also be a Comment or Doctype.
 */
class CcDocumentsSHARED CcXmlNode
{
public: // Types

//! Enumeration Content-Type within this node
  enum class EType
  {
    Unknown = 0, //!< Content-Type Unknown, this should never happen
    Node,        //!< Content is a XML-Node
    String,      //!< Content is a String
    Attribute,   //!< Content is a Attribute
    Comment,     //!< Content is a Comment-Line
    Doctype      //!< Content is a DocType
  };

public:
  /**
   * @brief Constructor
   */
  CcXmlNode(CcXmlNode::EType eNodeType = CcXmlNode::EType::Unknown);

  /**
   * @brief Constructor
   */
  CcXmlNode(const CcString& sName);
  
  /**
   * @brief Constructor
   */
  CcXmlNode(CcXmlNode::EType eNodeType, const CcString& sData);

  /**
   * @brief Constructor
   */
  CcXmlNode(const CcXmlNode& oToCopy);

  /**
   * @brief Constructor
   */
  CcXmlNode(CcXmlNode&& oToMove);

  /**
   * @brief Destructor
   */
  ~CcXmlNode();

  /**
   * @brief Move all content from a XmlNode to a new Node.
   *        This will set moved node to an unusable state, do not use it any more.
   * @param oToMove
   * @return
   */
  CcXmlNode& operator=(CcXmlNode&& oToMove);
  CcXmlNode& operator=(const CcXmlNode& oToCopy);

  bool operator==(const CcXmlNode& oToCompare) const;
  inline bool operator!=(const CcXmlNode& oToCompare) const
    { return !operator==(oToCompare); }

  inline CcXmlNode& operator[](size_t uiPosition) const
    { return at(uiPosition); }
  inline CcXmlNode& operator[](const CcString& sNodeName) const
    { return getNode(sNodeName); }

  void clear();
  void reset();

  size_t size() const;
  CcXmlNode& at(size_t i) const;
  CcXmlNodeList& remove(size_t iIndex);
  CcXmlNodeList& remove(const CcString& sName, size_t iIndex);
  CcXmlNode& append(const CcXmlNode& oAppend);
  CcXmlNode& append(CcXmlNode&& oAppend);

  /**
   * @brief Create a Xml Node and append it.
   * @param sName: Name of new Element
   * @return Reference to created Node
   */
  CcXmlNode& createSubNode(const CcString& sName);
  
  /**
   * @brief Create a Xml Node and append it, just if no other node with same name exists.
   * @param sName: Name of new Element
   * @param pbWasCreated: if not null, the boolean value will be set to true if new node was created
   * @return Reference to created Node, or reference to found node if already existing.
   */
  CcXmlNode& createSubNodeIfNotExists(const CcString& sName, bool *pbWasCreated = nullptr);
  
  CcXmlNodeList& nodeList()
  { return *m_pNodeList.ptr(); }
  
  const CcXmlNodeList& getNodeList() const
  { return *m_pNodeList.getPtr(); }


  /**
   * @brief Set name of Node
   * @param sName: new Name as String
   */
  inline void setName(const CcString& sName)
    { m_sData = sName; }

  /**
   * @brief Set Value of Node
   * @param sValue: Value of Node
   * @todo Is it realy required to have a Value?
   */
  inline void setInnerText(const CcString& sValue) 
    { clear(); append(CcXmlNode(CcXmlNode::EType::String, sValue)); }

  /**
   * @brief Set Node to an open tag.
   *        All subnodes are getting deleted.
   * @param bOpenTag
   */
  void setIsOpenTag(bool bOpenTag);

  /**
   * @brief Get bool if OpenTag-Flag is set.
   * @return true if XmlNode is an OpenTag.
   */
  inline bool isOpenTag() const
    { return m_bIsOpenTag; }

  /**
   * @brief Set new Type to Node
   * @param Type: new Type
   * @todo transform between the types
   */
  inline void setType(CcXmlNode::EType Type)
    { m_eType = Type; }

  /**
   * @brief Get type of current content
   * @return Type as enum.
   */
  inline CcXmlNode::EType getType() const
    { return m_eType; }

  /**
   * @brief Get name of this node.
   * @return Name as string
   */
  inline const CcString& getName() const
    { return m_sData; }

  /**
   * @brief Get all subdata of this Node in XML-Format
   * @return String with XML-Data
   */
  CcString innerXml() const;

  /**
   * @brief Get all data and subdata of this Node in XML-Format
   * @return String with XML-Data
   */
  CcString outerXml() const;

  /**
   * @brief Get all strings stored in this Node.
   *        It appends all Strings from Sub-Nodes too.
   *        Otherwise than innerXml, Data is not formated and Xml-Values are
   *        excluded.
   * @return Text as String.
   */
  CcString innerText() const;

  /**
   * @brief Get all sub-nodes in this node as NodeList.
   * @param nodeName: nodes can be filtered by name.
   * @return Nodes as NodeList
   */
  CcXmlNodeList getNodes(const CcString& nodeName = "", bool bRecurse = false) const;
  
  /**
   * @brief Get all sub-nodes in this node as NodeList.
   * @param nodeName: nodes can be filtered by name.
   * @return Nodes as NodeList
   */
  CcXmlNodeList getAttributes(const CcString& nodeName = "") const;

  /**
   * @brief Get Single Node from List by Name
   * @param nodeName: Name of required Node
   * @param nr: If more than one Node with the same name is stored,
   *            the required node can be selected by it's index
   * @return Target Node or NULL if not found
   */
  CcXmlNode& getNode(const CcString& nodeName, size_t nr = 0) const;
  
  /**
   * @brief Get first Single Node from List by its path
   * @param[in] oNodePath: select a Node with multpile subnodes;
   * @param[in] uiCurrentPos: current position in oNodePath;
   * @return Target Node or NULL if not found
   */
  inline CcXmlNode& getNode(const CcStringList& oNodePath, size_t uiCurrentPos=0) const
    { size_t uiNr = 0; return getNode(oNodePath, uiCurrentPos, uiNr);}

  /**
   * @brief Get single Node from List by Name and existing number
   * @param[in] oNodePath: select a Node with multpile subnodes;
   * @param[in,out] nr: If more than one Node with the same name is stored,
   *            the required node can be selected by it's index
   *            This number will be reduced for every matching node.
   * @param[in] uiCurrentPos: current position in oNodePath;
   * @return Target Node or NULL if not found
   */
  CcXmlNode& getNode(const CcStringList& oNodePath, size_t uiCurrentPos, size_t& nr) const;

  CcXmlNode& getLastAddedNode()
    { return *m_pLastAddedNode; }

  inline bool isNull() const
    { return CcXmlNode::EType::Unknown == m_eType; }
  inline bool isNotNull() const
    { return CcXmlNode::EType::Unknown != m_eType; }

  CcXmlNodeListIterator begin();
  CcXmlNodeListIterator end();

private:
  bool m_bIsOpenTag = false;  //!< Open-Tag indicator
  CcString m_sData;   //!< Value stored in this node, this can be the name if Node is name or content else
  CcXmlNode::EType m_eType = CcXmlNode::EType::Unknown;       //!< Type of Node
  CcSharedPointer<CcXmlNodeList> m_pNodeList;
  CcXmlNode* m_pLastAddedNode = nullptr;
};

#include "CcXmlNodeList.h"

#endif // H_CcXmlNode_H_
