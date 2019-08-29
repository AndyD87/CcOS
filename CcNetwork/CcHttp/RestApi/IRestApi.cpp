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
  if(pParent)
  {
    pParent->appendProvider(this);
  }
}

IRestApi::~IRestApi()
{
}

bool IRestApi::get(CcHttpWorkData& oData)
{
  return custom(oData);
}

bool IRestApi::post(CcHttpWorkData& oData)
{
  return custom(oData);
}

bool IRestApi::put(CcHttpWorkData& oData)
{
  return custom(oData);
}

bool IRestApi::del(CcHttpWorkData& oData)
{
  return custom(oData);
}

bool IRestApi::patch(CcHttpWorkData& oData)
{
  return custom(oData);
}

bool IRestApi::custom(CcHttpWorkData& oData)
{
  bool bSuccess = false;
  sendMethodNotFound(oData);
  return bSuccess;
}

bool IRestApi::checkAuth(CcHttpWorkData& oData)
{
  bool bAuthSuccessfull = false;
  CCUNUSED(oData);
  if (m_pParent == nullptr)
  {
    bAuthSuccessfull = true;
  }
  else
  {
    bAuthSuccessfull = m_pParent->checkAuth(oData);
  }
  return bAuthSuccessfull;
}

bool IRestApi::execPath(CcStringList& oPath, CcHttpWorkData& oData)
{
  bool bSuccess = false;
  if(oPath.size() > 0)
  {
    IRestApi* pChild = getProvider(oPath[0]);
    if(pChild != nullptr)
    {
      oPath.remove(0);
      bSuccess = pChild->execPath(oPath, oData);
    }
    else
    {
      switch (oData.getRequestType())
      {
        case EHttpRequestType::Get:
#ifdef DEBUG
          if (oPath.size() != 0 && oPath[0] == "list")
          {
            // fall through to List
          }
          else
          {
#endif // DEBUG
            sendMethodNotFound(oData);
            break;
#ifdef DEBUG
          }
          CCFALLTHROUGH;
#endif // DEBUG
        case EHttpRequestType::List:
        {
          bSuccess = true;
          sendList(oData);;
          break;
        }
        default:
          bSuccess = custom(oData);
      }
    }
  }
  else if (checkAuth(oData))
  {
    switch (oData.getRequestType())
    {
      case EHttpRequestType::Get:
        get(oData);
        break;
      case EHttpRequestType::Post:
        post(oData);
        break;
      case EHttpRequestType::Put:
        put(oData);
        break;
      case EHttpRequestType::Delete:
        del(oData);
        break;
      case EHttpRequestType::Patch:
        patch(oData);
        break;
      default:
        custom(oData);
    }
  }
  else
  {
    sendAuthRequired(oData);
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

void IRestApi::sendList(CcHttpWorkData& oData)
{
  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  CcJsonDocument oJsonDoc;
  oJsonDoc.getJsonData().setJsonArray();
  CcJsonArray& rArray = oJsonDoc.getJsonData().array();
  for (IRestApi* pChildItem : m_oChilds)
  {
    rArray.append(CcJsonNode("", pChildItem->getPath()));
  }
  oData.writeChunked(oJsonDoc.getDocument());
}

void IRestApi::sendMethodNotFound(CcHttpWorkData& oData)
{
  oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
  oData.sendHeader();
}

void IRestApi::sendAuthRequired(CcHttpWorkData& oData)
{
  oData.getResponse().setError(CcHttpGlobals::EError::ErrorAuthRequired);
  oData.getResponse().setWwwAuthenticate("Basic realm=\"CcOS Basic Auth\"");
  oData.sendHeader();
}
