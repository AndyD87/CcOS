/*
 * This file is part of CcRemoteDeviceServer.
 *
 * CcRemoteDeviceServer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceServer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceServer.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceServer
 */
#include "CcRemoteDeviceServer.h"
#include "CcKernel.h"
#include "CcHttpServer.h"
#include "CcHttpServerConfig.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcRemoteDeviceCssProvider.h"
#include "CcRemoteDeviceJsProvider.h"
#include "RestApi/CcRestApiApplication.h"
#include "Applications/RestApiWebframework/CcHttpWebframeworkIndex.h"
#include "CcFile.h"
#include "Devices/IWlan.h"
#include "Devices/IWlanAccessPoint.h"
#include "Devices/IWlanClient.h"

using namespace CcHttp::Application::RestApiWebframework;

class CcRemoteDeviceServer::CPrivate
{
public:
  CPrivate()
  {
  }
  ~CPrivate()
  {
  }
  CcRemoteDeviceJsProvider* pJsProvider = nullptr;
  CcRemoteDeviceCssProvider* pCssProvider = nullptr;
  CcHandle<IWlan> pWlanDevice = nullptr;
};

CcRemoteDeviceServer::CcRemoteDeviceServer(bool bNoUi) :
  CcHttpWebframework(bNoUi),
  m_oDirectories(CcRemoteDeviceGlobals::ProjectName, true)
{
  CCNEW(m_pPrivate, CPrivate);
  m_oConfig.init();
  if(bNoUi == false)
  {
    CCNEW(m_pPrivate->pJsProvider, CcRemoteDeviceJsProvider);
    registerProvider(m_pPrivate->pJsProvider);
    CCNEW(m_pPrivate->pCssProvider, CcRemoteDeviceCssProvider);
    registerProvider(m_pPrivate->pCssProvider);
    getRestApiApplication().getMenu().append("Home", "/api/app/status");
    getRestApiApplication().getMenu().append("Gpio", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::GpioPin));
    getRestApiApplication().getMenu().append("Network", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::Network));
    getRestApiApplication().getMenu().append("WlanClient", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::WlanClient));
    getRestApiApplication().getMenu().append("WlanAccessPoint", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::WlanAccessPoint));

    if(getIndex())
    {
      getIndex()->addScript(m_pPrivate->pJsProvider->getPath());
      getIndex()->addStylesheet(m_pPrivate->pCssProvider->getPath());
    }
  }
}

CcRemoteDeviceServer::~CcRemoteDeviceServer()
{
  CCDELETE(m_pPrivate->pJsProvider);
  CCDELETE(m_pPrivate->pCssProvider);
  CCDELETE(m_pPrivate);
}

void CcRemoteDeviceServer::run()
{
  setupWlan();
  CcHttpServer::getConfig().getAddressInfo().setPort(CcCommonPorts::CcRemoteDevice);
  CcHttpServer::getConfig().setSslEnabled(true);
  m_oDirectories.createAllPaths();
  if(CcHttpServer::getConfig().getSslCertificate().length() == 0)
  {
    CcString sPath = m_oDirectories.getDataDir();
    CcHttpServer::getConfig().setSslCertificate(sPath.appendPath(CcRemoteDeviceGlobals::Defaults::SslCertificateFilename));
  }
  if(CcHttpServer::getConfig().getSslKey().length() == 0)
  {
    CcString sPath = m_oDirectories.getDataDir();
    CcHttpServer::getConfig().setSslKey(sPath.appendPath(CcRemoteDeviceGlobals::Defaults::SslKeyFilename));
  }
#ifdef GENERIC
  CcKernel::sleep(10000);
#endif
  CcHttpServer::run();
}

void CcRemoteDeviceServer::setupWlan()
{
  m_pPrivate->pWlanDevice = CcKernel::getDevice(EDeviceType::Wlan).cast<IWlan>();
  if( m_oConfig.oWlan.bServerEnabled &&
      m_pPrivate->pWlanDevice->getAccessPoint())
  {
    m_pPrivate->pWlanDevice->getAccessPoint()->setCredentials(m_oConfig.oWlan.sServerSsid, m_oConfig.oWlan.oServerPassword.getString());
    m_pPrivate->pWlanDevice->getAccessPoint()->start();
  }
  if(m_pPrivate->pWlanDevice->getClient())
  {
    m_pPrivate->pWlanDevice->getClient()->login(m_oConfig.oWlan.sClientSsid, m_oConfig.oWlan.oClientPassword.getString());
    m_pPrivate->pWlanDevice->getClient()->start();
  }
}
