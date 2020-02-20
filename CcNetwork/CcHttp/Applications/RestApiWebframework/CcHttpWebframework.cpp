/*
 * This file is part of CcHttpWebframework.
 *
 * CcHttpWebframework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcHttpWebframework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcHttpWebframework.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcHttpWebframework
 */
#include "CcHttpWebframework.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "CcString.h"
#include "CcDeviceList.h"
#include "CcMemoryMonitor.h"
#include "CcGlobalStrings.h"
#include "CcHtml/Common/CcHtmlRoot.h"
#include "CcHtml/CcHtmlDocument.h"
#include "CcHtml/Common/CcHtmlH.h"
#include "CcHtml/Common/CcHtmlDiv.h"
#include "CcHtml/Common/CcHtmlP.h"
#include "CcHtml/Common/CcHtmlScript.h"
#include "Network/CcCommonPorts.h"

#include "RestApi/CcRestApiSystem.h"
#include "RestApi/CcRestApiApplication.h"

#include "HttpProvider/CcHttpRestApiProvider.h"

#include "CcHttpWebframeworkJsProvider.h"
#include "CcHttpWebframeworkCssProvider.h"
#include "CcHttpWebframeworkIndex.h"
#include "CcUserList.h"
#include "IHttpUserControl.h"

using namespace CcHttp::Application::RestApiWebframework;

class CcHttpSHARED CcHttpWebframework::CPrivate
{
public:
  CPrivate(CcHttpServer* pServer) :
    oRestApi(nullptr, "/api"),
    oRestApiSystem(&oRestApi),
    oRestApiApplication(&oRestApi),
    oRequests(pServer)
  {
  }

  ~CPrivate()
  {
    removeUi();
  }

  class CcHttpSHARED CStatusServerRequests : public CcRestApiApplicationStatus::IPublisher
  {
  public:
    CStatusServerRequests(CcHttpServer* pServer) :
      pServer(pServer)
    {}
    virtual CcStringMap getStatus() override
    {
      CcStringMap oMap;
      oMap.append("HttpRequests", CcString::fromNumber(pServer->getRequestCount()));
      return oMap;
    }
    CcHttpServer* pServer;
  };

  class CcHttpSHARED CBaseProvider : public CcHttpRestApiProvider
  {
  public:
    CBaseProvider(IHttpUserControl* pUserControl, const CcString& sBasePath):
      CcHttpRestApiProvider(sBasePath),
      m_pUserControl(pUserControl)
    { }

    void setHttpUserControl(IHttpUserControl* pUserControl)
    {
      m_pUserControl = pUserControl;
    }

  private:
    virtual bool checkAuth(CcHttpWorkData& oWorkData) override
    {
      CCUNUSED(oWorkData);
      bool bSuccess = false;
      if (m_pUserControl == nullptr)
      {
        bSuccess = true;
      }
      return bSuccess;
    }

    IHttpUserControl*   m_pUserControl;
  };

  void setupUi()
  {
    removeUi();
    CCNEW(pWebframeworkJs , CcHttpWebframeworkJsProvider);
    CCNEW(pWebframeworkCss, CcHttpWebframeworkCssProvider);
    CCNEW(pIndex          , CcHttpWebframeworkIndex);
  }

  void removeUi()
  {
    CCDELETE(pWebframeworkJs);
    CCDELETE(pWebframeworkCss);
    CCDELETE(pIndex);
  }


  CcString                        oServerconfig;
  CBaseProvider                   oRestApi;
  CcRestApiSystem                 oRestApiSystem;
  CcRestApiApplication            oRestApiApplication;
  CcHttpWebframeworkJsProvider*   pWebframeworkJs = nullptr;
  CcHttpWebframeworkCssProvider*  pWebframeworkCss = nullptr;
  CcHttpWebframeworkIndex*        pIndex = nullptr;
  CStatusServerRequests           oRequests;
};

#ifndef CcHttpWebframework_NoUi
const bool CcHttpWebframework::s_bNoUiDefault = false;
#else
const bool CcHttpWebframework::s_bNoUiDefault = true;
#endif

CcHttpWebframework::CcHttpWebframework(bool bNoUi)
{
  CCNEW(m_pPrivate, CPrivate, this);
  setPort(CcCommonPorts::HTTP);
  registerProvider(&m_pPrivate->oRestApi);
#ifndef CcHttpWebframework_NoUi
  if (bNoUi == false)
  {
    m_pPrivate->setupUi();
    registerProvider(m_pPrivate->pWebframeworkJs);
    m_pPrivate->pIndex->addScript(m_pPrivate->pWebframeworkJs->getPath());
    registerProvider(m_pPrivate->pWebframeworkCss);
    m_pPrivate->pIndex->addStylesheet(m_pPrivate->pWebframeworkCss->getPath());

    registerProvider(m_pPrivate->pIndex);
    getRestApiApplication().getStatus().appendPublisher(&m_pPrivate->oRequests);
  }
#endif
}

CcHttpWebframework::~CcHttpWebframework()
{
  CCDELETE(m_pPrivate);
}

CcRestApiApplication& CcHttpWebframework::getRestApiApplication()
{
  return m_pPrivate->oRestApiApplication;
}

CcRestApiSystem& CcHttpWebframework::getRestApiSystem()
{
  return m_pPrivate->oRestApiSystem;
}

CcHttpWebframeworkIndex* CcHttpWebframework::getIndex()
{
  return m_pPrivate->pIndex;
}

void CcHttpWebframework::setHttpUserControl(IHttpUserControl* pUserControl)
{
  m_pPrivate->oRestApi.setHttpUserControl(pUserControl);
}
