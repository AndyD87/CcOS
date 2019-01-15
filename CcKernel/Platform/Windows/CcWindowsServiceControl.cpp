#include <Windows.h>
#include "CcWindowsServiceControl.h"
#include "CcVector.h"

class CcWindowsServiceControlPrivate
{
public:
  SC_HANDLE hManager = NULL;
  SC_HANDLE hService = NULL;
  CcVector<wchar_t> oDependencies;
  CcWString sUsername;
  CcWString sPassword;
  CcWString sCurrentPath;
  DWORD uiAccessRights = SERVICE_ALL_ACCESS;
};

CcWindowsServiceControl::CcWindowsServiceControl(const CcWString& sName) :
  m_sName(sName)
{
  m_pPrivate = new CcWindowsServiceControlPrivate();
  m_pPrivate->oDependencies.append(0);
  m_pPrivate->oDependencies.append(0);
  wchar_t szPath[MAX_PATH];
  DWORD dwResult = GetModuleFileNameW(NULL, szPath, ARRAYSIZE(szPath));
  if (dwResult != 0)
  {
    m_pPrivate->sCurrentPath.set(szPath, dwResult);
  }
  else
  {
    wprintf(L"GetModuleFileName failed w/err 0x%08lx\n", GetLastError());
  }
}

CcWindowsServiceControl::~CcWindowsServiceControl()
{
  if (m_pPrivate->hService != NULL)
  {
    CloseServiceHandle(m_pPrivate->hService);
  }
  if (m_pPrivate->hManager != NULL)
  {
    CloseServiceHandle(m_pPrivate->hManager);
  }
  delete m_pPrivate;
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
      NULL, // lpBinaryPathName 
      NULL, // lpLoadOrderGroup 
      NULL, // lpdwTagId 
      NULL, // lpDependencies 
      NULL, // lpServiceStartName 
      NULL, // lpPassword 
      m_sDisplayName.getWcharString()  //lpDisplayName
    );
    if (!bSuccess)
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::setStartType(EWindowsServiceStartType eStartType)
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
      NULL, // lpBinaryPathName 
      NULL, // lpLoadOrderGroup 
      NULL, // lpdwTagId 
      NULL, // lpDependencies 
      NULL, // lpServiceStartName 
      NULL, // lpPassword 
      NULL  //lpDisplayName
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
      NULL, // lpBinaryPathName 
      NULL, // lpLoadOrderGroup 
      NULL, // lpdwTagId 
      NULL, // lpDependencies 
      m_pPrivate->sUsername.getWcharString(), // lpServiceStartName 
      m_pPrivate->sPassword.getWcharString(), // lpPassword 
      NULL  //lpDisplayName
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
      NULL, // lpBinaryPathName 
      NULL, // lpLoadOrderGroup 
      NULL, // lpdwTagId 
      &m_pPrivate->oDependencies[0], // lpDependencies 
      NULL, // lpServiceStartName 
      NULL, // lpPassword 
      NULL  //lpDisplayName
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
    const wchar_t* pUsername = (m_pPrivate->sUsername.length() > 0) ? m_pPrivate->sUsername.getWcharString() : NULL;
    const wchar_t* pPassword = (m_pPrivate->sUsername.length() > 0) ? m_pPrivate->sPassword.getWcharString() : NULL;
    m_pPrivate->hService = CreateServiceW(m_pPrivate->hManager,                   // SCManager database
      m_sName.getWcharString(),                // Name of service
      m_sDisplayName.getWcharString(),         // Name to display
      SERVICE_QUERY_STATUS,           // Desired access
      SERVICE_WIN32_OWN_PROCESS,      // Service type
      static_cast<DWORD>(m_eStartType),                    // Service start type
      SERVICE_ERROR_NORMAL,           // Error control type
      m_pPrivate->sCurrentPath.getWcharString(),                         // Service's binary
      NULL,                           // No load ordering group
      NULL,                           // No tag identifier
      &m_pPrivate->oDependencies[0],                // Dependencies
      pUsername,                     // Service running account
      pPassword                      // Password of the account
    );
    if (m_pPrivate->hService != NULL)
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::open()
{
  bool bRet = false;
  if (serviceManagerAvailable())
  {
    m_pPrivate->hService = OpenServiceW(m_pPrivate->hManager, m_sName.getWcharString(), m_pPrivate->uiAccessRights);
    if (m_pPrivate->hService != NULL)
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
    if (FALSE == QueryServiceConfigW(m_pPrivate->hService, NULL, 0, &dwBytesRequired) &&
        GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
      PVOID pBuffer = malloc(dwBytesRequired);
      if (pBuffer != nullptr)
      {
        QUERY_SERVICE_CONFIGW* pServiceConfig = static_cast<QUERY_SERVICE_CONFIGW*>(pBuffer);
        BOOL bSuccess = QueryServiceConfigW(
                        m_pPrivate->hService,
                        pServiceConfig,
                        dwBytesRequired,
                        &dwBytesRequired
        );
        if (bSuccess)
        {
          m_sDisplayName.set(pServiceConfig->lpDisplayName);
          m_eStartType = static_cast<EWindowsServiceStartType>(pServiceConfig->dwStartType);
          m_pPrivate->sCurrentPath.set(pServiceConfig->lpBinaryPathName);
          m_pPrivate->sUsername.set(pServiceConfig->lpServiceStartName);
        }
        else
        {
          wprintf(L"QueryServiceConfigW failed w/err 0x%08lx\n", GetLastError());
        }
        free(pBuffer);
      }
    }
  }
  return bRet;
}

bool CcWindowsServiceControl::serviceManagerAvailable()
{
  bool bRet = false;

  if (m_pPrivate->hManager != NULL &&
      m_pPrivate->hService != NULL)
  {
    bRet = true;
  }
  else
  {
    m_pPrivate->hManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
    if (m_pPrivate->hManager != NULL)
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
  return m_pPrivate->hService != NULL;
}