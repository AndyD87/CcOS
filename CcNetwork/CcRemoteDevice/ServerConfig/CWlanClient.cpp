/*
 * This file is part of CWlanClient.
 *
 * CWlanClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CWlanClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CWlanClient.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CWlanClient
 */
#include "CWlanClient.h"
#include "CcJson/CcJsonArray.h"
#include "NDocumentsGlobals.h"

namespace NRemoteDeviceServerConfig
{

const CcConfigBinary::CItem* CWlanCredentials::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CcConfigBinary::EType::SSID:
        sSsid = pItem->getString();
        break;
      case CcConfigBinary::EType::Password:
        oPassword = pItem->getString();
        break;
      default:
        CCERROR("Wrong config item");
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CWlanCredentials::writeBinary(IIo& pStream)
{
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::WlanCredential);
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::SSID, sSsid);
  }
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Password, oPassword.getString());
  }
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
  }
  return uiWritten;
}

void CWlanClient::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
      }
      else if(rNode.isArray())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::KnownAccessPoints)
        {
          for (CcJsonNode& rAps : rNode.array())
          {
            if(rAps.isArray() &&
               rAps.array().size() == 2 &&
               rAps.array()[0].isValue() &&
               rAps.array()[1].isValue() &&
               rAps.array()[0].value().isString() &&
               rAps.array()[1].value().isString())
            {
              CWlanCredentials oCreds;
              oCreds.sSsid = rAps.array()[0].value().getString();
              oCreds.oPassword = rAps.array()[1].value().getString();
              oKnownAccessPoints.add(oCreds);
            }
          }
        }
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::Enable &&
           rNode.value().isBool())
        {
          bEnable = rNode.value().getBool();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::DhcpEnable &&
                rNode.value().isBool())
        {
          bDhcp = rNode.value().getBool();
        }
      }
    }
  }
}

void CWlanClient::writeJson(CcJsonNode& rNode)
{
  if(rNode.isObject())
  {
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::DhcpEnable, bDhcp));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Enable, bEnable));

    CcJsonNode oKnown(EJsonDataType::Array);
    oKnown.setName(NDocumentsGlobals::NConfig::KnownAccessPoints);
    for(const CWlanCredentials& oCredential : oKnownAccessPoints)
    {
      CcJsonNode oCredentialArray(EJsonDataType::Array);
      CcJsonNode oSsid("", oCredential.sSsid);
      CcJsonNode oPassword("", oCredential.oPassword.getString());
      oCredentialArray.array().append(oSsid);
      oCredentialArray.array().append(oPassword);
      oKnown.array().append(oCredentialArray);

    }
    rNode.object().append(oKnown);
  }
}

const CcConfigBinary::CItem* CWlanClient::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CcConfigBinary::EType::Enable:
        bEnable = pItem->getBool();
        break;
      case CcConfigBinary::EType::DhcpEnable:
        bDhcp = pItem->getBool();
        break;
      case CcConfigBinary::EType::WlanCredential:
      {
        CWlanCredentials oCredential;
        pItem = oCredential.parseBinary(pItem, uiMaxSize);
        oKnownAccessPoints.append(oCredential);
        break;
      }
      default:
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CWlanClient::writeBinary(IIo& pStream)
{
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::WlanClient);
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Enable, bEnable);
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::DhcpEnable, bDhcp);
  }
  for (CWlanCredentials& oCredential : oKnownAccessPoints)
  {
    if (uiWritten != SIZE_MAX)
    {
      uiWritten += oCredential.writeBinary(pStream);
    }
    else
    {
      break;
    }
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
  }
  return uiWritten;
}

}
