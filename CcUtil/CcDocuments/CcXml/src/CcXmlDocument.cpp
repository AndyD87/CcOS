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
 * @brief     Implementation of Class CcXmlDocument
 */
#include "CcXml/CcXmlDocument.h"
#include "CcStringUtil.h"
#include "CcXml/CcXmlNodeList.h"
#include "CcGlobalStrings.h"

const CcString c_sINTEND        ("  ");
const CcString c_sCOMMENT_BEGIN ("!--");
const CcString c_sCOMMENT_END   ("-->");
const CcString c_sDOCTYPE_BEGIN ("!DOCTYPE");
const CcString c_sDOCTYPE_END   (">");
const CcString c_sNODE_BEGIN    ("<");
const CcString c_sNODE_END      ("<");

CcXmlDocument::CcXmlDocument(CcXmlNode &Node) :
m_RootNode(Node)
{

}

CcXmlDocument::CcXmlDocument(const CcString& String) :
m_RootNode(EXmlNodeType::Node)
{
  parseDocument(String);
}

CcXmlDocument::~CcXmlDocument(void)
{
}


bool CcXmlDocument::parseDocument(const CcString& String)
{
  m_RootNode.reset();
  m_RootNode.setType(EXmlNodeType::Node);
  size_t stringStart = 0;
  CcString sDocumentCopy = String;
  CcXmlNode oNode;
  while (findNode(sDocumentCopy, stringStart, oNode))
  {
    m_RootNode.setType(EXmlNodeType::Node);
    m_RootNode.append(oNode);
    oNode.reset();
  }
  return m_RootNode.size() > 0;
}

CcString &CcXmlDocument::getDocument(bool bIntend)
{
  m_bIntend = bIntend;
  if (m_RootNode.size() > 0)
  {
    m_sContent.clear();
    m_uiIntendLevel = 0;
    for (CcXmlNode& pNode : m_RootNode.nodeList())
    {
      writeInnerXml(pNode);
    }
  }
  return m_sContent;
}

void CcXmlDocument::writeInnerXml(const CcXmlNode& oInNode)
{
  if (oInNode.getType() == EXmlNodeType::String)
  {
    // Type is String between Tags
    m_sContent = oInNode.getValue();
  }
  else if (oInNode.getType() == EXmlNodeType::Comment)
  {
    writeIntend();
    m_sContent << "<!--" << oInNode.getValue() << "-->";
    writeNewLine();
  }
  else if (oInNode.getType() == EXmlNodeType::Doctype)
  {
    writeIntend();
    m_sContent << "<!DOCTYPE" << oInNode.getValue() << ">";
    writeNewLine();
  }
  else if (oInNode.getName().length() > 0)
  {
    writeIntend();
    // Type is a common Tag, write Tag
    m_sContent << "<";
    m_sContent << oInNode.getName();
    CcXmlNodeList oAttributes = oInNode.getAttributes();
    for (CcXmlNode& rNode : oAttributes)
    {
      m_sContent << " " << rNode.getName() << "=\"" << rNode.getValue() + "\"";
    }
    if (oInNode.isOpenTag())
    {
      m_sContent << " />";
      writeNewLine();
    }
    else
    {
      m_sContent << ">";
      m_uiIntendLevel++;
      bool bLineWritten = false;
      m_sContent << oInNode.getValue();
      for (CcXmlNode& rNode : oInNode.getNodeList())
      {
        if (rNode.getType() != EXmlNodeType::Attribute &&
            rNode.getType() != EXmlNodeType::Unknown)
        {
          if (bLineWritten == false)
          {
            writeNewLine(); 
            bLineWritten = true;
          }
          writeInnerXml(rNode);
        }
      }
      m_uiIntendLevel--;
      if (bLineWritten)
        writeIntend();
      m_sContent << "</" << oInNode.getName() << ">";
      writeNewLine();
    }
  }
}

bool CcXmlDocument::findAttribute(const CcString& String, size_t &offset, CcXmlNode& oOutNode)
{
  bool bRet = false;
  offset = String.posNextNotWhitespace(offset);
  if (offset != SIZE_MAX)
  {
    if (String[offset] == '>' || String.substr(offset, 2) == "/>")
    {
    }
    else
    {
      bRet = true;
      oOutNode.setType(EXmlNodeType::Attribute);
      size_t posEqual = String.find("=", offset);
      size_t posWS = String.posNextWhitespace(offset);
      // @todo situation checked> not implemented yet
      if (posWS > posEqual) posWS = posEqual;
      CcString sName(String.substr(offset, posWS - offset));
      oOutNode.setName(sName);
      offset = String.posNextNotWhitespace(posWS);
      if (String[offset] == '=')
      {
        CcString sValue;
        offset = String.posNextNotWhitespace(offset + 1);
        if (String[offset] == '"')
        {
          offset++;
          size_t posEnd = String.find('"', offset);
          sValue = String.substr(offset, posEnd - offset);
          oOutNode.setValue(sValue);
          offset = posEnd + 1;
        }
        else if (String[offset] == '\'')
        {
          offset++;
          size_t posEnd = String.find('\'', offset);
          sValue = String.substr(offset, posEnd - offset);
          oOutNode.setValue(sValue);
          offset = posEnd + 1;
        }
        // @todo send exception or warning
        else{
          
        }
      }
    }
    offset = String.posNextNotWhitespace(offset);
  }
  return bRet;
}

void CcXmlDocument::writeIntend()
{
  for (uint16 i = 0; i<m_uiIntendLevel && m_bIntend; i++)
  {
    m_sContent << c_sINTEND;
  }
}


void CcXmlDocument::writeNewLine()
{
  if (m_bIntend)
  {
    m_sContent << CcGlobalStrings::EolLong;
  }
}

bool CcXmlDocument::findNode(const CcString& String, size_t &offset, CcXmlNode& oOutNode)
{
  bool bRet = false;
  offset = String.posNextNotWhitespace(offset);
  if (offset < String.length())
  {
    if (String[offset] == '<')
    {
      offset++;
      offset = String.posNextNotWhitespace(offset);
      if (offset != SIZE_MAX)
      {
        if (String[offset] == '/')
        {
          offset--;
        }
        else
        {
          bRet = true;
          oOutNode.setType(EXmlNodeType::Node);
          parseInnerTag(String, offset, oOutNode);
          if (!oOutNode.isOpenTag())
          {
            CcXmlNode tempNode;
            while (findNode(String, offset, tempNode))
            {
              oOutNode.append(tempNode);
              tempNode.reset();
            }
            offset = String.posNextNotWhitespace(offset);
            if (offset < String.length() && String[offset] != '<')
            {
              CcString sValue;
              size_t pos = String.find('<', offset);
              if (pos != SIZE_MAX)
              {
                sValue = String.substr(offset, pos-offset);
                offset = pos;
                oOutNode.setValue(sValue.trim());
              }
            }
            if (String.substr(offset, 2) == "</")
            {
              offset += 2;
              size_t endTagEnd = String.find('>', offset);
              if (endTagEnd != SIZE_MAX)
              {
                CcString endTagName = String.substr(offset, endTagEnd - offset);
                endTagName.trim();
                if (endTagName == oOutNode.getName())
                {
                  offset = endTagEnd + 1;
                }
                else
                {
                  oOutNode.setIsOpenTag(true);
                  offset -= 2;
                }
              }
              else
              {
                offset -= 2;
              }
            }
          }
        }
      }
    }
  }
  return bRet;
}

bool CcXmlDocument::parseInnerTag(const CcString& String, size_t &offset, CcXmlNode& oOutNode)
{
  bool bRet = false;
  CcString sName;
  bool bDone = false;
  if (String.isStringAtOffset(c_sCOMMENT_BEGIN, offset))
  {
    offset += c_sCOMMENT_BEGIN.length();
    size_t pos = String.find(c_sCOMMENT_END, offset);
    if (pos < SIZE_MAX)
    {
      oOutNode.setValue(String.substr(offset, pos - offset));
      offset = pos + c_sCOMMENT_BEGIN.length();
    }
    oOutNode.setIsOpenTag(true);
    oOutNode.setType(EXmlNodeType::Comment);
  }
  else if (String[offset] == '?')
  {
    offset++;
    size_t pos = String.find("?>", offset);
    if (pos < SIZE_MAX)
    {
      oOutNode.setValue(String.substr(offset, pos - offset));
      offset = pos + 2;
    }
    oOutNode.setIsOpenTag(true);
    oOutNode.setType(EXmlNodeType::Doctype);
  }
  else
  {
    while (bDone == false && offset < String.length())
    {
      if (String[offset] == '/')
      {
        // End-SingleTag-Symbol is reached
        oOutNode.setName(sName);
        oOutNode.setIsOpenTag(true);
        offset = String.posNextNotWhitespace(offset++);
        bDone = true;
        if (offset != SIZE_MAX)
        {
          if (String[offset] == '>')
          {
            offset++;
          }
        }
      }
      else if (CcStringUtil::isWhiteSpace(String[offset]))
      {
        // Tag has Attributes;
        CcXmlNode tempAttr;
        while (findAttribute(String, offset, tempAttr))
        {
          oOutNode.append(tempAttr);
          tempAttr.reset();
        }
        offset = String.posNextNotWhitespace(offset++);
        if (offset != SIZE_MAX)
        {
          if (String[offset] == '/')
          {
            offset++;
            // End-SingleTag-Symbol is reached
            oOutNode.setName(sName);
            oOutNode.setIsOpenTag(true);
            offset = String.posNextNotWhitespace(offset);
            bDone = true;
            if (offset != SIZE_MAX)
            {
              if (String[offset] == '>')
              {
                offset++;
              }
            }
          }
        }
      }
      else if (String[offset] == '>')
      {
        oOutNode.setName(sName);
        offset++;
        bDone = true;
      }
      else
      {
        sName << String[offset];
        offset++;
      }
    }
  }
  return bRet;
}
