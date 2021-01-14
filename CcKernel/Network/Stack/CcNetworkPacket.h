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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcNetworkPacket
 */
#ifndef H_CcNetworkPacket_H_
#define H_CcNetworkPacket_H_

#include "Network/CcMacAddress.h"
#include "CcBase.h"
#include "CcBase.h"
#include "CcBufferList.h"
#include "CcIp.h"

class INetwork;

class CcKernelSHARED CcNetworkPacket : public CcBufferList
{
public:
  INetwork* pInterface = nullptr;
  CcIp oSourceIp;
  CcMacAddress oSourceMac;
  uint16 uiSourcePort   = 0;
  CcIp oTargetIp;
  CcMacAddress oTargetMac;
  uint16 uiTargetPort   = 0;
  uint16 uiProtocolType = 0;
  uint16 uiSize         = 0;

  CcNetworkPacket* createAndRetarget();
};

typedef CcNetworkPacket*& CcNetworkPacketRef;

#endif //H_CcNetworkPacket_H_
