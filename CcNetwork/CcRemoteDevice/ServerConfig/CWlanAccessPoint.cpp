/*
 * This file is part of CWlanAccessPoint.
 *
 * CWlanAccessPoint is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CWlanAccessPoint is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CWlanAccessPoint.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CWlanAccessPoint
 */
#include "CWlanAccessPoint.h"
#include "CcDocumentsGlobals.h"

namespace NRemoteDeviceServerConfig
{

void CWlanAccessPoint::parseJson(CcJsonNode& rJson)
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
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::SSID &&
           rNode.value().isString())
        {
          sSsid = rNode.value().getString();
        }
        if(rNode.getName() == CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::Password &&
           rNode.value().isString())
        {
          oPassword = rNode.value().getString();
        }
        else if(rNode.getName() == CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::DhcpEnable &&
                rNode.value().isBool())
        {
          bDhcp = rNode.value().getBool();
        }
        else if(rNode.getName() == CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::Enable &&
                rNode.value().isBool())
        {
          bEnable = rNode.value().getBool();
        }
      }
    }
  }
}

void CWlanAccessPoint::writeJson(CcJsonNode& rNode)
{
  if(rNode.isObject())
  {
    rNode.object().append(CcJsonNode(CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::DhcpEnable, bDhcp));
    rNode.object().append(CcJsonNode(CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::Enable, bEnable));
    rNode.object().append(CcJsonNode(CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::SSID, sSsid));
    rNode.object().append(CcJsonNode(CcDocumentsGlobals::Config::SystemNs::WlanAccessPointNs::Password, oPassword.getString()));
  }
}

void CWlanAccessPoint::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
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
      case CcConfigBinary::EType::Enable:
        bEnable = pItem->getBool();
        break;
      case CcConfigBinary::EType::DhcpEnable:
        bDhcp = pItem->getBool();
        break;
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CWlanAccessPoint::writeBinary(CcConfigBinary::CItem* pItem, size_t& uiMaxSize)
{
  CcConfigBinary::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CcConfigBinary::EType::WlanAccessPoint);
  if(pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::SSID, sSsid, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::Password, oPassword.getString(), uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::Enable, bEnable, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::DhcpEnable, bDhcp, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

}
