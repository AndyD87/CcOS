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
 * @brief     Implementation of Class CcHttpServer
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcHttpServer.h"
#include "CcHttpGlobals.h"
#include "Network/CcCommonPorts.h"
#include "CcFile.h"
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
  CCNEW(m_pConfig, CcHttpServerConfig, Port);
  if(m_pConfig)
  {
    m_pConfig->getAddressInfo().init(ESocketType::TCP);
    if(Port != CcCommonPorts::InvalidPort)
    {
      if(CcKernel::isAdmin())
        m_pConfig->getAddressInfo().setPort(Port);
      else
        m_pConfig->getAddressInfo().setPort(Port + CcCommonPorts::CcOSOffset);
      if (Port == CcCommonPorts::HTTPS)
        m_pConfig->setSslEnabled(true);
    }
    else
    {
      m_pConfig->getAddressInfo().setPort(CcCommonPorts::HTTP);
    }
  }
}

CcHttpServer::CcHttpServer(CcHttpServerConfig* pConfig) :
  CcApp("CcHttpServer"),
  m_pConfig(pConfig),
  m_bConfigOwner(false)
{
}

CcHttpServer::~CcHttpServer()
{
  stop();
  size_t uiTimeout = 5;

  // Wait for worker to be done and onStop is finished.
  while((m_uiWorkerCount || m_eState != EState::Stopped) && uiTimeout-- && CcKernel::sleep(1000))
    ;

  if(uiTimeout > 5 || 0)
    CCDEBUG("~CcHttpServer Timed out");
  if (m_bConfigOwner)
  {
    CCDELETE(m_pConfig);
  }
  CCDELETE(m_pDefaultProvider);
}

void CcHttpServer::registerProvider(const CcHandle<IHttpProvider> &toAdd)
{
  m_ProviderList.append(toAdd);
}

bool CcHttpServer::deregisterProvider(const CcHandle<IHttpProvider> &toRemove)
{
  return m_ProviderList.removeItem(toRemove);
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
    pRet = m_pDefaultProvider;
  return pRet;
}

const CcList<CcHandle<IHttpProvider>>& CcHttpServer::getReceiverList()
{
  return m_ProviderList;
}

void CcHttpServer::run()
{
  m_eState = EState::Starting;
  setExitCode(EStatus::Error);
  init();
  if( m_pConfig != nullptr &&
      isRunning())
  {
    do
    {
      CCDEBUG("HTTP-Server starting on Port: " + CcString::fromNumber(getConfig().getAddressInfo().getPort()));
      #ifdef CCSSL_ENABLED
        if( m_pConfig->isSslEnabled() && isRunning())
        {
          CCNEWTYPE(pSocket, CcSslSocket);
          pSocket->initServer();
          if (CcFile::exists(m_pConfig->getSslKey()) == false ||
              CcFile::exists(m_pConfig->getSslCertificate()) == false)
          {
            CcSslControl::createCert(
              m_pConfig->getSslCertificate(),
              m_pConfig->getSslKey()
            );
          }
          m_oSocket = pSocket;
        }
        else if(isRunning())
        {
          m_oSocket = CcKernel::getSocket(ESocketType::TCP);
        }
      #else
        m_oSocket = CcSocket(ESocketType::TCP);
      #endif
      CCVERBOSE("HTTP-Server start binding");
      #ifdef CCSSL_ENABLED
        m_oSocket.lock();
        bool bSsl = (m_pConfig->isSslEnabled() == false ||
                      (static_cast<CcSslSocket*>(m_oSocket.getRawSocket())->loadKey(m_pConfig->getSslKey()) &&
                        static_cast<CcSslSocket*>(m_oSocket.getRawSocket())->loadCertificate(m_pConfig->getSslCertificate())
                        )
                    );
        m_oSocket.unlock();
      #else
        bool bSsl = true;
      #endif
      if (isRunning() && bSsl &&
          m_oSocket.bind(getConfig().getAddressInfo()) )
      {
        #ifndef GENERIC
          int32 iTrue = 1;
          if(!m_oSocket.setOption(ESocketOption::Reuse, &iTrue, sizeof(iTrue)))
          {
            CCERROR("Failed to set reuse option");
          }
          if(!m_oSocket.setTimeout(m_pConfig->getComTimeout()))
          {
            CCERROR("Failed to set timeout option");
          }
        #endif
        m_eState = EState::Listening;
        if(m_oSocket.listen())
        {
          ISocket *temp = nullptr;
          while (getThreadState() == EThreadState::Running)
          {
            if(m_uiWorkerCount < m_pConfig->getMaxWorkerCount())
            {
              temp = m_oSocket.accept();
              if(temp != nullptr)
              {
                m_uiRequestsCount++;
                CCNEWTYPE(worker, CcHttpServerWorker, *this, CcSocket(temp));
                if(worker->start())
                {
                  incWorker();
                }
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
    } while (getThreadState() == EThreadState::Running &&
             CcKernel::sleep(2000)); // Something was wrong, restart server
  }
  else
  {
    setExitCode(EStatus::ConfigError);
    CCERROR("No config available");
  }
  m_eState = EState::Stopped;
}

void CcHttpServer::onStop()
{
  m_oSocket.close();
}

void CcHttpServer::init()
{
  if(m_pDefaultProvider == nullptr)
  {
    CCNEW(m_pDefaultProvider, CcHttpDefaultProvider);
  }
}

void CcHttpServer::setConfig(CcHttpServerConfig* pConfig)
{
  if (m_bConfigOwner)
    CCDELETE(m_pConfig);

  m_bConfigOwner = false;
  m_pConfig = pConfig;
}
