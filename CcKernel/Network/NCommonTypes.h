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
 * @brief     Class NCommonTypes
 */
#pragma once

#include "CcBase.h"

namespace NCommonTypes
{
  namespace NNetwork
  {
    static const uint16 Ethernet = 0; //!< Ethernet type 0
    namespace NEthernet
    {
      static const uint16 IP  = 0x0800; //!<  Type for ip packet
      static const uint16 ARP = 0x0806; //!<  Type for arp packet

      namespace NIp
      {
        static const uint8 ICMP = 0x01; //!< IP type for ICMP
        static const uint8 TCP  = 0x06; //!< IP type for TCP
        static const uint8 UDP  = 0x11; //!< IP type for UDP
      }
    }
  }
}
