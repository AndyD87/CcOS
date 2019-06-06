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

CcHtmlNode::CcHtmlNode(EType Type) :
  m_Type(Type),
  m_bIsOpenTag(false)
{
}

CcHtmlNode::~CcHtmlNode()
{
  m_lAttributes.clear();
  clear();
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
  if (getType() == CcHtmlNode::EType::Node)
  {
    for (CcHtmlNode* pTemp : *this)
    {
      sValue += pTemp->outerHtml();
    }
  }
  return sValue;
}

CcString CcHtmlNode::outerHtml()
{
  CcString sValue;
  if (getType() == CcHtmlNode::EType::String)
  {
    // Type is String between Tags
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
    // Type is a common Tag, write Tag
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

void CcHtmlNode::addAttribute(CcHtmlAttribute *Attribute)
{
  m_lAttributes.append(Attribute);
}
