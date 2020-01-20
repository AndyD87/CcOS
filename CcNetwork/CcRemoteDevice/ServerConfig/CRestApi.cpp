/*
 * This file is part of CRestApi.
 *
 * CRestApi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CRestApi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CRestApi.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CRestApi
 */
#include "CRestApi.h"

namespace NRemoteDeviceServerConfig
{

void CRestApi::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
      }
      else if(rNode.isValue())
      {
      }
    }
  }
}

void CRestApi::writeJson(CcJsonNode& rNode)
{
  if(rNode.isObject())
  {
  }
}

void CRestApi::parseBinary(const CBinaryFormat::CItem*& pItem, size_t& uiMaxSize)
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

size_t CRestApi::writeBinary(CBinaryFormat::CItem*& pItem, size_t& uiMaxSize)
{
  CBinaryFormat::CItem* pThisItem = pItem;
  size_t uiWritten = 0;
  pItem->write(CBinaryFormat::EType::System, nullptr, 0);
  pItem->setSize(0);
  if(pItem->getNext(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

}
