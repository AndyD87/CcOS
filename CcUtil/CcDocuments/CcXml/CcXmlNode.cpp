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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcXmlNode
 */
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlNodeList.h"
#include "CcStringList.h"
#include "CcGlobalStrings.h"

CcXmlNode s_oNullNode(CcXmlNode::EType::Unknown);

CcXmlNode::CcXmlNode(CcXmlNode::EType eNodeType) :
  m_eType(eNodeType)
{
  CCNEW(m_pNodeList, CcXmlNodeList);
}

CcXmlNode::CcXmlNode(const CcString& sName) :
  m_sData(sName),
  m_eType(CcXmlNode::EType::Node)
{
  CCNEW(m_pNodeList, CcXmlNodeList);
}

CcXmlNode::CcXmlNode(CcXmlNode::EType eNodeType, const CcString& sData):
  m_sData(sData),
  m_eType(eNodeType)
{
  if (eNodeType == CcXmlNode::EType::Node)
  {
    CCNEW(m_pNodeList, CcXmlNodeList);
  }
}

CcXmlNode::CcXmlNode(const CcXmlNode& oToCopy)
{
  CCNEW(m_pNodeList, CcXmlNodeList);
  operator=(oToCopy);
}

CcXmlNode::CcXmlNode(CcXmlNode&& oToMove)
{
  operator=(CCMOVE(oToMove));
}

CcXmlNode::~CcXmlNode()
{
}

CcXmlNode& CcXmlNode::operator=(CcXmlNode&& oToMove)
{
  if (this != &oToMove)
  {
    m_sData = CCMOVE(oToMove.m_sData);
    m_pNodeList = oToMove.m_pNodeList;
    m_bIsOpenTag = oToMove.m_bIsOpenTag;
    m_eType = oToMove.m_eType;
    oToMove.m_pNodeList = nullptr;
  }
  return *this;
}

CcXmlNode& CcXmlNode::operator=(const CcXmlNode& oToCopy)
{
  if (oToCopy.m_pNodeList == nullptr)
  {
    m_pNodeList = nullptr;
  }
  else
  {
    *m_pNodeList = *oToCopy.m_pNodeList;
  }
  m_bIsOpenTag = oToCopy.m_bIsOpenTag;
  m_sData = oToCopy.m_sData;
  m_eType = oToCopy.m_eType;
  return *this;
}

bool CcXmlNode::operator==(const CcXmlNode& oToCompare) const
{
  bool bSuccess = false;
  if (  m_bIsOpenTag  == oToCompare.m_bIsOpenTag  &&
        m_sData       == oToCompare.m_sData       &&
        m_eType        == oToCompare.m_eType      )
  {
    if (m_eType == CcXmlNode::EType::Node)
    {
      if (m_pNodeList->size() == oToCompare.m_pNodeList->size())
      {
        bSuccess = true;
        CcList<CcXmlNode>::iterator rIterator1 = oToCompare.m_pNodeList->begin();
        CcList<CcXmlNode>::iterator rIterator2 = m_pNodeList->begin();
        while (rIterator2 != m_pNodeList->end())
        {
          if (*rIterator1 != *rIterator2)
          {
            bSuccess = false;
            break;
          }
          rIterator1++;
          rIterator2++;
        }
      }
    }
    else
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}

void CcXmlNode::clear()
{
  CCNEW(m_pNodeList, CcXmlNodeList);
}

void CcXmlNode::reset()
{
  clear();
  m_bIsOpenTag = false;
  m_eType = CcXmlNode::EType::Unknown;
  m_sData.clear();
}

size_t CcXmlNode::size() const
{
  if (m_pNodeList != nullptr)
    return m_pNodeList->size();
  else
    CCERROR("Tried to get size from not as Node typed XmlNode");
  return 0;
}

CcXmlNode &CcXmlNode::at(size_t i) const
{
  if (m_pNodeList != nullptr)
    return m_pNodeList->at(i);
  else
    CCERROR("Tried to get node from not as Node typed XmlNode");
  return s_oNullNode;
}

CcXmlNodeList& CcXmlNode::remove(size_t iIndex)
{
  if (m_pNodeList != nullptr)
    m_pNodeList->remove(iIndex);
  else
    CCERROR("Tried to removed node from not as Node typed XmlNode");
  return *m_pNodeList;
}

CcXmlNodeList& CcXmlNode::remove(const CcString& sName, size_t iIndex)
{
  if (m_pNodeList != nullptr)
  {
    size_t uiCount = 0;
    size_t uiCountIndex = 0;
    for (const CcXmlNode& rNode : *m_pNodeList)
    {
      if (rNode.getName() == sName)
      {
        if (uiCountIndex == iIndex)
        {
          break;
        }
        else
        {
          uiCountIndex++;
        }
      }
      uiCount++;
    }
    if (m_pNodeList->size() > uiCount)
    {
      m_pNodeList->remove(uiCount);
    }
  }
  else
    CCERROR("Tried to removed node from not as Node typed XmlNode");
  return *m_pNodeList;
}

CcXmlNode& CcXmlNode::append(const CcXmlNode& oAppend)
{
  if (m_pNodeList != nullptr)
  {
    m_pNodeList->append(oAppend);
    m_pLastAddedNode = &m_pNodeList->last();
  }
  else
    CCERROR("Tried to add node to not as Node typed XmlNode");
  return *this;
}

CcXmlNode& CcXmlNode::append(CcXmlNode&& oAppend)
{
  if (m_pNodeList != nullptr)
  {
    m_pNodeList->append(CCMOVE(oAppend));
    m_pLastAddedNode = &m_pNodeList->last();
  }
  else
    CCERROR("Tried to add node to not as Node typed XmlNode");
  return *this;
}

CcXmlNode& CcXmlNode::createSubNode(const CcString& sName)
{
  CcXmlNode oXmlNode(sName);
  append(oXmlNode);
  return getLastAddedNode();
}

CcXmlNode& CcXmlNode::createSubNodeIfNotExists(const CcString& sName, bool *pbWasCreated)
{
  CcXmlNode& rNode = getNode(sName);
  if (rNode.isNotNull())
  {
    if (pbWasCreated != nullptr) *pbWasCreated = false;
    return rNode;
  }
  else
  {
    if (pbWasCreated != nullptr) *pbWasCreated = true;
    return createSubNode(sName);
  }
}

void CcXmlNode::setIsOpenTag(bool bOpenTag)
{
  m_bIsOpenTag = bOpenTag;
  while (size() > 0)
    remove(0);
}

CcXmlNodeList CcXmlNode::getNodes(const CcString& nodeName, bool bRecurse) const
{
  CcXmlNodeList lRet;
  if (nodeName.length() == 0)
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if (pNode.m_eType == CcXmlNode::EType::Node)
      {
        lRet.append(pNode);
        if (bRecurse)
        {
          lRet.append(pNode.getNodes(nodeName, bRecurse));
        }
      }
    }
  }
  else
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if (pNode.m_eType == CcXmlNode::EType::Node)
      {
        if(pNode.m_sData == nodeName)
          lRet.append(pNode);
        if (bRecurse)
          lRet.append(pNode.getNodes(nodeName, bRecurse));
      }
    }
  }
  return lRet;
}

CcXmlNodeList CcXmlNode::getAttributes(const CcString& nodeName) const
{
  CcXmlNodeList lRet;
  if (nodeName.length() == 0)
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if (pNode.getType() == CcXmlNode::EType::Attribute)
        lRet.append(pNode);
    }
  }
  else
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if (pNode.getName() == nodeName)
      {
        if (pNode.getType() == CcXmlNode::EType::Attribute)
          lRet.append(pNode);
      }
    }
  }
  return lRet;
}

CcXmlNode& CcXmlNode::getNode(const CcString& nodeName, size_t nr) const
{
  size_t nrCounter = 0;
  if (nodeName.contains(CcGlobalStrings::Seperators::Slash))
  {
    CcStringList oNodes = nodeName.split(CcGlobalStrings::Seperators::Slash);
    return getNode(oNodes, 0, nr);
  }
  else
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if (pNode.getName() == nodeName)
      {
        if (nrCounter < nr)
        {
          nrCounter++;
        }
        else
        {
          return pNode;
          break;
        }
      }
    }
  }
  return s_oNullNode;
}

CcXmlNode& CcXmlNode::getNode(const CcStringList& oNodeNames, size_t uiCurrentPos, size_t& nr) const
{
  for (CcXmlNode& rNode : getNodeList())
  {
    if (rNode.getName() == oNodeNames[uiCurrentPos])
    {
      if (oNodeNames.size() > uiCurrentPos + 1)
      {
        CcXmlNode& Node = rNode.getNode(oNodeNames, uiCurrentPos + 1, nr);
        if (Node.isNotNull())
        {
          return Node;
        }
      }
      else
      {
        if (nr == 0)
        {
          return rNode;
        }
        else
        {
          nr--;
        }
      }
    }
  }
  return s_oNullNode;
}

CcXmlNodeListIterator CcXmlNode::begin()
{
  return m_pNodeList->begin();
}

CcXmlNodeListIterator CcXmlNode::end()
{
  return m_pNodeList->end();
}

CcString CcXmlNode::innerXml() const
{
  CcString sValue;
  for (CcXmlNode& pNode : getNodeList())
  {
    if (pNode.getType() != CcXmlNode::EType::Attribute &&
        pNode.getType() != CcXmlNode::EType::Unknown)
    {
      sValue += pNode.outerXml();
    }
  }
  return sValue;
}

CcString CcXmlNode::outerXml() const
{
  CcString sValue;
  if (getType() == CcXmlNode::EType::String)
  {
    // Type is String between Tags
    sValue = m_sData;
  }
  else if (getType() == CcXmlNode::EType::Comment)
  {
    sValue << "<!--";
    sValue << m_sData;
    sValue << "-->";
  }
  else if (getType() == CcXmlNode::EType::Doctype)
  {
    sValue << "<!DOCTYPE ";
    sValue << m_sData;
    sValue << ">\r\n";
  }
  else if (getName().length() > 0)
  {
    // Type is a common Tag, write Tag
    sValue << "<";
    sValue << getName();
    CcXmlNodeList oAttributes = getAttributes();
    for (CcXmlNode& rNode : oAttributes)
    {
      sValue << CcGlobalStrings::Space + rNode.getName();
      sValue << "=\"" + rNode.innerText() + "\"";
    }
    if (isOpenTag())
    {
      sValue << " />";
    }
    else
    {
      sValue << ">";
      sValue << innerXml();
      sValue << "</";
      sValue << getName();
      sValue << ">";
    }
  }
  return sValue;
}

CcString CcXmlNode::innerText() const
{
  CcString sValue;
  if (getType() == CcXmlNode::EType::Node ||
      getType() == CcXmlNode::EType::Attribute)
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      sValue << pNode.innerText();
    }
  }
  else
  {
    sValue << m_sData;
  }
  return sValue;
}
