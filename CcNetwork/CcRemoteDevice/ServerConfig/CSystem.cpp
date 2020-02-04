/*
 * This file is part of CSystem.
 *
 * CSystem is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CSystem is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CSystem.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CSystem
 */
#include "CSystem.h"
#include "CcConfig/CcConfigBinary.h"
#include "NDocumentsGlobals.h"

namespace NRemoteDeviceServerConfig
{

void CSystem::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::WlanAccessPoint)
          oWlanAccessPoint.parseJson(rNode);
        else if(rNode.getName() == NDocumentsGlobals::NConfig::WlanClient)
          oWlanClient.parseJson(rNode);
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::Name &&
           rNode.value().isString())
        {
          sName = rNode.value().getString();
          CCDEBUG("CcRemoteDevice device name: " + sName);
        }
      }
    }
  }
}

void CSystem::writeJson(CcJsonNode& rNode)
{
  if(rNode.isObject())
  {
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Name, sName));
    CcJsonNode oWlanAccessPointNode(EJsonDataType::Object);
    oWlanAccessPointNode.setName(NDocumentsGlobals::NConfig::WlanAccessPoint);
    oWlanAccessPoint.writeJson(oWlanAccessPointNode);
    rNode.object().append(oWlanAccessPointNode);
    CcJsonNode oWlanClientNode(EJsonDataType::Object);
    oWlanClientNode.setName(NDocumentsGlobals::NConfig::WlanClient);
    oWlanClient.writeJson(oWlanClientNode);
    rNode.object().append(oWlanClientNode);
  }
}

void CSystem::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CcConfigBinary::EType::Name:
        sName = pItem->getString();
        break;
      case CcConfigBinary::EType::WlanAccessPoint:
        oWlanAccessPoint.parseBinary(pItem, uiMaxSize);
        break;
      case CcConfigBinary::EType::WlanClient:
        oWlanClient.parseBinary(pItem, uiMaxSize);
        break;
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CSystem::writeBinary(CcConfigBinary::CItem* pItem, size_t& uiMaxSize)
{
  CcConfigBinary::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CcConfigBinary::EType::System);
  if(pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::Name, sName, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += oWlanClient.writeBinary(pItem, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += oWlanAccessPoint.writeBinary(pItem, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

}
