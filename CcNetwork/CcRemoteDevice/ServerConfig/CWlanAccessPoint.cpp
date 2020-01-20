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
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::SSID &&
           rNode.value().isString())
        {
          sSsid = rNode.value().getString();
        }
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Password &&
           rNode.value().isString())
        {
          oPassword = rNode.value().getString();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Dhcp &&
                rNode.value().isBool())
        {
          bDhcp = rNode.value().getBool();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Enable &&
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
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Dhcp, bDhcp));
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Enable, bEnable));
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::SSID, sSsid));
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Password, oPassword.getString()));
  }
}

void CWlanAccessPoint::parseBinary(const CBinaryFormat::CItem*& pItem, size_t& uiMaxSize)
{
  bool bAllOk = true;
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CBinaryFormat::EType::Version:
        break;
      case CBinaryFormat::EType::System:
        //bAllOk = pItem->getNext(pItem, uiMaxSize);
        //oSystem.parseBinary(pItem, uiMaxSize);
        break;
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CWlanAccessPoint::writeBinary(CBinaryFormat::CItem*& pItem, size_t& uiMaxSize)
{
  CBinaryFormat::CItem* pThisItem = pItem;
  size_t uiWritten = 0;
  pItem->write(CBinaryFormat::EType::System, nullptr, 0);
  pItem->setSize(0);
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::SSID, sSsid, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::Password, oPassword.getString(), uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::Enable, bEnable, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::Dhcp, bDhcp, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

}
