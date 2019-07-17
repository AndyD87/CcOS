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
#include "CcHtml/CcHtmlRoot.h"
#include "CcHtml/CcHtmlDocument.h"
#include "CcHtml/CcHtmlH.h"
#include "CcHtml/CcHtmlDiv.h"
#include "CcHtml/CcHtmlP.h"
#include "CcHtml/CcHtmlScript.h"

#include "RestApi/CcRestApiSystem.h"
#include "RestApi/CcRestApiApplication.h"
#include "RestApi/CcRestApiMemoryMonitor.h"

#include "CcHttpServer.h"
#include "HttpProvider/CcHttpJqueryProvider.h"
#include "HttpProvider/CcHttpRestApiProvider.h"

#include "CcHttpWebframeworkJsProvider.h"
#include "CcHttpWebframeworkCssProvider.h"
#include "CcHttpWebframeworkIndex.h"
#include "CcUserList.h"
#include "IHttpUserControl.h"

using namespace CcHttp::Application::RestApiWebframework;

class CcHttpWebframework::CPrivate
{
public:
  CPrivate() :
    oJquery("/jquery.js"),
    oRestApi(nullptr, "/api"),
    oRestApiSystem(&oRestApi),
    oRestApiApplication(&oRestApi)
#ifdef MEMORYMONITOR_ENABLED
    , oRestApiMemoryMonitor(&oRestApiSystem)
#endif
  {
  }
  class CBaseProvider : public CcHttpRestApiProvider
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
    IHttpUserControl* m_pUserControl;
  };
  CcString              oServerconfig;
  CcHttpServer          oHttpServer;
  CcHttpJqueryProvider  oJquery;
  CBaseProvider         oRestApi;
  CcRestApiSystem                 oRestApiSystem;
  CcRestApiApplication            oRestApiApplication;
#ifdef MEMORYMONITOR_ENABLED
  CcRestApiMemoryMonitor          oRestApiMemoryMonitor;
#endif
  CcHttpWebframeworkJsProvider    oWebframeworkJs;
  CcHttpWebframeworkCssProvider   oWebframeworkCss;
  CcHttpWebframeworkIndex         oIndex;
};

CcHttpWebframework::CcHttpWebframework()
{
  CCNEW(m_pPrivate, CPrivate);
#if defined(LINUX) &&  defined(DEBUG)
  m_pPrivate->oHttpServer.setPort(10080);
#endif
  m_pPrivate->oHttpServer.registerProvider(&m_pPrivate->oRestApi);
  m_pPrivate->oHttpServer.registerProvider(&m_pPrivate->oJquery);
  m_pPrivate->oHttpServer.registerProvider(&m_pPrivate->oWebframeworkJs);
  m_pPrivate->oIndex.addScript(m_pPrivate->oWebframeworkJs.getPath());
  m_pPrivate->oHttpServer.registerProvider(&m_pPrivate->oWebframeworkCss);
  m_pPrivate->oIndex.addStylesheet(m_pPrivate->oWebframeworkCss.getPath());
  m_pPrivate->oHttpServer.registerProvider(&m_pPrivate->oIndex);
}

CcHttpWebframework::~CcHttpWebframework()
{
  CCDELETE(m_pPrivate);
}

void CcHttpWebframework::run()
{
  m_pPrivate->oHttpServer.start();
  m_pPrivate->oHttpServer.waitForExit();
}

CcRestApiApplication& CcHttpWebframework::getRestApiApplication()
{
  return m_pPrivate->oRestApiApplication;
}

CcRestApiSystem& CcHttpWebframework::getRestApiSystem()
{
  return m_pPrivate->oRestApiSystem;
}

CcHttpWebframeworkIndex& CcHttpWebframework::getIndex()
{
  return m_pPrivate->oIndex;
}

CcHttpServer& CcHttpWebframework::getServer()
{
  return m_pPrivate->oHttpServer;
}

void CcHttpWebframework::setHttpUserControl(IHttpUserControl* pUserControl)
{
  m_pPrivate->oRestApi.setHttpUserControl(pUserControl);
}
