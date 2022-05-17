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
 * @brief     Implementation of Class CcHtmlDocument
 */
#include "Html/CcHtmlDocument.h"
#include "CcStringUtil.h"
#include "CcStringStream.h"
#include "CcGlobalStrings.h"
#include "CcHtmlNodeList.h"

//! Defenition of an intend for html
const CcString CCHTML_INTEND("  ");
//! Defenition for comment start tag
const CcString CCHTML_COMMENT_BEGIN("!--");
//! Defenition for doctype start tag
const CcString CCHTML_DOCTYPE_BEGIN("!DOCTYPE");

CcHtmlDocument::CcHtmlDocument(const CcHtmlNode& rNode) :
  m_bContentValid(false),
  m_pRootNode(rNode)
{
}

CcHtmlDocument::CcHtmlDocument(const CcString& String) :
  m_bContentValid(false)
{
  parseDocument(String);
}

void CcHtmlDocument::parseDocument(const CcString& String)
{
  size_t stringStart = 0;
  m_pRootNode.clear();

  bool bSuccess = true;
  while(bSuccess)
  {
    CcHtmlNode tempNode;
    if (findNode(String, stringStart, tempNode))
    {
      m_pRootNode.append(tempNode);
    }
    else
    {
      bSuccess = false;
    }
  }
  if (m_pRootNode.size() > 0)
    m_bContentValid = true;
  else
    m_bContentValid = false;
}

CcString CcHtmlDocument::getHtmlDocument(bool bIntend)
{
  CcString sString;
  CcStringStream sStream(sString);
  writeHtmlDocument(sStream, bIntend);
  return sString;
}

void CcHtmlDocument::writeHtmlDocument(IIo& rStream, bool bIntend)
{
  outerHtml(m_pRootNode, rStream, bIntend);
}

bool CcHtmlDocument::findAttribute(const CcString& String, size_t &offset, CcHtmlAttribute& rOutAttribute)
{
  bool bRet = false;
  offset = String.posNextNotWhitespace(offset);
  if ( offset != SIZE_MAX )
  {
    if (String[offset] == '>' || String.substr(offset, 2) == "/>")
    {
    }
    else
    {
      bRet = true;
      size_t posEqual = String.find(CcGlobalStrings::Seperators::Equal, offset);
      size_t posWS = String.posNextWhitespace(offset);
      // @todo situation checked> not implemented yet
      if (posWS > posEqual) posWS = posEqual;
      CcString sName(String.substr(offset, posWS - offset));
      rOutAttribute.setName(sName);
      offset = String.posNextNotWhitespace(posWS);
      if (String[offset] == '=')
      {
        CcString sValue;
        offset = String.posNextNotWhitespace(offset+1);
        if (String[offset] == '"')
        {
          offset++;
          size_t posEnd = String.find('"', offset);
          sValue = String.substr(offset, posEnd - offset);
          rOutAttribute.setValue(sValue);
          offset = posEnd + 1;
        }
        else if (String[offset] == '\'')
        {
          offset++;
          size_t posEnd = String.find('\'', offset);
          sValue = String.substr(offset, posEnd - offset);
          rOutAttribute.setValue(sValue);
          offset = posEnd + 1;
        }
        // @todo implement failed html code with opening tag <
        else{
          size_t posEnd = String.posNextWhitespace();
          sValue = String.substr(offset, posEnd - offset);
          rOutAttribute.setValue(sValue);
          offset = posEnd;
        }
      }
    }
    offset = String.posNextNotWhitespace(offset);
  }
  return bRet;
}

void CcHtmlDocument::innerHtml(CcHtmlNode& pNode, IIo& rStream, bool bIntend)
{
  for (CcHtmlNode& pTemp : pNode)
  {
    outerHtml(pTemp, rStream, bIntend);
  }
}

void CcHtmlDocument::outerHtml(CcHtmlNode& pNode, IIo& rStream, bool bIntend)
{
  if (pNode.getType() == CcHtmlNode::EType::String)
  {
    // eType is String between Tags
    rStream << pNode.innerText();
  }
  else if (pNode.getType() == CcHtmlNode::EType::Comment)
  {
    rStream << "<!--" << pNode.innerText() << "-->";
  }
  else if (pNode.getType() == CcHtmlNode::EType::Doctype)
  {
    rStream << "<!DOCTYPE" << pNode.innerText() << ">";
  }
  else if (pNode.getName().length() > 0)
  {
    if (bIntend)
    {
      writeIntends(rStream);
      m_uiIntendLevel++;
    }
    // eType is a common Tag, write Tag
    rStream << "<" << pNode.getName();
    if (pNode.getAttributeCount() > 0)
    {
      for (CcHtmlAttribute& pAttribute : pNode.getAttributeList())
      {
        rStream << CcGlobalStrings::Space << pAttribute.getName() << "=\"" << pAttribute.getValue() << "\"";
      }
    }
    if (pNode.getOpenTag())
    {
      rStream << " />";
    }
    else
    {
      rStream << ">";
      rStream << pNode.innerHtml();
      if (bIntend)
      {
        writeIntends(rStream);
      }
      rStream << "</";
      rStream << pNode.getName();
      rStream << ">";
    }
    if (bIntend)
    {
      m_uiIntendLevel--;
      rStream << CcGlobalStrings::EolShort;
    }
  }
}

void CcHtmlDocument::writeIntends(IIo& rStream)
{
  for (size_t i = 0; i < m_uiIntendLevel; i++)
    for (size_t j = 0; j < m_uiIntendSize; j++)
      rStream << CcGlobalStrings::Space;
}

bool CcHtmlDocument::findNode(const CcString& String, size_t &offset, CcHtmlNode& rOutNode)
{
  bool bRet = false;
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
          parseInnerTag(String, offset, rOutNode);
          if (!rOutNode.getOpenTag())
          {
            bool bSucces = true;
            while (bSucces)
            {
              CcHtmlNode tempNode;
              if (findNode(String, offset, tempNode))
              {
                // if pRet is an open tag, take it's nodes to own
                if (tempNode.getOpenTag())
                {
                  while (tempNode.size())
                  {
                    rOutNode.append(CCMOVE(tempNode.at(0)));
                    tempNode.remove(0);
                  }
                }
                rOutNode.append(tempNode);
              }
              else
              {
                bSucces = false;
              }
            }
            if ( offset < String.length() &&
                  String[offset] == '<' &&
                  String[offset + 1] == '/')
            {
              offset += 2;
              size_t endTagEnd = String.find('>', offset);
              if (endTagEnd != SIZE_MAX)
              {
                CcString endTagName(String.substr(offset, endTagEnd-offset));
                endTagName.trim();
                if (endTagName == rOutNode.getName())
                {
                  offset = endTagEnd +1;
                }
                else
                {
                  rOutNode.setOpenTag(true);
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
    else
    {
      bRet = true;
      CcString sValue;
      while (offset < String.length() &&
        String[offset] != '<')
      {
        sValue << String[offset++];
      }
      rOutNode.setType(CcHtmlNode::EType::String);
      rOutNode.setInnerText(sValue);
    }
  }
  return bRet;
}

void CcHtmlDocument::parseInnerTag(const CcString& String, size_t &offset, CcHtmlNode& rOutNode)
{
  CcString sName;
  bool bDone = false;
  if (String[offset] == '!')
  {
    if (String.substr(offset, CCHTML_COMMENT_BEGIN.length()) == CCHTML_COMMENT_BEGIN)
    {
      rOutNode.setType(CcHtmlNode::EType::Comment);
      offset += CCHTML_COMMENT_BEGIN.length();
      size_t pos = String.find("-->", offset);
      if (pos < SIZE_MAX)
      {
        rOutNode.setInnerText(String.substr(offset, pos - offset));
        offset = pos + CCHTML_COMMENT_BEGIN.length();
      }
    }
    else if (String.substr(offset, CCHTML_DOCTYPE_BEGIN.length()) == CCHTML_DOCTYPE_BEGIN)
    {
      offset += CCHTML_DOCTYPE_BEGIN.length();
      rOutNode.setType(CcHtmlNode::EType::Doctype);
      size_t pos = String.find(">", offset);
      if (pos < SIZE_MAX)
      {
        rOutNode.setInnerText(String.substr(offset, pos - offset));
        offset = pos+1;
      }
    }
    rOutNode.setOpenTag(true);
  }
  else if (String[offset] == '?')
  {
    offset++;
    rOutNode.setType(CcHtmlNode::EType::HtmlVersion);
    size_t pos = String.find("?>", offset);
    if (pos < SIZE_MAX)
    {
      rOutNode.setInnerText(String.substr(offset, pos - offset));
      offset = pos + 2;
    }
    rOutNode.setOpenTag(true);
  }
  else
  {
    while (bDone == false && offset < String.length())
    {
      if (String[offset] == '/')
      {
        // End-SingleTag-Symbol is reached
        rOutNode.setName(sName);
        rOutNode.setOpenTag(true);
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
        bool bSucces = true;
        while (bSucces)
        {
          CcHtmlAttribute tempAttr;
          if (findAttribute(String, offset, tempAttr))
          {
            rOutNode.addAttribute(tempAttr);
          }
          else
          {
            bSucces = false;
          }
        }
        offset = String.posNextNotWhitespace(offset++);
        if (offset != SIZE_MAX)
        {
          if (String[offset] == '/')
          {
            offset++;
            // End-SingleTag-Symbol is reached
            rOutNode.setName(sName);
            rOutNode.setOpenTag(true);
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
        rOutNode.setName(sName);
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
}
