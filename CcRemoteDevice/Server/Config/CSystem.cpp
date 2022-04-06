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
#include "Config/CcConfigBinary.h"
#include "NDocumentsGlobals.h"

namespace NRemoteDevice
{
namespace Server
{
namespace Config
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

const CcConfigBinary::CItem* CSystem::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
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
        pItem = oWlanAccessPoint.parseBinary(pItem, uiMaxSize);
        break;
      case CcConfigBinary::EType::WlanClient:
        pItem = oWlanClient.parseBinary(pItem, uiMaxSize);
        break;
      default:
        // Unhandled/Not required
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CSystem::writeBinary(IIo& pStream)
{
  size_t uiWrittenTemp = 0;
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::System);
  if(uiWritten != SIZE_MAX)
  {
    uiWrittenTemp = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Name, sName);
    if (uiWrittenTemp != SIZE_MAX) uiWritten += uiWrittenTemp;
    else uiWritten = SIZE_MAX;
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWrittenTemp = oWlanClient.writeBinary(pStream);
    if (uiWrittenTemp != SIZE_MAX) uiWritten += uiWrittenTemp;
    else uiWritten = SIZE_MAX;
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWrittenTemp = oWlanAccessPoint.writeBinary(pStream);
    if (uiWrittenTemp != SIZE_MAX) uiWritten += uiWrittenTemp;
    else uiWritten = SIZE_MAX;
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWrittenTemp = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
    if (uiWrittenTemp != SIZE_MAX) uiWritten += uiWrittenTemp;
    else uiWritten = SIZE_MAX;
  }
  return uiWritten;
}

}
}
}
