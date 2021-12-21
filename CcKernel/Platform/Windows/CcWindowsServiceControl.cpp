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
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcWindowsServiceControl
 **/
#include <windows.h>
#include <stdio.h>
#include "CcWindowsServiceControl.h"
#include "CcVector.h"
#include "CcArguments.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"

class CcWindowsServiceControl::CPrivate
{
public:
  SC_HANDLE hManager = nullptr;
  SC_HANDLE hService = nullptr;
  CcVector<wchar_t> oDependencies;
  CcWString sUsername;
  CcWString sPassword;
  CcString sApplication;
  CcArguments oArguments;
  DWORD uiAccessRights = SERVICE_ALL_ACCESS;
};

CcWindowsServiceControl::CcWindowsServiceControl(const CcWString& sName, bool bOpenReadOnly) :
  m_sName(sName)
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->oDependencies.append(0);
  m_pPrivate->oDependencies.append(0);
  wchar_t szPath[MAX_PATH];
  DWORD dwResult = GetModuleFileNameW(nullptr, szPath, sizeof(szPath));
  if (dwResult != 0)
  {
    m_pPrivate->sApplication = CcWString(szPath, dwResult).getString();
  }
  else
  {
    wprintf(L"GetModuleFileName failed w/err 0x%08lx\n", GetLastError());
  }

  if (sName.length())
  {
    open(bOpenReadOnly);
  }
}

CcWindowsServiceControl::~CcWindowsServiceControl()
{
  if (m_pPrivate->hService != nullptr)
  {
    CloseServiceHandle(m_pPrivate->hService);
  }
  if (m_pPrivate->hManager != nullptr)
  {
    CloseServiceHandle(m_pPrivate->hManager);
  }
  CCDELETE(m_pPrivate);
}

bool CcWindowsServiceControl::setDisplayName(const CcWString& sName)
{
  m_sDisplayName = sName;
  bool bRet = true;
  if (serviceOpened())
  {
    BOOL bSuccess = ChangeServiceConfigW(
      m_pPrivate->hService,
      SERVICE_WIN32_OWN_PROCESS,
      static_cast<DWORD>(m_eStartType),
      SERVICE_ERROR_IGNORE,
      nullptr, // lpBinaryPathName
      nullptr, // lpLoadOrderGroup
      nullptr, // lpdwTagId
      nullptr, // lpDependencies
      nullptr, // lpServiceStartName
      nullptr, // lpPassword
      m_sDisplayName.getWcharString()  //lpDisplayName
    );
    if (!bSuccess)
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::setArguments(const CcArguments& oArguments)
{
  bool bRet = true;
  m_pPrivate->oArguments = oArguments;
  if (serviceOpened())
  {
    CcString sBinaryPath = m_pPrivate->sApplication;
    if (m_pPrivate->oArguments.size())
      sBinaryPath << CcGlobalStrings::Seperators::Space << m_pPrivate->oArguments.getLine();
    BOOL bSuccess = ChangeServiceConfigW(
      m_pPrivate->hService,
      SERVICE_WIN32_OWN_PROCESS,
      SERVICE_NO_CHANGE,
      SERVICE_ERROR_IGNORE,
      sBinaryPath.getWString().getWcharString(), // lpBinaryPathName
      nullptr, // lpLoadOrderGroup
      nullptr, // lpdwTagId
      nullptr, // lpDependencies
      nullptr, // lpServiceStartName
      nullptr, // lpPassword
      nullptr  //lpDisplayName
    );
    if (!bSuccess)
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::setStartType(CcWindowsServiceControl::EStartType eStartType)
{
  bool bRet = true;
  m_eStartType = eStartType;
  if (serviceOpened())
  {
    BOOL bSuccess = ChangeServiceConfigW(
      m_pPrivate->hService,
      SERVICE_WIN32_OWN_PROCESS,
      static_cast<DWORD>(m_eStartType),
      SERVICE_ERROR_IGNORE,
      nullptr, // lpBinaryPathName
      nullptr, // lpLoadOrderGroup
      nullptr, // lpdwTagId
      nullptr, // lpDependencies
      nullptr, // lpServiceStartName
      nullptr, // lpPassword
      nullptr  //lpDisplayName
    );
    if (!bSuccess)
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::setUsername(const CcWString& sName, const CcWString& sPassword)
{
  m_pPrivate->sUsername = sName;
  m_pPrivate->sPassword = sPassword;
  bool bRet = true;
  if (serviceOpened())
  {
    BOOL bSuccess = ChangeServiceConfigW(
      m_pPrivate->hService,
      SERVICE_WIN32_OWN_PROCESS,
      static_cast<DWORD>(m_eStartType),
      SERVICE_ERROR_IGNORE,
      nullptr, // lpBinaryPathName
      nullptr, // lpLoadOrderGroup
      nullptr, // lpdwTagId
      nullptr, // lpDependencies
      m_pPrivate->sUsername.getWcharString(), // lpServiceStartName
      m_pPrivate->sPassword.getWcharString(), // lpPassword
      nullptr  //lpDisplayName
    );
    if (!bSuccess)
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::setServiceAccessRigths(unsigned int uiAccessRights)
{
  m_pPrivate->uiAccessRights = uiAccessRights;
  return true;
}

bool CcWindowsServiceControl::addDependency(const CcWString& sName)
{
  m_pPrivate->oDependencies.append(sName.getWcharString(), sName.length());
  m_pPrivate->oDependencies.append(0);
  m_pPrivate->oDependencies.append(0);
  bool bRet = true;
  if (serviceOpened())
  {
    BOOL bSuccess = ChangeServiceConfigW(
      m_pPrivate->hService,
      SERVICE_WIN32_OWN_PROCESS,
      static_cast<DWORD>(m_eStartType),
      SERVICE_ERROR_IGNORE,
      nullptr, // lpBinaryPathName
      nullptr, // lpLoadOrderGroup
      nullptr, // lpdwTagId
      &m_pPrivate->oDependencies[0], // lpDependencies
      nullptr, // lpServiceStartName
      nullptr, // lpPassword
      nullptr  //lpDisplayName
    );
    if (!bSuccess)
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::create()
{
  bool bRet = false;
  if (serviceManagerAvailable())
  {
    // Setup Username / Password if isset
    CcString sBinaryPath = m_pPrivate->sApplication;
    if (m_pPrivate->oArguments.size())
      sBinaryPath << CcGlobalStrings::Seperators::Space << m_pPrivate->oArguments.getLine();
    const wchar_t* pUsername = (m_pPrivate->sUsername.length() > 0) ? m_pPrivate->sUsername.getWcharString() : nullptr;
    const wchar_t* pPassword = (m_pPrivate->sUsername.length() > 0) ? m_pPrivate->sPassword.getWcharString() : nullptr;
    m_pPrivate->hService = CreateServiceW(m_pPrivate->hManager,                   // SCManager database
      m_sName.getWcharString(),                   // Name of service
      m_sDisplayName.getWcharString(),            // Name to display
      SERVICE_QUERY_STATUS,                       // Desired access
      SERVICE_WIN32_OWN_PROCESS,                  // Service type
      static_cast<DWORD>(m_eStartType),           // Service start type
      SERVICE_ERROR_NORMAL,                       // Error control type
      sBinaryPath.getWString().getWcharString(),  // Service's binary
      nullptr,                                    // No load ordering group
      nullptr,                                    // No tag identifier
      &m_pPrivate->oDependencies[0],                // Dependencies
      pUsername,                     // Service running account
      pPassword                      // Password of the account
    );
    if (m_pPrivate->hService != nullptr)
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::remove()
{
  bool bRet = false;
  if (serviceManagerAvailable())
  {
    if (this->open(false))
    {
      if (FALSE == DeleteService(m_pPrivate->hService))
      {
        uint32 uiError = GetLastError();
        if (ERROR_SERVICE_MARKED_FOR_DELETE == uiError)
        {
          bRet = true;
        }
        else
        {
          CCDEBUG(CcString::fromNumber(uiError));
        }
      }
      else
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::start()
{
  bool bRet = false;
  if (serviceManagerAvailable())
  {
    if (this->open(false))
    {
      if (FALSE == StartServiceW(m_pPrivate->hService, 0, NULL))
      {
        uint32 uiError = GetLastError();
        if (ERROR_SERVICE_MARKED_FOR_DELETE == uiError)
        {
          bRet = true;
        }
        else
        {
          CCDEBUG(CcString::fromNumber(uiError));
        }
      }
      else
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::stop()
{
  bool bRet = false;
  if (serviceManagerAvailable())
  {
    if (open(false))
    {
      SERVICE_STATUS oCurrentState;
      if (FALSE == ControlService(m_pPrivate->hService, SERVICE_CONTROL_STOP, &oCurrentState))
      {
        uint32 uiError = GetLastError();
        if (ERROR_SERVICE_MARKED_FOR_DELETE == uiError)
        {
          bRet = true;
        }
        else
        {
          CCDEBUG(CcString::fromNumber(uiError));
        }
      }
      else
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::open(bool bOpenReadOnly)
{
  bool bRet = false;
  if (serviceManagerAvailable())
  {
    if (bOpenReadOnly)
    {
      //m_pPrivate->uiAccessRights = SERVICE_QUERY_STATUS || SERVICE_QUERY_CONFIG;
    }
    m_pPrivate->hService = OpenServiceW(m_pPrivate->hManager, m_sName.getWcharString(), m_pPrivate->uiAccessRights);
    if (m_pPrivate->hService != nullptr)
    {
      bRet = true;
      updateConfig();
    }
    else
    {
      wprintf(L"OpenServiceW failed w/err 0x%08lx\n", GetLastError());
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::updateConfig()
{
  bool bRet = false;
  if (serviceOpened())
  {
    DWORD dwBytesRequired;
    if (FALSE == QueryServiceConfigW(m_pPrivate->hService, nullptr, 0, &dwBytesRequired) &&
      GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
      CCNEWARRAYTYPE(pBuffer, char, dwBytesRequired);
      if (pBuffer != nullptr)
      {
        QUERY_SERVICE_CONFIGW* pServiceConfig = CCVOIDPTRCAST(QUERY_SERVICE_CONFIGW*,pBuffer);
        BOOL bSuccess = QueryServiceConfigW(
                        m_pPrivate->hService,
                        pServiceConfig,
                        dwBytesRequired,
                        &dwBytesRequired
        );
        if (bSuccess)
        {
          m_sDisplayName.set(pServiceConfig->lpDisplayName);
          m_eStartType = static_cast<CcWindowsServiceControl::EStartType>(pServiceConfig->dwStartType);
          CcWString sCurrentPath(pServiceConfig->lpBinaryPathName);
          CcString sEscaped = CcStringUtil::escapeString(sCurrentPath.getString());
          m_pPrivate->oArguments.parseArguments(sEscaped);
          if (m_pPrivate->oArguments.size() > 0)
          {
            m_pPrivate->sApplication = m_pPrivate->oArguments[0];
            m_pPrivate->oArguments.remove(0);
          }
          else
          {
            m_pPrivate->sApplication = L"";
          }
          m_pPrivate->sUsername.set(pServiceConfig->lpServiceStartName);
        }
        else
        {
          wprintf(L"QueryServiceConfigW failed w/err 0x%08lx\n", GetLastError());
        }
        CCDELETEARR(pBuffer);
      }
    }
  }
  return bRet;
}

uint32 CcWindowsServiceControl::getStatus()
{
  uint32 uiStatus = UINT32_MAX;
  if (serviceOpened())
  {
    SERVICE_STATUS oServiceStatus;
    if (TRUE == QueryServiceStatus(m_pPrivate->hService, &oServiceStatus))
    {
      uiStatus = oServiceStatus.dwCurrentState;
    }
  }
  return uiStatus;
}

bool CcWindowsServiceControl::serviceManagerAvailable()
{
  bool bRet = false;

  if (m_pPrivate->hManager != nullptr &&
      m_pPrivate->hService != nullptr)
  {
    bRet = true;
  }
  else
  {
    m_pPrivate->hManager = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
    if (m_pPrivate->hManager != nullptr)
    {
      bRet = true;
    }
    else
    {
      wprintf(L"OpenSCManagerW failed w/err 0x%08lx\n", GetLastError());
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::serviceOpened()
{
  return m_pPrivate->hService != nullptr;
}
