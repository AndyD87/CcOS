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

CcApp* CcHttpServer::main(const CcStringList &Arg)
{
  CcApp* ret = new CcHttpServer(Arg); CCMONITORNEW(ret);
  return ret;
}

CcHttpServer::CcHttpServer( uint16 Port ) :
  CcApp("CcHttpServer")
{
  getConfig().getAddressInfo().init(ESocketType::TCP);
  getConfig().getAddressInfo().setPort(Port);
}

CcHttpServer::CcHttpServer(const CcHttpServerConfig& oConfig) :
  CcApp("CcHttpServer"),
  m_oConfig(oConfig)
{
}

CcHttpServer::CcHttpServer(const CcStringList &Arg) :
  CcApp("CcHttpServer")
{
  getConfig().getAddressInfo().init(ESocketType::TCP);
  getConfig().getAddressInfo().setPort(CcCommonPorts::HTTP);
  CCUNUSED(Arg);
}

CcHttpServer::~CcHttpServer()
{
}

void CcHttpServer::registerProvider(CcHandle<IHttpProvider> &toAdd)
{
  m_ProviderList.append(toAdd);
}

void CcHttpServer::deregisterProvider(CcHandle<IHttpProvider> &toRemove)
{
  m_ProviderList.removeItem(toRemove);
}

const CcHandle<IHttpProvider> CcHttpServer::findProvider(const CcString& Path) const
{
  CcHandle<IHttpProvider> pRet(nullptr);
  for (size_t i = 0; i < m_ProviderList.size(); i++)
  {
    if (m_ProviderList[i]->pregMatch(Path))
      pRet = m_ProviderList[i];
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
  m_oSocket = CcSocket(ESocketType::TCP);
  m_oSocket.setOption(ESocketOption::ReusePort);
  if (m_oSocket.bind(getConfig().getAddressInfo()))
  {
    m_eState = EState::Listening;
    if(m_oSocket.listen())
    {
      ISocket *temp;
      while (getThreadState() == EThreadState::Running)
      {
        if(m_uiWorkerCount < 4)
        {
          temp = m_oSocket.accept();
          if(temp != nullptr)
          {
            m_uiWorkerCount++;
            CcHttpServerWorker *worker = new CcHttpServerWorker(*this, CcSocket(temp)); CCMONITORNEW(worker);
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
  if (getExitCode() == EStatus::Error)
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
  m_DefaultProvider = new CcHttpDefaultProvider();
  getConfig().getConfigFilePath() = CcKernel::getConfigDir() + CcHttpGlobals::ServerConfigDirName + "/" + CcHttpGlobals::ServerConfigFileName;
}
