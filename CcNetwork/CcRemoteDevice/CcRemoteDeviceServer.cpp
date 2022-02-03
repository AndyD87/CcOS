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
#include "RestApi/CcRestApiDeviceGpioPin.h"
#include "RestApi/CcRestApiDeviceWlan.h"
#include "RestApi/CcRestApiSystem.h"
#include "Applications/RestApiWebframework/CcHttpWebframework.h"
#include "Applications/RestApiWebframework/CcHttpWebframeworkIndex.h"
#include "CcFile.h"
#include "Devices/IWlan.h"
#include "Devices/IWlanAccessPoint.h"
#include "Devices/IWlanClient.h"
#include "CcConsole.h"
#include "NDocumentsGlobals.h"
#include "Server/CWorker.h"

using namespace NHttp::Application::RestApiWebframework;
using namespace NRemoteDevice::Server;

namespace NRemoteDevice
{
namespace NServer
{

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

}
}

CcRemoteDeviceServer::CcRemoteDeviceServer() :
  CcApp(CcRemoteDeviceGlobals::ProjectName),
  m_oDirectories(CcRemoteDeviceGlobals::ProjectName, true)
{
  m_bConfigOwner = true;
  CCNEW(m_pConfig, CConfig);
  m_pConfig->oUsers.sAdmin = CcRemoteDeviceGlobals::Defaults::DefaultUserName;
  m_pConfig->oUsers.oUsers.append(NRemoteDevice::Server::Config::CUsers::CCredentials(CcRemoteDeviceGlobals::Defaults::DefaultUserName, CcRemoteDeviceGlobals::Defaults::DefaultUserPassword));
  init();
}

CcRemoteDeviceServer::CcRemoteDeviceServer(CConfig* pConfig, bool bNoUi) :
  CcApp(CcRemoteDeviceGlobals::ProjectName),
  m_pConfig(pConfig),
  m_bUi(!bNoUi),
  m_oDirectories(CcRemoteDeviceGlobals::ProjectName, true)
{
}

void CcRemoteDeviceServer::init()
{
  m_oDirectories.createAllPaths();
  if (m_pConfig != nullptr &&
      !m_pConfig->isRead())
  {
    CcString sConfigfile = m_oDirectories.getConfigDir();
    sConfigfile.appendPath(CcRemoteDeviceGlobals::Names::ConfigName);
    if (CcFile::exists(sConfigfile))
    {
      m_pConfig->read(sConfigfile);
    }
    else
    {
      initConfigDefaults();
      m_pConfig->write(CConfig::ESource::FileJson, sConfigfile);
    }
  }
}

CcRemoteDeviceServer::~CcRemoteDeviceServer()
{
  stop();
  m_oSocket.close();
  for (CcRestApiDevice* pRestApiDevice : m_oAllocatedRestApiDevices)
  {
    CCDELETE(pRestApiDevice);
  }
  for (CcRestApiApplicationStatus::IPublisher* pPublisher : m_oStatusPublisher)
  {
    CCDELETE(pPublisher);
  }
  CCDELETE(m_pHttpServer);
  CCDELETE(m_pJsProvider);
  CCDELETE(m_pCssProvider);
  if(m_bConfigOwner)
  {
    CCDELETE(m_pConfig);
  }
}

void CcRemoteDeviceServer::run()
{
  m_bRunActive = true;
  setupWlan();
  setupWebserver();
  m_pHttpServer->start();
  do
  {
    if( isRunning() &&
        m_pConfig->bDetectable)
    {
      m_oSocket = CcSocket(ESocketType::UDP);
      if (!m_oSocket.open())
      {
        CCDEBUG("CcRemoteDeviceServer::run open failed");
      }
      else
      {
        if( !m_oSocket.setOption(ESocketOption::Reuse))
        {
          CCDEBUG("CcRemoteDeviceServer::run reuse failed");
        }
        else if( !m_oSocket.setOption(ESocketOption::Broadcast))
        {
          CCDEBUG("CcRemoteDeviceServer::run broadcast failed");
        }
        else if (m_oSocket.bind(m_pConfig->oAddressInfo))
        {
          while (isRunning())
          {
            CCNEWTYPE(pWorker, CWorker, this);
            size_t uiReadSize = m_oSocket.readArray(pWorker->getData());
            if (uiReadSize != SIZE_MAX &&
              uiReadSize > 0)
            {
              pWorker->getPeerInfo() = m_oSocket.getPeerInfo();
              pWorker->start();
            }
            else
            {
              CCDELETE(pWorker);
            }
          }
        }
        else
        {
          CCDEBUG("CcRemoteDeviceServer::run Bind failed");
        }
        m_oSocket.close();
      }
    }
  } while (isRunning() && CcKernel::sleep(1000));
  CCDEBUG("CcRemoteDeviceServer stopped, code " + CcString::fromNumber(getExitCode().getErrorUint()));
  m_bRunActive = false;
}

void CcRemoteDeviceServer::onStop()
{
  m_oSocket.close();
  if(m_pHttpServer &&
     m_pHttpServer->isInProgress())
  {
    m_pHttpServer->stop();
    m_pHttpServer->waitForExit();
  }
  int32 uiCountDown = 100;
  while (m_bRunActive && uiCountDown-- > 0)
  {
    CcKernel::delayMs(1000);
  }
  CCDEBUG("CcRemoteDeviceServer stop signal received");
}

size_t CcRemoteDeviceServer::getStackSize()
{
  return 16 * 1024;
}

void CcRemoteDeviceServer::setupWebserver()
{
  CCNEW(m_pHttpServer, NHttp::Application::RestApiWebframework::CcHttpWebframework, &getConfig().oInterfaces.oHttpServer, !m_bUi);
  if (m_bUi &&
      m_pHttpServer != nullptr)
  {
    CCNEW(m_pJsProvider, CcRemoteDeviceJsProvider);
    m_pHttpServer->registerProvider(m_pJsProvider);
    CCNEW(m_pCssProvider, CcRemoteDeviceCssProvider);
    m_pHttpServer->registerProvider(m_pCssProvider);
    m_pHttpServer->getRestApiApplication().getMenu().append("Home", "/api/app/status?test");
    if (m_oBoardSupport.hasGpio())
    {
      for (size_t uiIndex = 0; uiIndex < m_oBoardSupport.getGpioPins().size(); uiIndex++)
      {
        CCNEWTYPE(pDevice, CcRestApiDeviceGpioPin,
          &m_pHttpServer->getRestApiSystem().getDevices(),
          m_oBoardSupport.getGpioPins()[uiIndex].getPort(),
          m_oBoardSupport.getGpioPins()[uiIndex].getPin());
        if (pDevice->getDevice().isValid())
        {
          pDevice->setName(m_oBoardSupport.getGpioPins()[uiIndex].getName());
          pDevice->getDevice().cast<IGpioPin>()->setDirection(m_oBoardSupport.getGpioPins()[uiIndex].getDirection());
          m_oAllocatedRestApiDevices.append(pDevice);
        }
        else
        {
          CCDELETE(pDevice);
        }
      }
      m_pHttpServer->getRestApiApplication().getMenu().append("Gpio", "/api/system/devices/" + CcDevice::getTypeString(EDeviceType::GpioPin));
    }
    if (m_oBoardSupport.hasLan())
      m_pHttpServer->getRestApiApplication().getMenu().append("Network", "/api/system/devices/" + CcDevice::getTypeString(EDeviceType::Network));
    if (m_oBoardSupport.hasWlanClient())
    {
      m_pHttpServer->getRestApiApplication().getMenu().append("WlanClient", "/api/system/devices/" + CcDevice::getTypeString(EDeviceType::WlanClient));
    }
    if (m_oBoardSupport.hasWlanAccessPoint())
    {
      m_pHttpServer->getRestApiApplication().getMenu().append("WlanAccessPoint", "/api/system/devices/" + CcDevice::getTypeString(EDeviceType::WlanAccessPoint));
    }
    if (m_pHttpServer->getIndex())
    {
      m_pHttpServer->getIndex()->addLoadableScript(m_pJsProvider->getPath());
      m_pHttpServer->getIndex()->addStylesheet(m_pCssProvider->getPath());
    }
  }
}


void CcRemoteDeviceServer::initConfigDefaults()
{
  if (m_pConfig)
  {
    // Setup paths
    CcString sWebCert = m_oDirectories.getConfigDir();
    sWebCert.appendPath(m_pConfig->oInterfaces.oHttpServer.getSslCertificate());
    CcString sWebKey = m_oDirectories.getConfigDir();
    sWebKey.appendPath(m_pConfig->oInterfaces.oHttpServer.getSslKey());

    // init config defaults
    m_pConfig->oInterfaces.oHttpServer.getAddressInfo().setPort(CcCommonPorts::CcRemoteDevice);
    m_pConfig->oInterfaces.oHttpServer.setSslCertificate(sWebCert);
    m_pConfig->oInterfaces.oHttpServer.setSslKey(sWebKey);
  }
}

void CcRemoteDeviceServer::setupWlan()
{
  m_pWlanDevice = CcKernel::getDevice(EDeviceType::Wlan).cast<IWlan>();
  if(m_pWlanDevice.isValid())
  {
    if( m_oBoardSupport.hasWlanAccessPoint()  &&
        m_pConfig->oSystem.oWlanAccessPoint.bEnable       &&
        m_pWlanDevice->getAccessPoint())
    {
      CCNEWTYPE(pPublisher,
                NRemoteDevice::NServer::CNetworkIp,
                NDocumentsGlobals::NConfig::WlanAccessPoint,
                m_pWlanDevice->getAccessPoint());
      m_pHttpServer->getRestApiApplication().getStatus().appendPublisher(pPublisher);
      m_oStatusPublisher.append(pPublisher);
      m_pWlanDevice->getAccessPoint()->setCredentials(m_pConfig->oSystem.oWlanAccessPoint.sSsid,
                                                                m_pConfig->oSystem.oWlanAccessPoint.oPassword.getString());
      m_pWlanDevice->getAccessPoint()->start();

      CCNEWTYPE(  pDevice, CcRestApiDeviceWlan,
                  &m_pHttpServer->getRestApiSystem().getDevices(),
                  CcDevice( m_pWlanDevice->getAccessPoint(),
                                  EDeviceType::WlanAccessPoint
                  )
      );
      if(pDevice->getDevice().isValid())
      {
        m_oAllocatedRestApiDevices.append(pDevice);
      }
      else
      {
        CCDELETE(pDevice);
      }
    }
    if(m_oBoardSupport.hasWlanClient()          &&
       m_pConfig->oSystem.oWlanClient.bEnable   &&
       m_pWlanDevice->getClient())
    {
      CCNEWTYPE(pPublisher,
                NRemoteDevice::NServer::CNetworkIp,
                NDocumentsGlobals::NConfig::WlanClient,
                m_pWlanDevice->getClient());
      m_pHttpServer->getRestApiApplication().getStatus().appendPublisher(pPublisher);
      m_oStatusPublisher.append(pPublisher);
      if(m_pConfig->oSystem.oWlanClient.oKnownAccessPoints.size() > 0)
      {
        m_pWlanDevice->getClient()->login(m_pConfig->oSystem.oWlanClient.oKnownAccessPoints[0].sSsid,
                                                    m_pConfig->oSystem.oWlanClient.oKnownAccessPoints[0].oPassword.getString());
        m_pWlanDevice->getClient()->start();
      }

      CCNEWTYPE(  pDevice, CcRestApiDeviceWlan,
                  &m_pHttpServer->getRestApiSystem().getDevices(),
                  CcDevice( m_pWlanDevice->getClient(),
                                  EDeviceType::WlanClient
                  )
      );
      if(pDevice->getDevice().isValid())
      {
        m_oAllocatedRestApiDevices.append(pDevice);
      }
      else
      {
        CCDELETE(pDevice);
      }
    }
  }
}
