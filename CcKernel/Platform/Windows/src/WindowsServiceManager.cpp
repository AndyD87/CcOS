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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsServiceManager
 **/
#include "WindowsServiceManager.h"
#include "CcKernel.h"
#include "CcWString.h"

WindowsServiceManager::WindowsServiceManager(const CcString& sServiceName):
  m_sServiceName(sServiceName)
{

}

WindowsServiceManager::~WindowsServiceManager() 
{
  disconnectService();
}

bool WindowsServiceManager::available()
{
  bool bRet = true;
  if (connectService(SERVICE_QUERY_STATUS))
  {
    bRet = true;
    disconnectService();
  }
  return bRet;
}

bool WindowsServiceManager::stop()
{
  bool bRet = false;
  if (connectService(SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS))
  {   
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = nullptr;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    uint64 dwStartTime = GetTickCount64();
    uint64 dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if (EnumDependentServices(m_schService, SERVICE_ACTIVE,
      lpDependencies, 0, &dwBytesNeeded, &dwCount))
    {
      bRet = true;
    }
    else
    {
      // Allocate a buffer for the dependencies.
      lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);

      if (lpDependencies != nullptr)
      {
          // Enumerate the dependencies.
          if (!EnumDependentServices(m_schService, SERVICE_ACTIVE,
            lpDependencies, dwBytesNeeded, &dwBytesNeeded,
            &dwCount))
            return FALSE;

          for (i = 0; i < dwCount; i++)
          {
            ess = *(lpDependencies + i);
            // Open the service.
            hDepService = OpenService(m_schSCManager,
              ess.lpServiceName,
              SERVICE_STOP | SERVICE_QUERY_STATUS);

            if (!hDepService)
              return FALSE;

              // Send a stop code.
              if (!ControlService(hDepService,
                SERVICE_CONTROL_STOP,
                (LPSERVICE_STATUS) &ssp))
                return FALSE;

              // Wait for the service to stop.
              while (ssp.dwCurrentState != SERVICE_STOPPED)
              {
                Sleep(ssp.dwWaitHint);
                if (!QueryServiceStatusEx(
                  hDepService,
                  SC_STATUS_PROCESS_INFO,
                  (LPBYTE) &ssp,
                  sizeof(SERVICE_STATUS_PROCESS),
                  &dwBytesNeeded))
                  return FALSE;

                if (ssp.dwCurrentState == SERVICE_STOPPED)
                  break;

                if (GetTickCount64() - dwStartTime > dwTimeout)
                  return FALSE;
              }
              // Always release the service handle.
              CloseServiceHandle(hDepService);
          }
          CCDEBUG("Exception received, do nothing.");
          // Always free the enumeration buffer.
          HeapFree(GetProcessHeap(), 0, lpDependencies);
      }
    }
  }
  return bRet;
}

bool WindowsServiceManager::start()
{
  bool bRet = false;

  return bRet;
}

bool WindowsServiceManager::disable()
{
  bool bRet = false;

  return bRet;
}

bool WindowsServiceManager::connectService(DWORD AccessFlags)
{
  bool bRet = false;
  m_schSCManager = OpenSCManagerW(nullptr,                    // local computer
    nullptr,                    // ServicesActive database 
    SC_MANAGER_ALL_ACCESS);  // full access rights 
  if (nullptr != m_schSCManager)
  {
    m_schService = OpenServiceW(
      m_schSCManager,         // SCM database 
      m_sServiceName.getWString().getLPCWSTR(),        // name of service 
      AccessFlags );
    if (m_schService != nullptr)
    {
      bRet = true;
    }
    else
    {
      CloseServiceHandle(m_schSCManager);
      m_schSCManager = nullptr;
    }
  }
  return bRet;
}

void WindowsServiceManager::disconnectService()
{
  if (m_schService != nullptr)
  {
    CloseServiceHandle(m_schService);
    m_schService = nullptr;
  }
  if (m_schSCManager != nullptr)
  {
    CloseServiceHandle(m_schSCManager);
    m_schSCManager = nullptr;
  }
}
