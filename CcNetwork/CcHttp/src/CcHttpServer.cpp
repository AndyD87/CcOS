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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcHttpServer
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcHttpServer.h"
#include "CcHttpGlobals.h"
#include "CcFile.h"

CcApp* CcHttpServer::main(const CcStringList &Arg)
{
  CcApp* ret = new CcHttpServer(Arg); CCMONITORNEW(ret);
  return ret;
}

CcHttpServer::CcHttpServer( uint16 Port ) :
  m_Port(Port)
{
  init();
}

CcHttpServer::CcHttpServer(const CcStringList &Arg) :
  m_Port(27580)
{
  init();
  CCUNUSED(Arg);
}

CcHttpServer::~CcHttpServer( void )
{
  if (m_Socket != nullptr)
  {
    m_Socket->close();
    CCMONITORDELETE(m_Socket);
    delete m_Socket;
    m_Socket = nullptr;
  }
}

void CcHttpServer::setWorkingDir(const CcString& Wd)
{
  m_WD = Wd;
}

CcString CcHttpServer::getWorkingDir(void)
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

const CcList<CcHandle<CcHttpProvider>>& CcHttpServer::getReceiverList(void)
{
  return m_ProviderList;
}

void CcHttpServer::run(void)
{
  CCDEBUG("HTTP-Server starting on Port: " + CcString::fromNumber(m_Port));
  m_Socket = CcKernel::getSocket(ESocketType::TCP);
  if (m_Socket->bind( m_Port))
  {
    m_Socket->listen();
    CcSocket *temp;
    while (getThreadState() == EThreadState::Running)
    {
      temp = m_Socket->accept();
      CcHttpServerWorker *worker = new CcHttpServerWorker(this, temp); CCMONITORNEW(worker);
      worker->start();
    }
  }
  else
  {
    CCDEBUG("Unable to bind Http-Port: " + CcString::fromNumber(m_Port));
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
