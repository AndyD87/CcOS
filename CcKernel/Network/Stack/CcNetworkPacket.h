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
 * @brief     Class CcNetworkPacket
 */
#pragma once

#include "Network/CcMacAddress.h"
#include "CcBase.h"
#include "CcBase.h"
#include "CcBufferList.h"
#include "CcIp.h"

class INetwork;

/**
 * @brief Network packet for tracking informations from socket to tnterface and vice versa.
 *        This object is based on buffer list to append or prepand Header informations to packet
 *        on each layer if required.
 */
class CcKernelSHARED CcNetworkPacket : public CcBufferList
{
public:
  INetwork* pInterface = nullptr; //!< Incoming or transmitting interface of packet.
  CcIp oSourceIp;                 //!< Read source ip of packet
  CcMacAddress oSourceMac;        //!< Read phyiscal address of source
  uint16 uiSourcePort   = 0;      //!< Read source port of packet
  CcIp oTargetIp;                 //!< Read target ip of packet
  CcMacAddress oTargetMac;        //!< Read phyiscal address of target
  uint16 uiTargetPort   = 0;      //!< Read target port of packet
  uint16 uiProtocolType = 0;      //!< Protocol type of last protocol
  uint16 uiSize         = 0;      //!< Size of Buffer in List.

  /**
   * @brief Create new packet buffer.
   * @return Buffer of created network packet.
   */
  CcNetworkPacket* createAndRetarget();
};

//! Packet as reference
typedef CcNetworkPacket*& CcNetworkPacketRef;
