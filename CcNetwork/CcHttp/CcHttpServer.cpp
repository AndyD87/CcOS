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
 * @brief     Implementation of Class CcHttpServer
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcHttpServer.h"
#include "CcHttpGlobals.h"
#include "Network/CcCommonPorts.h"
#include "CcFile.h"
#include "CcMemoryMonitor.h"
#include "CcGlobalStrings.h"
#ifdef CCSSL_ENABLED
#include "CcSsl.h"
#include "CcSslSocket.h"
#include "CcSslControl.h"
#endif

CcHttpServer::CcHttpServer( uint16 Port ) :
  CcApp("CcHttpServer"),
  m_bConfigOwner(true)
{
  CCNEW(m_pConfig, CcHttpServerConfig);
  getConfig().getAddressInfo().init(ESocketType::TCP);
  getConfig().getAddressInfo().setPort(Port);
}

CcHttpServer::CcHttpServer(CcHttpServerConfig* pConfig) :
  CcApp("CcHttpServer"),
  m_pConfig(pConfig),
  m_bConfigOwner(false)
{
}

CcHttpServer::~CcHttpServer()
{
  if (m_bConfigOwner)
  {
    CCDELETE(m_pConfig);
  }
}

void CcHttpServer::registerProvider(const CcHandle<IHttpProvider> &toAdd)
{
  m_ProviderList.append(toAdd);
}

void CcHttpServer::deregisterProvider(const CcHandle<IHttpProvider> &toRemove)
{
  m_ProviderList.removeItem(toRemove);
}

const CcHandle<IHttpProvider> CcHttpServer::findProvider(const CcHttpWorkData &oData) const
{
  CcHandle<IHttpProvider> pRet(nullptr);
  for (size_t i = 0; i < m_ProviderList.size(); i++)
  {
    if (m_ProviderList[i]->checkResponsible(oData))
    {
      pRet = m_ProviderList[i];
      break;
    }
  }
  if (pRet == nullptr)
    pRet = m_DefaultProvider;
  return pRet;
}

const CcList<CcHandle<IHttpProvider>>& CcHttpServer::getReceiverList()
{
  return m_ProviderList;
}

void CcHttpServer::run()
{
  CCDEBUG("HTTP-Server starting on Port: " + CcString::fromNumber(getConfig().getAddressInfo().getPort()));
  m_eState = EState::Starting;
  setExitCode(EStatus::Error);
  init();
#ifdef CCSSL_ENABLED
  if(m_pConfig->isSslEnabled())
  {
    CCNEWTYPE(pSocket, CcSslSocket);
    pSocket->initServer();
    if (CcFile::exists(m_pConfig->getSslKey()) == false ||
        CcFile::exists(m_pConfig->getSslKey()) == false)
    {
      CcSslControl::createCert(
        m_pConfig->getSslCertificate(),
        m_pConfig->getSslKey()
      );
    }
    m_oSocket = pSocket;
  }
  else
  {
    m_oSocket = CcSocket(ESocketType::TCP);
  }
#else
  m_oSocket = CcSocket(ESocketType::TCP);
#endif
  int iTrue = 1;
  m_oSocket.setOption(ESocketOption::Reuse, &iTrue, sizeof(iTrue));
  if (m_oSocket.bind(getConfig().getAddressInfo())
#ifdef CCSSL_ENABLED
    &&  
    (m_pConfig->isSslEnabled()==false ||
       ( static_cast<CcSslSocket*>(m_oSocket.getRawSocket())->loadKey(m_pConfig->getSslKey()) &&
         static_cast<CcSslSocket*>(m_oSocket.getRawSocket())->loadCertificate(m_pConfig->getSslCertificate())
       )
     )
#endif
  )
  {
    m_eState = EState::Listening;
    if(m_oSocket.listen())
    {
      ISocket *temp;
      CcMemoryMonitor::clear();
      while (getThreadState() == EThreadState::Running)
      {
        if(m_uiWorkerCount < 4)
        {
          temp = m_oSocket.accept();
          if(temp != nullptr)
          {
            m_uiWorkerCount++;
            CCNEWTYPE(worker, CcHttpServerWorker, *this, CcSocket(temp));
            worker->start();
          }
        }
        else
        {
          CcKernel::delayMs(5);
        }
      }
    }
     else
    {
      setExitCode(EStatus::NetworkPortInUse);
      CCDEBUG("Unable to listen to Http-Port: " + CcString::fromNumber(getConfig().getAddressInfo().getPort()));
    }
  }
                                                            else
  {
    setExitCode(EStatus::NetworkPortInUse);
    CCDEBUG("Unable to bind Http-Port: " + CcString::fromNumber(getConfig().getAddressInfo().getPort()));
  }
  // Check if nothing changed since init
  if (getExitCode())
  {
    setExitCode(EStatus::AllOk);
  }
  m_eState = EState::Stopped;
}

void CcHttpServer::onStop()
{
  if (m_oSocket.isValid())
  {
    m_oSocket.close();
  }
}

void CcHttpServer::init()
{
  CCNEW(m_DefaultProvider, CcHttpDefaultProvider);
  getConfig().getConfigFilePath() = CcKernel::getConfigDir() + CcHttpGlobals::ServerConfigDirName + "/" + CcHttpGlobals::ServerConfigFileName;
}
