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
 * @brief     Implementation of Class CcXmlNode
 */
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlNodeList.h"

CcXmlNode s_oNullNode(EXmlNodeType::Unknown);

CcXmlNode::CcXmlNode(EXmlNodeType eNodeType) :
  m_bIsOpenTag(false),
  m_eType(eNodeType)
{
  m_pNodeList = new CcXmlNodeList();
}

CcXmlNode::CcXmlNode(EXmlNodeType eNodeType, const CcString& sName, const CcString& sValue):
  m_bIsOpenTag(false),
  m_sName(sName),
  m_sValue(sValue),
  m_eType(eNodeType)
{
}

CcXmlNode::~CcXmlNode(void)
{
}

CcXmlNode& CcXmlNode::operator=(const CcXmlNode& oToCopy)
{
  m_pNodeList = oToCopy.m_pNodeList;
  m_bIsOpenTag = oToCopy.m_bIsOpenTag;
  m_sName = oToCopy.m_sName;
  m_sValue = oToCopy.m_sValue;
  m_eType = oToCopy.m_eType;
  return *this;
}

CcXmlNode& CcXmlNode::operator=(const CcXmlNode&& oToMove)
{
  if (this != &oToMove)
  {
    m_pNodeList = oToMove.m_pNodeList;
    m_bIsOpenTag = oToMove.m_bIsOpenTag;
    m_sName = oToMove.m_sName;
    m_sValue = oToMove.m_sValue;
    m_eType = oToMove.m_eType;
  }
  return *this;
}

bool CcXmlNode::operator==(const CcXmlNode& oToCompare) const
{
  if (  m_bIsOpenTag  == oToCompare.m_bIsOpenTag  &&
        m_sName       == oToCompare.m_sName       &&
        m_sValue      == oToCompare.m_sValue      &&
        m_eType        == oToCompare.m_eType      &&
        *m_pNodeList == *oToCompare.m_pNodeList   )
  {
    return true;
  }
  return false;
}

void CcXmlNode::reset()
{
  m_bIsOpenTag = false;
  m_eType = EXmlNodeType::Unknown;
  m_sName.clear();
  m_sValue.clear();
  m_pNodeList = new CcXmlNodeList();
}

size_t CcXmlNode::size() const
{
  return m_pNodeList->size();
}

CcXmlNode& CcXmlNode::at(size_t i)
{
  return m_pNodeList->at(i);
}

CcXmlNodeList& CcXmlNode::remove(size_t i)
{
  m_pNodeList->remove(i);
  return *m_pNodeList;
}

CcXmlNode& CcXmlNode::append(const CcXmlNode& oAppend)
{
  m_pNodeList->append(oAppend);
  return *this;
}

CcXmlNode& CcXmlNode::append(CcXmlNode&& oAppend)
{
  m_pNodeList->append(std::move(oAppend));
  return *this;
}

void CcXmlNode::setIsOpenTag(bool bOpenTag)
{
  m_bIsOpenTag = bOpenTag; 
  while (size() > 0) 
    remove(0);
}

CcXmlNodeList CcXmlNode::getNodes(const CcString& nodeName ) const
{
  CcXmlNodeList lRet;
  if (nodeName.length() == 0)
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if( pNode.m_eType == EXmlNodeType::Node)
        lRet.append(pNode);
    }
  }
  else
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if (pNode.getName() == nodeName &&
          pNode.m_eType == EXmlNodeType::Node)
      {
        lRet.append(pNode);
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
      if (pNode.getType() == EXmlNodeType::Attribute)
        lRet.append(pNode);
    }
  }
  else
  {
    for (CcXmlNode& pNode : getNodeList())
    {
      if (pNode.getName() == nodeName)
      {
        if (pNode.getType() == EXmlNodeType::Attribute)
          lRet.append(pNode);
      }
    }
  }
  return lRet;
}

CcXmlNode& CcXmlNode::getNode(const CcString& nodeName, size_t nr) const
{
  size_t nrCounter = 0;
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
  return s_oNullNode;
}

CcString CcXmlNode::innerXml() const
{
  CcString sValue;
  if (getType() == EXmlNodeType::String)
  {
    // Type is String between Tags
    sValue = m_sValue;
  }
  else if (getType() == EXmlNodeType::Comment)
  {
    sValue << "<!--";
    sValue << m_sValue;
    sValue << "-->";
  }
  else if (getType() == EXmlNodeType::Doctype)
  {
    sValue << "<!DOCTYPE";
    sValue << m_sValue;
    sValue << ">";
  }
  else if (getName().length() > 0)
  {
    // Type is a common Tag, write Tag
    sValue << "<";
    sValue << getName();
    CcXmlNodeList oAttributes = getAttributes();
    for (CcXmlNode& rNode : oAttributes)
    {
      sValue << " " + rNode.getName();
      sValue << "=\"" + rNode.getValue() + "\"";
    }
    if (isOpenTag())
    {
      sValue << " />";
    }
    else
    {
      sValue << ">";
      sValue << getValue();
      for (CcXmlNode& pNode : getNodeList())
      {
        if (pNode.getType() != EXmlNodeType::Attribute &&
            pNode.getType() != EXmlNodeType::Unknown)
        {
          sValue += pNode.innerXml();
        }
      }
      sValue << "</";
      sValue << getName();
      sValue << ">";
    }
  }
  return sValue;
}

CcString CcXmlNode::innerText(void) const
{
  CcString sValue;
  for (CcXmlNode& pNode : getNodeList())
  {
    if (pNode.getType() == EXmlNodeType::Node)
      sValue << pNode.getValue();
    else if (!pNode.isOpenTag())
      sValue << pNode.innerText();
  }
  return sValue;
}
