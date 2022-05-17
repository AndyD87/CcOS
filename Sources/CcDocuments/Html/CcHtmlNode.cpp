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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcHtmlNode
 */
#include "Html/CcHtmlNode.h"
#include "Html/CcHtmlNodeList.h"
#include "CcStatic.h"

CcHtmlNode CcHtmlNode::s_oNullNode;

class CcHtmlNode::CPrivate
{
public:
  EType eType = EType::Node;              //!< Current type of this Node
  bool bIsOpenTag = false;                //!< Is this node an OpenTag node?
  CcString sData;                         //!< Value stored in this node, this can be the name if Node is name or content else
  CcVector<CcHtmlAttribute> lAttributes;    //!< Attribute-List.
  CcHtmlNode* pLastAddedNode = nullptr;   //!<
  CcSharedPointer<CcHtmlNodeList> pNodeList;
  size_t uiRefcount = 0;
};

CcHtmlNode::CcHtmlNode(EType eType)
{
  CCNEW(m_pPrivate, CPrivate);
  CCNEW(m_pPrivate->pNodeList, CcHtmlNodeList);
  m_pPrivate->eType = eType;
  m_pPrivate->uiRefcount++;
}

CcHtmlNode::CcHtmlNode(const CcString& sData, EType eType)
{
  CCNEW(m_pPrivate, CPrivate);
  CCNEW(m_pPrivate->pNodeList, CcHtmlNodeList);
  m_pPrivate->eType = eType;
  m_pPrivate->sData = sData;
  m_pPrivate->uiRefcount++;
}

CcHtmlNode::~CcHtmlNode()
{
  if (m_pPrivate)
  {
    m_pPrivate->uiRefcount--;
    if (m_pPrivate->uiRefcount == 0)
    {
      clear();
      CCDELETE(m_pPrivate);
    }
  }
}

CcHtmlNode& CcHtmlNode::operator=(const CcHtmlNode& oToCopy)
{
  CCDELETE(m_pPrivate);
  m_pPrivate = oToCopy.m_pPrivate;
  m_pPrivate->uiRefcount++;
  return *this;
}

bool CcHtmlNode::operator==(const CcHtmlNode& oToCompare) const
{
  if (oToCompare.m_pPrivate->bIsOpenTag == m_pPrivate->bIsOpenTag &&
      oToCompare.m_pPrivate->sData == m_pPrivate->sData &&
      oToCompare.m_pPrivate->lAttributes == m_pPrivate->lAttributes &&
      oToCompare.m_pPrivate->eType == m_pPrivate->eType &&
      oToCompare.m_pPrivate->pNodeList == m_pPrivate->pNodeList
    )
  {
    return true;
  }
  return false;
}

void CcHtmlNode::setName(const CcString& sName)
{
  m_pPrivate->sData = sName;
}

void CcHtmlNode::setInnerText(const CcString& sValue)
{
  if (m_pPrivate->eType == EType::Node)
  {
    clear();
    CcHtmlNode oNode(EType::String);
    append(oNode);
  }
  else
  {
    m_pPrivate->sData = sValue;
  }
}

void CcHtmlNode::setType(EType eType)
{
  m_pPrivate->eType = eType;
}

void CcHtmlNode::setIdAttribute(const CcString& sId)
{
  CcHtmlAttribute& pAttribute = getOrCreateAttribute("id");
  pAttribute.setValue(sId);
}

void CcHtmlNode::setClassAttribute(const CcString& sClass)
{
  CcHtmlAttribute& pAttribute = getOrCreateAttribute("class");
  pAttribute.setValue(sClass);
}

void CcHtmlNode::setNameAttribute(const CcString& sName)
{
  CcHtmlAttribute& pAttribute = getOrCreateAttribute("name");
  pAttribute.setValue(sName);
}

CcHtmlNode::EType CcHtmlNode::getType()
{
  return m_pPrivate->eType;
}

void CcHtmlNode::setOpenTag(bool bOpenTag)
{
  m_pPrivate->bIsOpenTag = bOpenTag;
}

bool CcHtmlNode::getOpenTag()
{
  return m_pPrivate->bIsOpenTag;
}

CcString& CcHtmlNode::getName()
{
  return m_pPrivate->sData;
}

const CcString& CcHtmlNode::getName() const
{
  return m_pPrivate->sData;
}

/**
 * @brief Get List of Attributes stored in List
 * @return Attribues as Vector-List
 */
CcVector<CcHtmlAttribute>& CcHtmlNode::getAttributeList()
{
  return m_pPrivate->lAttributes;
}

void CcHtmlNode::clear()
{
  if (m_pPrivate)
  {
    m_pPrivate->pNodeList->clear();
    m_pPrivate->sData.clear();
    m_pPrivate->lAttributes.clear();
  }
}

size_t CcHtmlNode::size() const
{
  if (m_pPrivate->pNodeList != nullptr)
    return m_pPrivate->pNodeList->size();
  else
    CCERROR("Tried to get size from not as Node typed HtmlNode");
  return 0;
}

CcHtmlNode &CcHtmlNode::at(size_t i) const
{
  if (m_pPrivate->pNodeList != nullptr)
    return m_pPrivate->pNodeList->at(i);
  else
    CCERROR("Tried to get node from not as Node typed HtmlNode");
  return s_oNullNode;
}

bool CcHtmlNode::remove(size_t iIndex)
{
  if (m_pPrivate->pNodeList != nullptr)
    return m_pPrivate->pNodeList->remove(iIndex);
  else
    CCERROR("Tried to removed node from not as Node typed HtmlNode");
  return false;
}

bool CcHtmlNode::remove(const CcString& sName, size_t iIndex)
{
  if (m_pPrivate->pNodeList != nullptr)
  {
    size_t uiCount = 0;
    size_t uiCountIndex = 0;
    for (const CcHtmlNode& rNode : *m_pPrivate->pNodeList)
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
    if (m_pPrivate->pNodeList->size() > uiCount)
    {
      return m_pPrivate->pNodeList->remove(uiCount);
    }
  }
  else
    CCERROR("Tried to removed node from not as Node typed HtmlNode");
  return false;
}

CcHtmlNode& CcHtmlNode::append(const CcHtmlNode& oAppend)
{
  if (m_pPrivate->pNodeList != nullptr)
  {
    m_pPrivate->pNodeList->append(oAppend);
    m_pPrivate->pLastAddedNode = &m_pPrivate->pNodeList->last();
  }
  else
    CCERROR("Tried to add node to not as Node typed HtmlNode");
  return *this;
}

CcHtmlNode& CcHtmlNode::append(CcHtmlNode&& oAppend)
{
  if (m_pPrivate->pNodeList != nullptr)
  {
    m_pPrivate->pNodeList->append(CCMOVE(oAppend));
    m_pPrivate->pLastAddedNode = &m_pPrivate->pNodeList->last();
  }
  else
    CCERROR("Tried to add node to not as Node typed HtmlNode");
  return *this;
}

CcHtmlNode& CcHtmlNode::getNode(const CcString& sName, size_t nr)
{
  size_t nrCounter = 0;
  for (CcHtmlNode& pTemp : *this)
  {
    if (pTemp.getName() == sName)
    {
      if (nrCounter < nr)
      {
        nrCounter++;
      }
      else
      {
        return pTemp;
      }
    }
  }
  return s_oNullNode;
}

CcHtmlNode& CcHtmlNode::getLastAddedNode()
{
  return *m_pPrivate->pLastAddedNode;
}

bool CcHtmlNode::isNull() const
{
  return CcHtmlNode::EType::Unknown == m_pPrivate->eType;
}

CcHtmlNode& CcHtmlNode::createNode(const CcString& sName)
{
  CcHtmlNode oNode(sName);
  append(oNode);
  return getLastAddedNode();
}

CcHtmlNode& CcHtmlNode::createString(const CcString& sContent)
{
  CcHtmlNode oNode(sContent, CcHtmlNode::EType::String);
  append(oNode);
  return getLastAddedNode();
}

CcHtmlNode& CcHtmlNode::getOrCreateNode(const CcString& sName)
{
  CcHtmlNode& pRet = getNode(sName);
  if (pRet.isNull())
  {
    return createNode(sName);
  }
  return pRet;
}

CcHtmlNodeList CcHtmlNode::getNodeList(const CcString& sName, bool bRecursive)
{
  CcHtmlNodeList nlRet;
  for (CcHtmlNode& pNode : *this )
  {
    if (pNode.getName() == sName)
      nlRet.append(pNode);
    if (bRecursive)
    {
      if (pNode.getType() == EType::Node &&
          !pNode.getOpenTag())
      {
        nlRet.append(pNode.getNodeList(sName, bRecursive));
      }
    }
  }
  return nlRet;
}

CcString CcHtmlNode::innerHtml()
{
  CcString sValue;
  for (CcHtmlNode& pTemp : *this)
  {
    sValue += pTemp.outerHtml();
  }
  return sValue;
}

CcString CcHtmlNode::outerHtml()
{
  CcString sValue;
  if (getType() == CcHtmlNode::EType::String)
  {
    // eType is String between Tags
    sValue = m_pPrivate->sData;
  }
  else if (getType() == CcHtmlNode::EType::Comment)
  {
    sValue << "<!--" << m_pPrivate->sData << "-->";
  }
  else if (getType() == CcHtmlNode::EType::Doctype)
  {
    sValue << "<!DOCTYPE" << m_pPrivate->sData << ">";
  }
  else if (getName().length() > 0)
  {
    // eType is a common Tag, write Tag
    sValue << "<" << getName();
    if (getAttributeCount() > 0)
    {
      for (const CcHtmlAttribute& pAttribute : m_pPrivate->lAttributes)
      {
        sValue << CcGlobalStrings::Space << pAttribute.getName() << "=\"" << pAttribute.getValue() << "\"";
      }
    }
    if (getOpenTag())
    {
      sValue << " />";
    }
    else
    {
      sValue << ">";
      sValue += innerHtml();
      sValue << "</";
      sValue << getName();
      sValue << ">";
    }
  }
  return sValue;
}

CcString CcHtmlNode::innerText()
{
  CcString sValue;
  if (this->getType() == EType::String)
  {
    return m_pPrivate->sData;
  }
  else
  {
    for (CcHtmlNode& pNode : *this)
    {
      if (!pNode.getOpenTag())
        sValue << pNode.innerText();
    }
  }
  return sValue;
}

CcHtmlAttribute& CcHtmlNode::getAttribute(const CcString& sName)
{
  for (CcHtmlAttribute& rAttribute : m_pPrivate->lAttributes)
  {
    if (rAttribute.getName() == sName)
    {
      return rAttribute;
    }
  }
  return CcStatic::getNullRef<CcHtmlAttribute>();
}

CcHtmlAttribute& CcHtmlNode::createAttribute(const CcString& sName)
{
  m_pPrivate->lAttributes.append(CcHtmlAttribute(sName));
  return m_pPrivate->lAttributes.last();
}

CcHtmlAttribute& CcHtmlNode::getOrCreateAttribute(const CcString& sName)
{
  CcHtmlAttribute& pAttribute = getAttribute(sName);
  if (CCISNULLREF(pAttribute))
  {
    return createAttribute(sName);
  }
  return pAttribute;
}

void CcHtmlNode::addAttribute(const CcHtmlAttribute& Attribute)
{
  m_pPrivate->lAttributes.append(Attribute);
}

CcHtmlNodeListIterator CcHtmlNode::begin()
{
  return m_pPrivate->pNodeList->begin();
}

CcHtmlNodeListIterator CcHtmlNode::end()
{
  return m_pPrivate->pNodeList->end();
}

CcHtmlNode& CcHtmlNode::getNullNode()
{
  return s_oNullNode;
}
