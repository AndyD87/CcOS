/*
 * This file is part of CInterfaces.
 *
 * CInterfaces is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CInterfaces is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CInterfaces.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CInterfaces
 */
#include "CInterfaces.h"
#include "NDocumentsGlobals.h"

namespace NRemoteDevice
{
namespace Server
{
namespace Config
{

void CInterfaces::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::RestApi)
          oRestApi.parseJson(rNode);
        else if(rNode.getName() == NDocumentsGlobals::NConfig::HttpServer)
          oHttpServer.parseJson(rNode);
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::RestApiEnabled &&
           rNode.value().isBool())
        {
          bRestApiEnabled = rNode.value().getBool();
        }
      }
    }
  }
}

void CInterfaces::writeJson(CcJsonNode& rNode)
{
  if(rNode.isObject())
  {
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::RestApiEnabled,
                                     bRestApiEnabled));

    CcJsonNode oRestApiNode(EJsonDataType::Object);
    oRestApiNode.setName(NDocumentsGlobals::NConfig::RestApi);
    oRestApi.writeJson(oRestApiNode);
    rNode.object().append(oRestApiNode);

    CcJsonNode oHttpServerNode(EJsonDataType::Object);
    oHttpServerNode.setName(NDocumentsGlobals::NConfig::HttpServer);
    oHttpServer.writeJson(oHttpServerNode);
    rNode.object().append(oHttpServerNode);
  }
}

const CcConfigBinary::CItem* CInterfaces::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CcConfigBinary::EType::RestApiEnabled:
        bRestApiEnabled = pItem->getBool();
        break;
      case CcConfigBinary::EType::RestApi:
        pItem = oRestApi.parseBinary(pItem, uiMaxSize);
        break;
      case CcConfigBinary::EType::HttpServer:
        pItem = oHttpServer.parseBinary(pItem, uiMaxSize);
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CInterfaces::writeBinary(IIo& pStream)
{
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Interfaces);
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::RestApiEnabled, bRestApiEnabled);
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += oRestApi.writeBinary(pStream);
  }
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += oHttpServer.writeBinary(pStream);
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
