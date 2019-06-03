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
 * @brief     Implementation of Class IRestApi
 */
#include "IRestApi.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"

IRestApi::IRestApi(IRestApi* pParent, const CcString& sPath) :
  m_pParent(pParent),
  m_sPath(sPath)
{

}

IRestApi::~IRestApi()
{
}

bool IRestApi::exec(CcStringList& oPath, CcHttpWorkData& oData)
{
  bool bSuccess = false;
  switch (oData.getRequestType())
  {
#ifdef DEBUG
    case EHttpRequestType::Get:
      if (oPath.size() == 0 && oPath[0] != "list")
      {
        oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
        oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
        oData.sendHeader();
      }
      CCFALLTHROUGH
#endif
    case EHttpRequestType::List:
    {
      bSuccess = true;
      oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
      CcJsonDocument oJsonDoc;
      oJsonDoc.getJsonData().setJsonArray();
      CcJsonArray& rArray = oJsonDoc.getJsonData().array();
      for (IRestApi* pChild : m_oChilds)
      {
        rArray.append(pChild->getPath());
      }
      oData.sendHeader();
      break;
    }
    default:
      oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
      oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
      oData.sendHeader();
  }
  return bSuccess;
}

IRestApi* IRestApi::getProvider(const CcString& sPath)
{
  IRestApi* pFound = nullptr;
  for (IRestApi* pChild : m_oChilds)
  {
    if (sPath == pChild->getPath())
    {
      pFound = pChild;
      break;
    }
  }
  return pFound;
}
