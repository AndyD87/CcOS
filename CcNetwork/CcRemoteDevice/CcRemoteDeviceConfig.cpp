/*
 * This file is part of CcRemoteDeviceConfig.
 *
 * CcRemoteDeviceConfig is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceConfig is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceConfig.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceConfig
 */
#include "CcRemoteDeviceConfig.h"

CcRemoteDeviceConfig::CcRemoteDeviceConfig()
{
}

CcRemoteDeviceConfig::~CcRemoteDeviceConfig()
{
  CCDELETE(m_pServer);
  CCDELETE(m_pClient);
}

CcRemoteDeviceConfig::CServer* CcRemoteDeviceConfig::getServerConfig()
{
  if(m_pServer == nullptr)
  {
    CCNEW(m_pServer, CServer);
  }
  return m_pServer;
}

CcRemoteDeviceConfig::CClient* CcRemoteDeviceConfig::getClienConfig()
{
  if(m_pClient == nullptr)
  {
    CCNEW(m_pClient, CClient);
  }
  return m_pClient;
}
