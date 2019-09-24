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
 * @brief     Implementation of Class CcMqttClient
 */
#include "CcMqttClient.h"
#include "CcKernel.h"
#include "CcMqttGlobals.h"
#include "CcMutex.h"
#include "Network/CcSocket.h"
#ifdef CCSSL_ENABLED
#include "CcSslSocket.h"
#endif // CCSSL_ENABLED

class CcMqttClient::CPrivate
{
public:
  CcSocket oSocket;
};

CcMqttClient::CcMqttClient() :
  CcApp(CcMqttGlobals::Strings::ClientApplicationName)
{
  CCNEW(m_pPrivate, CPrivate);
}

CcMqttClient::~CcMqttClient()
{
  CCDELETE(m_pPrivate);
}

void CcMqttClient::run()
{
#ifdef CCSSL_ENABLED
  if (m_oConfig.isSsl())
  {
    m_pPrivate->oSocket = new CcSslSocket();
  }
  else
  {
#endif // CCSSL_ENABLED
    m_pPrivate->oSocket = CcKernel::getSocket(ESocketType::TCP);
#ifdef CCSSL_ENABLED
  }
#endif // CCSSL_ENABLED
  m_pPrivate->oSocket.setAddressInfo(getConfig().getAddressInfo());
  if (m_pPrivate->oSocket.connect())
  {

  }
}

bool CcMqttClient::connect()
{
  if (getConfig().getUsername().length() ||
      getConfig().getPassword().getString().length())
  {
    // Login with password
  }
  else
  {
    // Login without password
  }
  return false;
}
