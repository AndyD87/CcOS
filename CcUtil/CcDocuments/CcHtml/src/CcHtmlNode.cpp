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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcHtmlNode
 */
#include "CcHtml/CcHtmlNode.h"
#include "CcHtml/CcHtmlNodeList.h"

CcHtmlNode::CcHtmlNode(eType Type) :
  m_Type(Type),
  m_bIsOpenTag(false)
{
  setNameByType(Type);
}

CcHtmlNode::~CcHtmlNode( void )
{
  m_lAttributes.clearForce();
  clearForce();
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

CcHtmlNodeList CcHtmlNode::getNodeList(const CcString& sName)
{
  CcHtmlNodeList nlRet;
  for (CcHtmlNode* pNode : *this )
  {
    if (pNode->getName() == sName)
      nlRet.append(pNode);
  }
  return nlRet;
}

CcString CcHtmlNode::innerHtml()
{
  CcString sValue;
  if (getType() == CcHtmlNode::eString)
  {
    // Type is String between Tags
    sValue = m_sValue;
  }
  else if (getType() == CcHtmlNode::eComment)
  {
    sValue << "<!--" << m_sValue << "-->";
  }
  else if (getType() == CcHtmlNode::eDoctype)
  {
    sValue << "<!DOCTYPE" << m_sValue << ">";
  }
  else if (getName().length() > 0)
  {
    // Type is a common Tag, write Tag
    sValue << "<" << getName();
    if (getAttributeCount() > 0)
    {
      CcList<CcHtmlAttribute*> lAttributes = getAttributeList();
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
      sValue << m_sValue;
      for (CcHtmlNode* pTemp : *this )
      {
        sValue += pTemp->getValue();
      }
      sValue << "</";
      sValue << getName();
      sValue << ">";
    }
  }
  return sValue;
}

CcString CcHtmlNode::innerText(void)
{
  CcString sValue;
  if (this->getType() == eString)
  { 
    return m_sValue;
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

void CcHtmlNode::setNameByType(eType Type)
{
  switch(Type)
  {
    case eA:
      m_sName = "a";
      m_bIsOpenTag = false;
      break;
    case eB:
      m_sName = "b";
      m_bIsOpenTag = false;
      break;
    case eP:
      m_sName = "P";
      m_bIsOpenTag = false;
      break;
    case eBr:
      m_sName = "br";
      m_bIsOpenTag = true;
      break;
    case eHtml:
      m_sName = "html";
      m_bIsOpenTag = false;
      break;
    case eHead:
      m_sName = "head";
      m_bIsOpenTag = false;
      break;
    case eBody:
      m_sName = "body";
      m_bIsOpenTag = false;
      break;
    case eForm:
      m_sName = "form";
      m_bIsOpenTag = false;
      break;
    case eIframe:
      m_sName = "iframe";
      m_bIsOpenTag = false;
      break;
    case eTable:
      m_sName = "table";
      m_bIsOpenTag = false;
      break;
    case eTr:
      m_sName = "tr";
      m_bIsOpenTag = false;
      break;
    case eTd:
      m_sName = "td";
      m_bIsOpenTag = false;
      break;
    case eTbody:
      m_sName = "tbody";
      m_bIsOpenTag = false;
      break;
    case eScript:
      m_sName = "script";
      m_bIsOpenTag = false;
      break;
    case eMeta:
      m_sName = "meta";
      m_bIsOpenTag = true;
      break;
    case eH1:
      m_sName = "h1";
      m_bIsOpenTag = false;
      break;
    case eH2:
      m_sName = "h2";
      m_bIsOpenTag = false;
      break;
    case eH3:
      m_sName = "h3";
      m_bIsOpenTag = false;
      break;
    default:
      m_sName = "";
      m_bIsOpenTag = false;
  }
}
