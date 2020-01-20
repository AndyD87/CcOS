/*
 * This file is part of CStartup.
 *
 * CStartup is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CStartup is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CStartup.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CStartup
 */
#include "CStartup.h"

namespace NRemoteDeviceServerConfig
{

void CStartup::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
  }
}

void CStartup::parseBinary(const CBinaryFormat::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CStartup::writeBinary(CBinaryFormat::CItem* pItem, size_t& uiMaxSize)
{
  CBinaryFormat::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CBinaryFormat::EType::Startup);
  if(pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CBinaryFormat::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

}
