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
#include "CcHttpServer.h"
#include "CcHttpServerConfig.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcFile.h"
#include "CcSslControl.h"

class CcRemoteDeviceServer::CPrivate
{
public:
  CcHttpServer oHttpServer;
};

CcRemoteDeviceServer::CcRemoteDeviceServer(CcRemoteDeviceConfig* pConfig) :
  m_oDirectories(CcRemoteDeviceGlobals::ProjectName, true)
{
  CCNEW(m_pPrivate, CPrivate);
  if (pConfig == nullptr)
  {
    CCNEW(m_pConfig, CcRemoteDeviceConfig);
  }
  else
  {
    m_pConfig = pConfig;
    m_bConfigOwner = false;
  }
}

CcRemoteDeviceServer::~CcRemoteDeviceServer()
{
  CCDELETE(m_pPrivate);
}

void CcRemoteDeviceServer::run()
{
  m_pPrivate->oHttpServer.getConfig().setSslEnabled(true);
  if(m_pConfig &&
     m_pConfig->getServerConfig())
  {
    m_oDirectories.createAllPaths();
    m_pPrivate->oHttpServer.getConfig().getAddressInfo().setPort(m_pConfig->getServerConfig()->oRemoteControl.uiPort);
    if(m_pConfig->getServerConfig()->oRemoteControl.sSslCertificatePath.length() == 0)
    {
      m_pConfig->getServerConfig()->oRemoteControl.sSslCertificatePath = m_oDirectories.getDataDir();
      m_pConfig->getServerConfig()->oRemoteControl.sSslCertificatePath.appendPath(CcRemoteDeviceGlobals::Defaults::SslCertificateFilename);
    }
    if(m_pConfig->getServerConfig()->oRemoteControl.sSslKeyPath.length() == 0)
    {
      m_pConfig->getServerConfig()->oRemoteControl.sSslKeyPath = m_oDirectories.getDataDir();
      m_pConfig->getServerConfig()->oRemoteControl.sSslKeyPath.appendPath(CcRemoteDeviceGlobals::Defaults::SslKeyFilename);
    }

    if(CcFile::exists(m_pConfig->getServerConfig()->oRemoteControl.sSslCertificatePath) == false ||
       CcFile::exists(m_pConfig->getServerConfig()->oRemoteControl.sSslKeyPath) == false)
    {
      CcSslControl::createCert(m_pConfig->getServerConfig()->oRemoteControl.sSslCertificatePath,
                               m_pConfig->getServerConfig()->oRemoteControl.sSslKeyPath);
    }
    m_pPrivate->oHttpServer.getConfig().setSslCertificate(m_pConfig->getServerConfig()->oRemoteControl.sSslCertificatePath);
    m_pPrivate->oHttpServer.getConfig().setSslKey(m_pConfig->getServerConfig()->oRemoteControl.sSslKeyPath);
    setExitCode(m_pPrivate->oHttpServer.exec());
  }
}
