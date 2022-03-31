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
#include "NDocumentsGlobals.h"

namespace NRemoteDevice
{
namespace Server
{
namespace Config
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
        if(rNode.getName() == NDocumentsGlobals::NConfig::SSID &&
           rNode.value().isString())
        {
          sSsid = rNode.value().getString();
        }
        if(rNode.getName() == NDocumentsGlobals::NConfig::Password &&
           rNode.value().isString())
        {
          oPassword = rNode.value().getString();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::DhcpEnable &&
                rNode.value().isBool())
        {
          bDhcp = rNode.value().getBool();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::Enable &&
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
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::DhcpEnable, bDhcp));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Enable, bEnable));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::SSID, sSsid));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Password, oPassword.getString()));
  }
}

const CcConfigBinary::CItem* CWlanAccessPoint::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
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
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CWlanAccessPoint::writeBinary(IIo& pStream)
{
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::WlanAccessPoint);
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::SSID, sSsid);
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Password, oPassword.getString());
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Enable, bEnable);
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::DhcpEnable, bDhcp);
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
  }
  return uiWritten;
}

}
}
}
