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
 * @brief     Implementation of Class CcHtmlDocument
 */
#include "CcHtml/CcHtmlDocument.h"
#include "CcStringUtil.h"

const CcString CCHTML_INTEND("  ");
const CcString CCHTML_COMMENT_BEGIN("!--");
const CcString CCHTML_DOCTYPE_BEGIN("!DOCTYPE");

CcHtmlDocument::CcHtmlDocument(CcHtmlNode *Node) :
  m_bContentValid(false),
  m_RootNode(Node)
{
}

CcHtmlDocument::CcHtmlDocument(const CcString& String) :
  m_bContentValid(false),
  m_RootNode(nullptr)
{
  parseDocument(String);
}

CcHtmlDocument::~CcHtmlDocument( void )
{
  CCDELETE(m_RootNode);
}


void CcHtmlDocument::parseDocument(const CcString& String)
{
  size_t stringStart = 0;
  m_RootNode = new CcHtmlNode(); 
  CCMONITORNEW(m_RootNode);
  CcHtmlNode* tempNode;
  while((tempNode = findNode(String, stringStart) ) != nullptr)
  {
    m_RootNode->addNode(tempNode);
  }
  if (m_RootNode->size() > 0)
    m_bContentValid = true;
  else
    m_bContentValid = false;
}

CcString &CcHtmlDocument::getHtmlDocument(bool bIntend)
{
  CCUNUSED(bIntend);
  for (CcHtmlNode *tempNode : *m_RootNode )
  {
    m_sContent << tempNode->innerHtml();
  }
  return m_sContent;
}

void CcHtmlDocument::appendIntend(uint16 level)
{
  for (uint16 i = 0; i<level; i++)
  {
    m_sContent << CCHTML_INTEND;
  }
}

CcHtmlAttribute* CcHtmlDocument::findAttribute(const CcString& String, size_t &offset)
{
  CcHtmlAttribute *pRet = nullptr;
  offset = String.posNextNotWhitespace(offset);
  if ( offset != SIZE_MAX )
  {
    if (String[offset] == '>' || String.substr(offset, 2) == "/>")
    {
    }
    else
    {
      pRet = new CcHtmlAttribute(); 
      CCMONITORNEW(pRet);
      size_t posEqual = String.find("=", offset);
      size_t posWS = String.posNextWhitespace(offset);
      // @todo situation checked> not implemented yet
      if (posWS > posEqual) posWS = posEqual;
      CcString sName(String.substr(offset, posWS - offset));
      pRet->setName(sName);
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
          pRet->setValue(sValue);
          offset = posEnd + 1;
        }
        else if (String[offset] == '\'')
        {
          offset++;
          size_t posEnd = String.find('\'', offset);
          sValue = String.substr(offset, posEnd - offset);
          pRet->setValue(sValue);
          offset = posEnd + 1;
        }
        // @todo implement failed html code with opening tag <
        else{
          size_t posEnd = String.posNextWhitespace();
          sValue = String.substr(offset, posEnd - offset);
          pRet->setValue(sValue);
          offset = posEnd;
        }
      }
    }
    offset = String.posNextNotWhitespace(offset);
  }
  return pRet;
}

CcHtmlNode* CcHtmlDocument::findNode(const CcString& String, size_t &offset)
{
  CcHtmlNode *pRet = nullptr;
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
          pRet = parseInnerTag(String, offset);
          if (!pRet->getOpenTag())
          {
            CcHtmlNode *tempNode;
            while ((tempNode = findNode(String, offset)) != nullptr)
            {
              pRet->addNode(tempNode);
              tempNode->setParent(pRet);
              // if pRet is an open tag, take it's nodes to own
              if (tempNode->getOpenTag())
              {
                CcHtmlNode *moveNode;
                while(tempNode->size())
                {
                  moveNode = tempNode->at(0);
                  pRet->addNode(moveNode);
                  tempNode->remove(0);
                }
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
                endTagName = endTagName.trim();
                if (endTagName == pRet->getName())
                {
                  offset = endTagEnd +1;
                }
                else
                {
                  pRet->setOpenTag(true);
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
      CcString sValue;
      pRet = new CcHtmlNode(); 
      CCMONITORNEW(pRet);
      while (offset < String.length() &&
        String[offset] != '<')
      {
        sValue << String[offset++];
      }
      pRet->setType(CcHtmlNode::eString);
      pRet->setValue(sValue);
    }
  }
  return pRet;
}

CcHtmlNode* CcHtmlDocument::parseInnerTag(const CcString& String, size_t &offset)
{
  CcHtmlNode *pRet = new CcHtmlNode(); 
  CCMONITORNEW(pRet);
  CcString sName;
  bool bDone = false;
  if (String[offset] == '!')
  {
    if (String.substr(offset, CCHTML_COMMENT_BEGIN.length()) == CCHTML_COMMENT_BEGIN)
    {
      pRet->setType(CcHtmlNode::eComment);
      offset += CCHTML_COMMENT_BEGIN.length();
      size_t pos = String.find("-->", offset);
      if (pos < SIZE_MAX)
      {
        pRet->setValue(String.substr(offset, pos - offset));
        offset = pos + CCHTML_COMMENT_BEGIN.length();
      }
    }
    else if (String.substr(offset, CCHTML_DOCTYPE_BEGIN.length()) == CCHTML_DOCTYPE_BEGIN)
    {
      offset += CCHTML_DOCTYPE_BEGIN.length();
      pRet->setType(CcHtmlNode::eDoctype);
      size_t pos = String.find(">", offset);
      if (pos < SIZE_MAX)
      {
        pRet->setValue(String.substr(offset, pos - offset));
        offset = pos+1;
      }
    }
    pRet->setOpenTag(true);
  }
  else
  {
    while (bDone == false && offset < String.length())
    {
      if (String[offset] == '/')
      {
        // End-SingleTag-Symbol is reached
        pRet->setName(sName);
        pRet->setOpenTag(true);
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
        CcHtmlAttribute *tempAttr;
        while ((tempAttr = findAttribute(String, offset)) != nullptr)
          pRet->addAttribute(tempAttr);
        offset = String.posNextNotWhitespace(offset++);
        if (offset != SIZE_MAX)
        {
          if (String[offset] == '/')
          {
            offset++;
            // End-SingleTag-Symbol is reached
            pRet->setName(sName);
            pRet->setOpenTag(true);
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
        pRet->setName(sName);
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
  return pRet;
}
