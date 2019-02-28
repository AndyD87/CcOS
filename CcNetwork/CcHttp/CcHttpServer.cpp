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

CcHttpServer::CcHttpServer( uint16 Port )
{
  m_oAddressInfo.init(ESocketType::TCP);
  m_oAddressInfo.setPort(Port);
  //m_oAddressInfo.setIp("127.0.0.1");
  init();
}

CcHttpServer::CcHttpServer(const CcStringList &Arg)
{
  m_oAddressInfo.init(ESocketType::TCP);
  m_oAddressInfo.setPort(CcCommonPorts::HTTP);
  init();
  CCUNUSED(Arg);
}

CcHttpServer::~CcHttpServer()
{
}

void CcHttpServer::setWorkingDir(const CcString& Wd)
{
  m_WD = Wd;
}

CcString CcHttpServer::getWorkingDir()
{
  return m_WD;
}

void CcHttpServer::registerProvider(CcHandle<CcHttpProvider> &toAdd)
{
  m_ProviderList.append(toAdd);
}

void CcHttpServer::deregisterProvider(CcHandle<CcHttpProvider> &toRemove)
{
  m_ProviderList.removeItem(toRemove);
}

const CcHandle<CcHttpProvider> CcHttpServer::findProvider(const CcString& Path)
{
  CcHandle<CcHttpProvider> pRet(nullptr);
  for (size_t i = 0; i < m_ProviderList.size(); i++)
  {
    if (m_ProviderList[i]->pregMatch(Path))
      pRet = m_ProviderList[i];
  }
  if (pRet == nullptr)
    pRet = m_DefaultProvider;
  return pRet;
}

const CcList<CcHandle<CcHttpProvider>>& CcHttpServer::getReceiverList()
{
  return m_ProviderList;
}

void CcHttpServer::run()
{
  CCDEBUG("HTTP-Server starting on Port: " + CcString::fromNumber(m_oAddressInfo.getPort()));
  m_Socket = CcSocket(ESocketType::TCP);
  if (m_Socket.bind(m_oAddressInfo))
  {
    m_Socket.listen();
    ISocket *temp;
    while (getThreadState() == EThreadState::Running)
    {
      temp = m_Socket.accept();
      CcHttpServerWorker *worker = new CcHttpServerWorker(this, CcSocket(temp)); CCMONITORNEW(worker);
      worker->start();
    }
  }
  else
  {
    CCDEBUG("Unable to bind Http-Port: " + CcString::fromNumber(m_oAddressInfo.getPort()));
  }
}

void CcHttpServer::init()
{
  m_DefaultProvider = new CcHttpDefaultProvider();
  m_sConfigFilePath = CcKernel::getConfigDir() + CcHttpGlobals::ServerConfigDirName + "/" + CcHttpGlobals::ServerConfigFileName;
}

void CcHttpServer::parseConfigHeader()
{
  CcFile oConfigFile(m_sConfigFilePath);
  if (oConfigFile.exists())
  {

  }
}
