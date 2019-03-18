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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcIpProtocol
 */
#include "Network/CcIpProtocol.h"
#include "CcStringList.h"

CcIpProtocol::CcIpProtocol(INetworkProtocol* pParent) :
  INetworkProtocol(pParent)
{
}

CcIpProtocol::~CcIpProtocol()
{
}

uint16 CcIpProtocol::getProtocolType() const
{
  return 0x0800;
}

bool CcIpProtocol::transmit(void* pFrame, size_t uiSize)
{
  return false;
}

bool CcIpProtocol::receive(void* pFrame, size_t uiSize)
{
  return false;
}