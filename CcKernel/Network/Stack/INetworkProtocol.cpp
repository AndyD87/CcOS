/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class INetworkProtocol
 */
#include "Network/Stack/CcNetworkStack.h"
#include "Network/Stack/INetworkProtocol.h"

INetworkProtocol* INetworkProtocol::findProtocol(uint16 uiProtocolType)
{
  INetworkProtocol* pProtocol = nullptr;
  for (INetworkProtocol* pTemp : *this)
  {
    if (pTemp->getProtocolType() == uiProtocolType)
    {
      pProtocol = pTemp;
    }
  }
  return pProtocol;
}

CcNetworkStack* INetworkProtocol::getNetworkStack()
{
  CcNetworkStack* pNs = nullptr;
  INetworkProtocol* pCurrentProtocol = m_pParentProtocol;
  while(nullptr != pCurrentProtocol)
  {
    pNs = static_cast<CcNetworkStack*>(pCurrentProtocol);
    pCurrentProtocol = pCurrentProtocol->m_pParentProtocol;
  }
  return pNs;
}
