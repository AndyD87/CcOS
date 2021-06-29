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
 * @brief     Implementation of Class CcWmiInterface
 */

#include "CcWmiInterface.h"
#include <cstdio>
#include <comdef.h>
#include "CcString.h"
#include "CcWString.h"

# pragma comment(lib, "wbemuuid.lib")

CcWmiInterface::CcWmiInterface() :
  m_pLoc(nullptr),
  m_pSvc(nullptr)
{
}

CcWmiInterface::~CcWmiInterface()
{
  CoUninitialize();
}

CcStatus CcWmiInterface::open()
{
  bool bRet = false;
  HRESULT hres;

  // Step 1: --------------------------------------------------
  // Initialize COM. ------------------------------------------

  hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
  if ( hres == S_OK)
  {
    // Obtain the initial locator to WMI -------------------------
    hres = CoCreateInstance(
        CLSID_WbemLocator,             
        0, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator, (LPVOID *) &m_pLoc);
    if ( hres == S_OK)
    {
      // Connect to CIM-Server
      hres = m_pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
            nullptr,                    // User name. nullptr = current user
            nullptr,                    // User password. nullptr = current
            nullptr,                    // Locale. nullptr indicates current
            0,                       // Security flags.
            0,                       // Authority (for example, Kerberos)
            0,                       // Context object 
            &m_pSvc                  // pointer to IWbemServices proxy
            );
        
      if ( hres == S_OK)
      {
        // Set security levels on the proxy -------------------------
        hres = CoSetProxyBlanket(
            m_pSvc,                        // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
            nullptr,                        // Server principal name 
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            nullptr,                        // client identity
            EOAC_NONE                    // proxy capabilities 
        );
        if ( hres == S_OK)
        {
          bRet = true;
        }
      }
    }
  }

  if(bRet == false)
  {
    close();
  }
  return true;   // Program successfully completed.
}

CcStatus CcWmiInterface::close()
{
  if(m_pSvc != nullptr)
  {
    m_pSvc->Release();
    m_pSvc=nullptr;
  }
  if(m_pLoc != nullptr)
  {
    m_pLoc->Release();
    m_pLoc=nullptr;
  }
  return true;
}

CcSqlResult CcWmiInterface::query(const CcString& queryString)
{
  CcSqlResult sRet;
  HRESULT hres;
  // Use the IWbemServices pointer to make requests of WMI ----
  IEnumWbemClassObject* pEnumerator = nullptr;
  hres = m_pSvc->ExecQuery(
    bstr_t(L"WQL"),
    bstr_t(queryString.getWString().getLPCWSTR()),
    WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
    nullptr,
    &pEnumerator);
  if (hres == S_OK)
  {
    // Read Query-Result
    IWbemClassObject *pclsObj = nullptr;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
      HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
        &pclsObj, &uReturn);

      if (0 == uReturn)
      {
        break;
      }
      VARIANT vtProp;
      // Get Size of current partition in list
      CIMTYPE pvtType;
      hr = pclsObj->Get(L"Size", 0, &vtProp, &pvtType, 0);
      CcString sSize(vtProp.bstrVal);
      bool bConvOk;
      size_t ullSize = sSize.toUint32(&bConvOk);
      CCUNUSED(ullSize);
      VariantClear(&vtProp);
      pclsObj->Release();
    }
    pEnumerator->Release();
  }
  return sRet;
}
