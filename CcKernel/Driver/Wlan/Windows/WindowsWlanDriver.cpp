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
 * @brief     Implementation of Class WindowsWlanDriver
 */

#include "WindowsWlanDriver.h"
#include "WindowsWlanClient.h"
#include "CcKernel.h"
#include "CcList.h"
#include <windows.h>
#include <wlanapi.h>
#include <winerror.h>

WindowsWlanDriver* WindowsWlanDriver::g_pInstance(nullptr);

class WindowsWlanDriver::CPrivate
{
public:
  IKernel* pKernel;
  HANDLE hWlan = NULL;
  CcList<WindowsWlanClient*> oClients;
};

WindowsWlanDriver::WindowsWlanDriver (IKernel* pKernel)
{
  g_pInstance = this;
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->pKernel = pKernel;
  DWORD dwVersion;
  if (ERROR_SUCCESS != WlanOpenHandle(2, NULL, &dwVersion, &m_pPrivate->hWlan))
  {
    m_pPrivate->hWlan = NULL;
  }
}

WindowsWlanDriver::~WindowsWlanDriver ()
{
  if (m_pPrivate->hWlan)
  {
    WlanCloseHandle(m_pPrivate->hWlan, NULL);
    m_pPrivate->hWlan = NULL;
  }
  CCDELETE(m_pPrivate);
}

CcStatus WindowsWlanDriver::entry()
{
  return true;
}

CcStatus WindowsWlanDriver::unload()
{
  for (WindowsWlanClient* pClient : m_pPrivate->oClients)
  {
    pClient->setState(IDevice::EState::Stopping);
    m_pPrivate->pKernel->removeDevice(CcDeviceHandle(pClient, EDeviceType::WlanClient));
    CCDELETE(pClient);
  }
  return true;
}

CcStatus WindowsWlanDriver::setupClient(size_t uiNr)
{
  CcStatus oStatus = false;
  if (m_pPrivate->hWlan)
  {
    CCNEWTYPE(pClient, WindowsWlanClient, uiNr);
    oStatus = pClient->init(m_pPrivate->hWlan);
    if (oStatus)
    {
      m_pPrivate->pKernel->addDevice(CcDeviceHandle(pClient, EDeviceType::WlanClient));
      m_pPrivate->oClients.append(pClient);
    }
    else
    {
      CCDELETE(pClient);
    }
  }
  return oStatus;
}
