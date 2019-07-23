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

class CcRemoteDeviceServer::CPrivate
{
public:
};

CcRemoteDeviceServer::CcRemoteDeviceServer(CcRemoteDeviceConfigServer* pConfig) :
  CcHttpServer(nullptr),
  m_oDirectories(CcRemoteDeviceGlobals::ProjectName, true)
{
  CCNEW(m_pPrivate, CPrivate);
  if (pConfig == nullptr)
  {
    CCNEW(m_pConfig, CcRemoteDeviceConfigServer);
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
  if(m_pConfig != nullptr)
  {
    setConfig(&m_pConfig->oHttpConfig);
    m_pConfig->oHttpConfig.setSslEnabled(true);
    m_oDirectories.createAllPaths();
    if(m_pConfig->oHttpConfig.getSslCertificate().length() == 0)
    {
      CcString sPath = m_oDirectories.getDataDir();
      m_pConfig->oHttpConfig.setSslCertificate(sPath.appendPath(CcRemoteDeviceGlobals::Defaults::SslCertificateFilename));
    }
    if(m_pConfig->oHttpConfig.getSslKey().length() == 0)
    {
      CcString sPath = m_oDirectories.getDataDir();
      m_pConfig->oHttpConfig.setSslKey(sPath.appendPath(CcRemoteDeviceGlobals::Defaults::SslKeyFilename));
    }
    CcHttpServer::run();
  }
}
