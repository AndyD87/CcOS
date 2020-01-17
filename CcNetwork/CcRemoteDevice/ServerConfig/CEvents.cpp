/*
 * This file is part of CEvents.
 *
 * CEvents is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CEvents is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CEvents.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CEvents
 */
#include "CEvents.h"

namespace NRemoteDeviceServerConfig
{

void CEvents::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
  }
}

void CEvents::parseBinary(const CBinaryFormat::CItem*& pItem, size_t& uiMaxSize)
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
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CEvents::writeBinary(CBinaryFormat::CItem*& pItem, size_t& uiMaxSize)
{
  CCUNUSED(pItem);
  CCUNUSED(uiMaxSize);
  return false;
}

}
