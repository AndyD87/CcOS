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

namespace NRemoteDevice
{
namespace Server
{
namespace Config
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

const CcConfigBinary::CItem* CRestApi::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    CCERROR("Wrong config item");
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CRestApi::writeBinary(IIo& pStream)
{
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::RestApi);
  if(uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
  }
  return uiWritten;
}

}
}
}
