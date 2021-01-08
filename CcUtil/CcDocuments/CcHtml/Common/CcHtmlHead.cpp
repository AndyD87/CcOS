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
 * @brief     Implemtation of class CcHtmlHead
 */
#include "CcHtmlHead.h"

CcHtmlHead::CcHtmlHead(CcHtmlNode& rNode) :
  IHtmlImpl(rNode, "head")
{
}

void CcHtmlHead::setTitle(const CcString& sTitle)
{
  CcHtmlNode& pTitleNode = getNode().getOrCreateNode("title");
  pTitleNode.setInnerText(sTitle);
}

void CcHtmlHead::setIcon(const CcString& sIcon)
{
  CCUNUSED(sIcon);
}

void CcHtmlHead::setCharset(const CcString& sCharset)
{
  CcHtmlNode& pStyle = getNode().createNode("meta");
  pStyle.setOpenTag(true);
  pStyle.createAttribute("charset").setValue(sCharset);
}

CcHtmlNode& CcHtmlHead::addRelCanonical(const CcString& sLink)
{
  CcHtmlNode& pStyle = getNode().createNode("link");
  pStyle.setOpenTag(true);
  pStyle.createAttribute("rel").setValue("canonical");
  pStyle.createAttribute("href").setValue(sLink);
  return pStyle;
}

CcHtmlNode& CcHtmlHead::addMeta(const CcString& sName, const CcString& sContent)
{
  CcHtmlNode& pMeta = getNode().createNode("meta");
  pMeta.setOpenTag(true);
  pMeta.createAttribute("name").setValue(sName);
  if (sContent.length() > 0)
  {
    pMeta.createAttribute("content").setValue(sContent);
  }
  return pMeta;
}

CcHtmlNode& CcHtmlHead::addStyleSheet(const CcString& sLink)
{
  CcHtmlNode& pStyle = getNode().createNode("link");
  pStyle.setOpenTag(true);
  pStyle.createAttribute("rel").setValue("stylesheet");
  pStyle.createAttribute("type").setValue("text/css");
  pStyle.createAttribute("href").setValue(sLink);
  return pStyle;
}

CcHtmlNode& CcHtmlHead::addScript(const CcString& sLink)
{
  CcHtmlNode& pScriptNode = getNode().createNode("script");
  pScriptNode.getOrCreateAttribute("src").setValue(sLink);
  return pScriptNode;
}
