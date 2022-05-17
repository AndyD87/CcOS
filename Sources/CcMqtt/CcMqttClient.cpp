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

CcMqttClient::CcMqttClient() :
  CcApp(CcMqttGlobals::Strings::ClientApplicationName)
{
}

CcMqttClient::~CcMqttClient()
{
  onStop();
}

void CcMqttClient::run() 
{
  CCDEBUG("in  CcMqttClient::run");
  m_oSocketLock.lock();
  #ifdef CCSSL_ENABLED
    if (m_oConfig.isSsl())
    {
      if(isRunning())
        m_oSocket = CCNEW_INLINE(CcSslSocket);
    }
    else
    {
        m_oSocket = CcKernel::getSocket(ESocketType::TCP);
    }
  #else
    m_oSocket = CcKernel::getSocket(ESocketType::TCP);
  #endif // CCSSL_ENABLED
  m_oSocket.setPeerInfo(getConfig().getAddressInfo());
  m_oSocketLock.unlock();
  while(isRunning())
  {
    if (m_oSocket.connect())
    {

    }
    else
    {
      //CcKernel::sleep(100);
    }
  }
  CCDEBUG("out CcMqttClient::run");
}

void CcMqttClient::onStop()
{
  m_oSocketLock.lock();
  m_oSocket.close();
  m_oSocketLock.unlock();
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
