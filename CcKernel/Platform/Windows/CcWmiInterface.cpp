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

#include <wbemidl.h>

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
  // Open only if not already done
  if (m_pSvc == nullptr && m_pLoc == nullptr)
  {
    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------
    HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (hres == S_OK)
    {
      // Obtain the initial locator to WMI -------------------------
      hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *)&m_pLoc);
      if (hres == S_OK)
      {
        // Connect to CIM-Server
        hres = m_pLoc->ConnectServer(
          SysAllocString(L"ROOT\\CIMV2"), // Object path of WMI namespace
          nullptr,                    // User name. nullptr = current user
          nullptr,                    // User password. nullptr = current
          nullptr,                    // Locale. nullptr indicates current
          0,                       // Security flags.
          0,                       // Authority (for example, Kerberos)
          0,                       // Context object 
          &m_pSvc                  // pointer to IWbemServices proxy
        );

        if (hres == S_OK)
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
          if (hres == S_OK)
          {
            bRet = true;
          }
        }
      }
    }

    if (bRet == false)
    {
      close();
    }
  }
  else
  {
    bRet = true;
  }
  return true;   // Program successfully completed.
}

CcStatus CcWmiInterface::close(bool bForce)
{
  if (bForce)
  {
    m_pSvc = nullptr;
    m_pLoc = nullptr;
  }
  else
  {
    if (m_pSvc != nullptr)
    {
      m_pSvc->Release();
      m_pSvc = nullptr;
    }
    if (m_pLoc != nullptr)
    {
      m_pLoc->Release();
      m_pLoc = nullptr;
    }
  }
  return true;
}

CcWmiResult CcWmiInterface::query(const CcString& queryString)
{
  CcWmiResult oResult;
  HRESULT hres;
  // Use the IWbemServices pointer to make requests of WMI ----
  IEnumWbemClassObject* pEnumerator = nullptr;
  if (m_pSvc)
  {
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
      ULONG uReturn = 1;
      CcStringList      oColumns;
      CcList<CcWString> oList;

      // If uReturn is null, no more data available to read
      while (pEnumerator && uReturn != 0)
      {
        CcTableRow oRow;
        // Get Enumerator for all lines
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (S_OK == hr && pclsObj)
        {

          if (oColumns.size() == 0)
          {
            SAFEARRAY* pArray;
            if (WBEM_S_NO_ERROR == pclsObj->GetNames(NULL, WBEM_FLAG_ALWAYS, NULL, &pArray))
            {
              if (SUCCEEDED(SafeArrayLock(pArray)))
              {
                LONG lstart, lend;
                if (SUCCEEDED(SafeArrayGetLBound(pArray, 1, &lstart)))
                {
                  if (SUCCEEDED(SafeArrayGetUBound(pArray, 1, &lend)))
                  {
                    BSTR* pbstr;
                    if (SUCCEEDED(SafeArrayAccessData(pArray, (void**)&pbstr)))
                    {
                      pbstr = pbstr + lstart;
                      while (lstart <= lend)
                      {
                        oColumns.append(*pbstr);
                        oList.append(oColumns.last().getWString());
                        pbstr++;
                        lstart++;
                      }
                    }
                  }
                }
                SafeArrayUnlock(pArray);
              }
            }
            oResult.setColumnCount(oColumns.size());
            oResult.setColumnNames(oColumns);
          }

          for (CcWString& sColumn : oList)
          {
            VARIANT vtProp;
            // Get Size of current partition in list
            CIMTYPE vtType;
            CcVariant oVariant;
            hr = pclsObj->Get(sColumn.getWcharString(), 0, &vtProp, &vtType, 0);
            if (SUCCEEDED(hr))
            {
              switch (vtType)
              {
                case CIM_EMPTY:
                  // Keep no type
                  break;
                case CIM_SINT8:
                  oVariant.set(vtProp, VARENUM::VT_I1);
                  break;
                case CIM_UINT8:
                  oVariant.set(vtProp, VARENUM::VT_UI1);
                  break;
                case CIM_SINT16:
                  oVariant.set(vtProp, VARENUM::VT_I2);
                  break;
                case CIM_UINT16:
                  oVariant.set(vtProp, VARENUM::VT_UI2);
                  break;
                case CIM_SINT32:
                  oVariant.set(vtProp, VARENUM::VT_I4);
                  break;
                case CIM_UINT32:
                  oVariant.set(vtProp, VARENUM::VT_UI4);
                  break;
                case CIM_SINT64:
                  oVariant.set(vtProp, VARENUM::VT_I8);
                  break;
                case CIM_UINT64:
                  oVariant.set(vtProp, VARENUM::VT_UI8);
                  break;
                case CIM_REAL32:
                  oVariant.set(vtProp, VARENUM::VT_R4);
                  break;
                case CIM_REAL64:
                  oVariant.set(vtProp, VARENUM::VT_R8);
                  break;
                case CIM_BOOLEAN:
                  oVariant.set(vtProp, VARENUM::VT_BOOL);
                  break;
                case CIM_STRING:
                  oVariant.set(vtProp, VARENUM::VT_BSTR);
                  break;
                  // Next are unsupported for this interface
                case CIM_DATETIME:
                  //oVariant.set(vtProp, VARENUM::VT_DATE);
                  //break;
                case CIM_CHAR16:
                case CIM_REFERENCE:
                case CIM_OBJECT:
                case CIM_FLAG_ARRAY:
                case CIM_ILLEGAL:
                default:
                  if (IS_FLAG_SET(vtType, VT_ARRAY))
                  {
                    oVariant.set(vtProp, (VARENUM)vtType);
                  }
                  break;
              }
            }
            oRow.append(oVariant);
            VariantClear(&vtProp);
          };
          oResult.addRow(oRow);
          pclsObj->Release();
        }
      }
      pEnumerator->Release();
    }
  }
  return oResult;
}
