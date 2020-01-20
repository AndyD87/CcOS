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

namespace NRemoteDeviceServerConfig
{

void CWlanCredentials::parseBinary(const CBinaryFormat::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CBinaryFormat::EType::SSID:
        sSsid = pItem->getString();
        break;
      case CBinaryFormat::EType::Password:
        oPassword = pItem->getString();
        break;
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CWlanCredentials::writeBinary(CBinaryFormat::CItem* pItem, size_t& uiMaxSize)
{
  CBinaryFormat::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CBinaryFormat::EType::WlanCredential);
  if (pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::SSID, sSsid, uiMaxSize);
  }
  if (pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::Password, oPassword.getString(), uiMaxSize);
  }
  if (pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::End);
  }
  pThisItem->setSize(uiWritten);
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
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::KnownAccessPoints)
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
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::Enable &&
           rNode.value().isBool())
        {
          bEnable = rNode.value().getBool();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::DhcpEnable &&
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
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::DhcpEnable, bDhcp));
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::Enable, bEnable));

    CcJsonNode oKnown(EJsonDataType::Array);
    oKnown.setName(CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::KnownAccessPoints);
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

void CWlanClient::parseBinary(const CBinaryFormat::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CBinaryFormat::EType::Enable:
        bEnable = pItem->getBool();
        break;
      case CBinaryFormat::EType::DhcpEnable:
        bDhcp = pItem->getBool();
        break;
      case CBinaryFormat::EType::WlanCredential:
      {
        CWlanCredentials oCredential;
        oCredential.parseBinary(pItem, uiMaxSize);
        oKnownAccessPoints.append(oCredential);
        break;
      }
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CWlanClient::writeBinary(CBinaryFormat::CItem* pItem, size_t& uiMaxSize)
{
  CBinaryFormat::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CBinaryFormat::EType::WlanClient);
  if(pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::Enable, bEnable, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::DhcpEnable, bDhcp, uiMaxSize);
  }
  for (CWlanCredentials& oCredential : oKnownAccessPoints)
  {
    if (pItem->getNext(pItem, uiMaxSize))
    {
      uiWritten += oCredential.writeBinary(pItem, uiMaxSize);
    }
    else
    {
      break;
    }
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

}
