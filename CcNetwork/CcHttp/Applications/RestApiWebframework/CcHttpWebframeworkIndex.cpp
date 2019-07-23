/*
 * This file is part of CcHttpWebframeworkIndex.
 *
 * CcHttpWebframeworkIndex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcHttpWebframeworkIndex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcHttpWebframeworkIndex.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcHttpWebframeworkIndex
 */
#include "CcHttpWebframeworkIndex.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "CcString.h"
#include "CcDeviceList.h"
#include "CcMemoryMonitor.h"
#include "CcGlobalStrings.h"
#include "CcHtml/CcHtmlRoot.h"
#include "CcHtml/CcHtmlDocument.h"
#include "CcHtml/CcHtmlH.h"
#include "CcHtml/CcHtmlDiv.h"
#include "CcHtml/CcHtmlP.h"
#include "CcHtml/CcHtmlScript.h"

using namespace CcHttp::Application::RestApiWebframework;

class CcHttpWebframeworkIndex::CPrivate
{
public:
  static const CcString sScript;
};

const CcString CcHttpWebframeworkIndex::CPrivate::sScript("\
Page_LoadMenu('/api/app/menu');\
Page_LoadFooter('/api/app/footer');\
");

CcHttpWebframeworkIndex::CcHttpWebframeworkIndex()
{
  //CCNEW(m_pPrivate, CPrivate);
  //CCMONITORNEW(m_pPrivate);
  m_oScripts.append("/jquery.js");
}

CcHttpWebframeworkIndex::~CcHttpWebframeworkIndex()
{
  //CCDELETE(m_pPrivate);
}

bool CcHttpWebframeworkIndex::checkResponsible(const CcHttpWorkData& oData) const
{
  bool bIsResponsible = false;
  if (oData.getRequest().getPath() == "/" ||
    oData.getRequest().getPath().startsWith("/index"))
  {
    bIsResponsible = true;
  }
  return bIsResponsible;
}

CcStatus CcHttpWebframeworkIndex::execGet(CcHttpWorkData& oData)
{
  CcStatus oStatus;
  if (bCleared == false)
  {
    bCleared = true;
    CcMemoryMonitor::clear();
  }
  CcHtmlRoot oRootNode;
  oRootNode.setLanguage("en");
  oRootNode.getHeader().setTitle(m_sTitle);
  oRootNode.getHeader().setCharset("utf-8");
  for (const CcString& sScript : m_oScripts)
  {
    oRootNode.getHeader().addScript(sScript);
  }
  for (const CcString& sStylesheet : m_oStylesheets)
  {
    oRootNode.getHeader().addStyleSheet(sStylesheet);
  }
  CcHtmlDocument oDocument(&oRootNode);
  CcHtmlDiv oMenueDiv(&oRootNode.getBody());
  oMenueDiv.setIdAttribute("menu");
  CcHtmlDiv oContentDiv(&oRootNode.getBody());
  oContentDiv.setIdAttribute("content");
  CcHtmlDiv oFooterDiv(&oRootNode.getBody());
  oFooterDiv.setIdAttribute("footer");
  CcHtmlScript oScript(&oRootNode.getBody());
  oScript.setContent(CPrivate::sScript);

  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  oData.writeChunked("<!DOCTYPE html>", sizeof("<!DOCTYPE html>") - 1);
  oDocument.writeHtmlDocument(oData, false);
  return oStatus;
}