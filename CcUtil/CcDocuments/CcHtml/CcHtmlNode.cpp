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
 * @brief     Implementation of Class CcHtmlNode
 */
#include "CcHtml/CcHtmlNode.h"
#include "CcHtml/CcHtmlNodeList.h"

CcHtmlNode::CcHtmlNode(EType eType) :
  m_eType(eType)
{
}

CcHtmlNode::CcHtmlNode(CcHtmlNode* pParent, EType eType) :
  m_eType(eType),
  m_pParent(pParent)
{
  if (pParent != nullptr)
  {
    pParent->add(this);
  }
}

CcHtmlNode::CcHtmlNode(CcHtmlNode* pParent, const CcString& sData, EType eType) :
  m_eType(eType),
  m_pParent(pParent),
  m_sData(sData)
{
  if (pParent != nullptr)
  {
    pParent->add(this);
  }
}

CcHtmlNode::~CcHtmlNode()
{
  m_lAttributes.clear();
  clear();
  if (m_pCreatedNodes != nullptr)
  {
    for (CcHtmlNode* pNode : *m_pCreatedNodes)
    {
      CCDELETE(pNode);
    }
    m_pCreatedNodes->clear();
    CCDELETE(m_pCreatedNodes);
  }
  if (m_pGeneratedAttributes != nullptr)
  {
    for (CcHtmlAttribute* pAttribute : *m_pGeneratedAttributes)
    {
      CCDELETE(pAttribute);
    }
    m_pGeneratedAttributes->clear();
    CCDELETE(m_pGeneratedAttributes);
  }
  if (m_pParent != nullptr)
  {
    m_pParent->removeItem(this);
  }
}

void CcHtmlNode::setInnerText(const CcString &sValue)
{
  if (m_eType == EType::Node)
  {
    clear();
    CcHtmlNode* pInnerNode = new CcHtmlNode(this, EType::String);
    getCreatedNodes() += pInnerNode;
    pInnerNode->setInnerText(sValue);
  }
  else
  {
    m_sData = sValue;
  }
}

void CcHtmlNode::setIdAttribute(const CcString& sId)
{
  CcHtmlAttribute* pAttribute = getOrCreateAttribute("id");
  if (pAttribute)
  {
    pAttribute->setValue(sId);
  }
}

void CcHtmlNode::setClassAttribute(const CcString& sClass)
{
  CcHtmlAttribute* pAttribute = getOrCreateAttribute("class");
  if (pAttribute)
  {
    pAttribute->setValue(sClass);
  }
}

void CcHtmlNode::setNameAttribute(const CcString& sName)
{
  CcHtmlAttribute* pAttribute = getOrCreateAttribute("name");
  if (pAttribute)
  {
    pAttribute->setValue(sName);
  }
}

CcHtmlNode* CcHtmlNode::getNode(const CcString& sName, size_t nr)
{
  CcHtmlNode *ret = nullptr;
  size_t nrCounter = 0;
  for (CcHtmlNode* pTemp : *this)
  {
    if (pTemp->getName() == sName)
    {
      if (nrCounter < nr)
      {
        nrCounter++;
      }
      else
      {
        ret = pTemp;
        break;
      }
    }
  }
  return ret;
}

CcHtmlNode* CcHtmlNode::createNode(const CcString& sName)
{
  CcHtmlNode* pRet = new CcHtmlNode(this, sName);
  CCMONITORNEW(pRet);
  getCreatedNodes() += pRet;
  return pRet;
}

CcHtmlNode* CcHtmlNode::getOrCreateNode(const CcString& sName)
{
  CcHtmlNode *pRet = getNode(sName);
  if (pRet == nullptr)
  {
    pRet = createNode(sName);
  }
  return pRet;
}

CcHtmlNodeList CcHtmlNode::getNodeList(const CcString& sName, bool bRecursive)
{
  CcHtmlNodeList nlRet;
  for (CcHtmlNode* pNode : *this )
  {
    if (pNode->getName() == sName)
      nlRet.append(pNode);
    if (bRecursive)
    {
      if (pNode->getType() == EType::Node &&
          !pNode->getOpenTag())
      {
        nlRet.append(pNode->getNodeList(sName, bRecursive));
      }
    }
  }
  return nlRet;
}

CcString CcHtmlNode::innerHtml()
{
  CcString sValue;
  for (CcHtmlNode* pTemp : *this)
  {
    sValue += pTemp->outerHtml();
  }
  return sValue;
}

CcString CcHtmlNode::outerHtml()
{
  CcString sValue;
  if (getType() == CcHtmlNode::EType::String)
  {
    // eType is String between Tags
    sValue = m_sData;
  }
  else if (getType() == CcHtmlNode::EType::Comment)
  {
    sValue << "<!--" << m_sData << "-->";
  }
  else if (getType() == CcHtmlNode::EType::Doctype)
  {
    sValue << "<!DOCTYPE" << m_sData << ">";
  }
  else if (getName().length() > 0)
  {
    // eType is a common Tag, write Tag
    sValue << "<" << getName();
    if (getAttributeCount() > 0)
    {
      CcVector<CcHtmlAttribute*> lAttributes = getAttributeList();
      for (CcHtmlAttribute* pAttribute : lAttributes)
      {
        sValue << " " << pAttribute->getName() << "=\"" << pAttribute->getValue() << "\"";
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
    return m_sData;
  }
  else
  {
    for (CcHtmlNode* pNode : *this)
    {
      if (!pNode->getOpenTag())
        sValue << pNode->innerText();
    }
  }
  return sValue;
}

CcHtmlAttribute* CcHtmlNode::getAttribute(const CcString& sName)
{
  CcHtmlAttribute* pRet = nullptr;
  for (size_t i = 0; i < m_lAttributes.size(); i++)
  {
    if (m_lAttributes.at(i)->getName() == sName)
    {
      pRet = m_lAttributes.at(i);
      break;
    }
  }
  return pRet;
}

CcHtmlAttribute* CcHtmlNode::createAttribute(const CcString& sName)
{
  CcHtmlAttribute* pAttribute = new CcHtmlAttribute(sName);
  CCMONITORNEW(pAttribute);
  getAttributeList().append(pAttribute);
  getCreatedAttributes().append(pAttribute);
  return pAttribute;
}

CcHtmlAttribute* CcHtmlNode::getOrCreateAttribute(const CcString& sName)
{
  CcHtmlAttribute* pAttribute = getAttribute(sName);
  if (pAttribute == nullptr)
  {
    pAttribute = createAttribute(sName);
  }
  return pAttribute;
}

void CcHtmlNode::addAttribute(CcHtmlAttribute *Attribute)
{
  m_lAttributes.append(Attribute);
}

CcHtmlNodeList& CcHtmlNode::getCreatedNodes()
{
  if (m_pCreatedNodes == nullptr)
  {
    m_pCreatedNodes = new CcHtmlNodeList();
  }
  return *m_pCreatedNodes;
}

CcVector<CcHtmlAttribute*>& CcHtmlNode::getCreatedAttributes()
{
  if (m_pGeneratedAttributes == nullptr)
  {
    m_pGeneratedAttributes = new CcVector<CcHtmlAttribute*>();
  }
  return *m_pGeneratedAttributes;
}
