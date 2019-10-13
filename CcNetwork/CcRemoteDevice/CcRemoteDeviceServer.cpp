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
#include "RestApi/CcRestApiDevice.h"
#include "RestApi/CcRestApiSystem.h"
#include "Applications/RestApiWebframework/CcHttpWebframework.h"
#include "Applications/RestApiWebframework/CcHttpWebframeworkIndex.h"
#include "CcFile.h"
#include "Devices/IWlan.h"
#include "Devices/IWlanAccessPoint.h"
#include "Devices/IWlanClient.h"

using namespace CcHttp::Application::RestApiWebframework;

class CcRemoteDeviceServer::CPrivate
{
public:
  class CNetworkIp : public CcRestApiApplicationStatus::IPublisher
  {
  public:
    CNetworkIp(const CcString& sName, INetwork* pNetwork) :
      sName(sName),
      pNetwork(pNetwork)
    {}

    virtual CcStringMap getStatus() override
    {
      CcStringMap oMap;
      for(const CcIpInterface& rInterface : pNetwork->getInterfaceList())
      {
        oMap.append(sName, rInterface.oIpAddress.getString());
      }
      return oMap;
    }

    CcString  sName;
    INetwork* pNetwork;
  };

  CcHttpWebframework*         pHttpServer = nullptr;
  CcRemoteDeviceJsProvider*   pJsProvider = nullptr;
  CcRemoteDeviceCssProvider*  pCssProvider = nullptr;
  CcVector<CcRestApiDevice*>  oAllocatedRestApiDevices;
  CcVector<CcRestApiApplicationStatus::IPublisher*> oStatusPublisher;

  CcHandle<IWlan>             pWlanDevice = nullptr;
};

CcRemoteDeviceServer::CcRemoteDeviceServer(CcRemoteDeviceServerConfig* pConfig, bool bNoUi) :
  CcApp(CcRemoteDeviceGlobals::ProjectName),
  m_pConfig(pConfig),
  m_oDirectories(CcRemoteDeviceGlobals::ProjectName, true)
{
  CCNEW(m_pPrivate, CPrivate);
  if(m_pConfig == nullptr)
  {
    m_bConfigOwner = true;
    CCNEW(m_pConfig, CcRemoteDeviceServerConfig);
  }
  m_pPrivate->pHttpServer = new CcHttp::Application::RestApiWebframework::CcHttpWebframework(bNoUi);
  if(bNoUi == false &&
     m_pPrivate->pHttpServer != nullptr)
  {
    CCNEW(m_pPrivate->pJsProvider, CcRemoteDeviceJsProvider);
    m_pPrivate->pHttpServer->registerProvider(m_pPrivate->pJsProvider);
    CCNEW(m_pPrivate->pCssProvider, CcRemoteDeviceCssProvider);
    m_pPrivate->pHttpServer->registerProvider(m_pPrivate->pCssProvider);
    m_pPrivate->pHttpServer->getRestApiApplication().getMenu().append("Home", "/api/app/status");
    if(m_oBoardSupport.hasGpio())
    {
      for (size_t uiIndex = 0; uiIndex < m_oBoardSupport.getGpioPins().uiPinCount; uiIndex++)
      {
        CCNEWTYPE(  pDevice, CcRestApiDevice,
                    &m_pPrivate->pHttpServer->getRestApiSystem().getDevices(),
                    m_oBoardSupport.getGpioPins().pPins[uiIndex].uiPort,
                    m_oBoardSupport.getGpioPins().pPins[uiIndex].uiPin);
        if(pDevice->getDevice().isValid())
        {
          pDevice->setName(m_oBoardSupport.getGpioPins().pPins[uiIndex].pcName);
          pDevice->getDevice().cast<IGpioPin>()->setDirection(m_oBoardSupport.getGpioPins().pPins[uiIndex].eDirection);
          m_pPrivate->oAllocatedRestApiDevices.append(pDevice);
        }
        else
        {
          CCDELETE(pDevice);
        }
      }
      m_pPrivate->pHttpServer->getRestApiApplication().getMenu().append("Gpio", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::GpioPin));
    }
    if(m_oBoardSupport.hasLan())
      m_pPrivate->pHttpServer->getRestApiApplication().getMenu().append("Network", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::Network));
    if(m_oBoardSupport.hasWlanClient())
      m_pPrivate->pHttpServer->getRestApiApplication().getMenu().append("WlanClient", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::WlanClient));
    if(m_oBoardSupport.hasWlanAccessPoint())
      m_pPrivate->pHttpServer->getRestApiApplication().getMenu().append("WlanAccessPoint", "/api/system/devices/" + CcDeviceHandle::getTypeString(EDeviceType::WlanAccessPoint));

    if(m_pPrivate->pHttpServer->getIndex())
    {
      m_pPrivate->pHttpServer->getIndex()->addLoadableScript(m_pPrivate->pJsProvider->getPath());
      m_pPrivate->pHttpServer->getIndex()->addStylesheet(m_pPrivate->pCssProvider->getPath());
    }
  }
}

CcRemoteDeviceServer::~CcRemoteDeviceServer()
{
  while(m_pPrivate->oAllocatedRestApiDevices.size())
  {
    CCDELETE(m_pPrivate->oAllocatedRestApiDevices[0]);
    m_pPrivate->oAllocatedRestApiDevices.remove(0);
  }
  if(m_bConfigOwner)
  {
    CCDELETE(m_pConfig);
  }
  CCDELETE(m_pPrivate->pJsProvider);
  CCDELETE(m_pPrivate->pCssProvider);
  CCDELETE(m_pPrivate);
}

void CcRemoteDeviceServer::run()
{
  setupWlan();
  setupWebserver();
  m_pPrivate->pHttpServer->start();
  while(isRunning())
  {
    CcKernel::sleep(1000);
  }
}

void CcRemoteDeviceServer::onStop()
{
  if(m_pPrivate->pHttpServer &&
     m_pPrivate->pHttpServer->isInProgress())
  {
    m_pPrivate->pHttpServer->stop();
    m_pPrivate->pHttpServer->waitForExit();
  }
}

void CcRemoteDeviceServer::setupWebserver()
{
  m_pPrivate->pHttpServer->getConfig().getAddressInfo().setPort(CcRemoteDeviceGlobals::Defaults::HttpPort);
  m_pPrivate->pHttpServer->getConfig().setSslEnabled(true);
  m_oDirectories.createAllPaths();
  if(m_pPrivate->pHttpServer->getConfig().getSslCertificate().length() == 0)
  {
    CcString sPath = m_oDirectories.getDataDir();
    m_pPrivate->pHttpServer->getConfig().setSslCertificate(sPath.appendPath(CcRemoteDeviceGlobals::Defaults::SslCertificateFilename));
  }
  if(m_pPrivate->pHttpServer->getConfig().getSslKey().length() == 0)
  {
    CcString sPath = m_oDirectories.getDataDir();
    m_pPrivate->pHttpServer->getConfig().setSslKey(sPath.appendPath(CcRemoteDeviceGlobals::Defaults::SslKeyFilename));
  }
}

void CcRemoteDeviceServer::setupWlan()
{
  m_pPrivate->pWlanDevice = CcKernel::getDevice(EDeviceType::Wlan).cast<IWlan>();
  if(m_pPrivate->pWlanDevice.isValid())
  {
    if( m_oBoardSupport.hasWlanAccessPoint()  &&
        m_pConfig->oSystem.oWlanAccessPoint.bEnable       &&
        m_pPrivate->pWlanDevice->getAccessPoint())
    {
      CCNEWTYPE(pPublisher,
                CPrivate::CNetworkIp,
                CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPoint,
                m_pPrivate->pWlanDevice->getAccessPoint());
      m_pPrivate->pHttpServer->getRestApiApplication().getStatus().appendPublisher(pPublisher);
      m_pPrivate->oStatusPublisher.append(pPublisher);
      m_pPrivate->pWlanDevice->getAccessPoint()->setCredentials(m_pConfig->oSystem.oWlanAccessPoint.sSsid,
                                                                m_pConfig->oSystem.oWlanAccessPoint.oPassword.getString());
      m_pPrivate->pWlanDevice->getAccessPoint()->start();
    }
    if(m_oBoardSupport.hasWlanClient()          &&
       m_pConfig->oSystem.oWlanClient.bEnable   &&
       m_pPrivate->pWlanDevice->getClient())
    {
      CCNEWTYPE(pPublisher,
                CPrivate::CNetworkIp,
                CcRemoteDeviceGlobals::Config::SystemNs::WlanClient,
                m_pPrivate->pWlanDevice->getClient());
      m_pPrivate->pHttpServer->getRestApiApplication().getStatus().appendPublisher(pPublisher);
      m_pPrivate->oStatusPublisher.append(pPublisher);
      if(m_pConfig->oSystem.oWlanClient.oKnownAccessPoints.size() > 0)
      {
        m_pPrivate->pWlanDevice->getClient()->login(m_pConfig->oSystem.oWlanClient.oKnownAccessPoints[0].sSsid,
                                                    m_pConfig->oSystem.oWlanClient.oKnownAccessPoints[0].oPassword.getString());
        m_pPrivate->pWlanDevice->getClient()->start();
      }
    }
  }
}
