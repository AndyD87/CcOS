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
 * @brief     Implementation of class WindowsWlanClient
 **/

#include "WindowsWlanClient.h"
#include "CcKernel.h"
#include "CcWString.h"
#include <windows.h>
#include <wlanapi.h>

class WindowsWlanClient::CPrivate
{
public:
  CPrivate()
    {}
  ~CPrivate()
    {}

public:
  HANDLE       hClientHandle;

  CcMacAddress oMacAddress;
};

WindowsWlanClient::WindowsWlanClient(size_t uiNr) :
  m_uiNr(uiNr)
{
  CCNEW(m_pPrivate, CPrivate);
}

WindowsWlanClient::~WindowsWlanClient()
{
  CCDELETE(m_pPrivate);
}

const CcMacAddress& WindowsWlanClient::getMacAddress()
{
  return m_pPrivate->oMacAddress;
}

bool WindowsWlanClient::isConnected()
{
  return false;
}

CcStatus WindowsWlanClient::login(const CcString& sSsid, const CcString& sPassord)
{
  CcStatus oStatus( false);
  CCUNUSED(sSsid);
  CCUNUSED(sPassord);
  return oStatus;
}

CcStatus WindowsWlanClient::init(void* hWlan)
{
  CcStatus oStatus(false);
  PWLAN_INTERFACE_INFO_LIST pWlanList;
  DWORD uiResult = WlanEnumInterfaces(static_cast<HANDLE>(hWlan), NULL, &pWlanList);
  if (uiResult == ERROR_SUCCESS)
  {

  }
  return oStatus;
}
