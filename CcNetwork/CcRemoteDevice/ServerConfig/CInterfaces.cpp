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

namespace NRemoteDeviceServerConfig
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
    rNode.object().append(oRestApiNode);
    oRestApi.writeJson(oRestApiNode);
  }
}

void CInterfaces::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
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
        oRestApi.parseBinary(pItem, uiMaxSize);
        break;
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CInterfaces::writeBinary(CcConfigBinary::CItem* pItem, size_t& uiMaxSize)
{
  CcConfigBinary::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CcConfigBinary::EType::Interfaces);
  if(pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::RestApiEnabled, bRestApiEnabled, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += oRestApi.writeBinary(pItem, uiMaxSize);
  }
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

}
