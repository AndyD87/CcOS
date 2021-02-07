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
 * @brief     Class CcDhcpGlobals
 **/
#pragma once

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcIp.h"
#include "CcString.h"

namespace CcDhcpGlobals
{
  namespace Server
  {
    extern const CcDhcpSHARED CcIp DefaultServerIp;     //!< Default server ip
    extern const CcDhcpSHARED CcIp DefaultIpBegin;      //!< Default start ip of dhcp
    extern const CcDhcpSHARED CcIp DefaultIpEnd;        //!< Default end ip of dhcp
    extern const CcDhcpSHARED CcIp DefaultSubnet;       //!< Default subnet
    extern const CcDhcpSHARED uint32 DefaultLeaseTime;  //!< Default lease time of client ip
    extern const CcDhcpSHARED uint32 DefaultRenewTime;  //!< Default renew time of client ip
    extern const CcDhcpSHARED uint32 DefaultRebindTime; //!< Default rebind time of client ip
    namespace Config
    {
      extern CcDhcpSHARED CcConstString_H(RootNode);    //!< Default root node for dhcp config
      extern CcDhcpSHARED CcConstString_H(IpBegin);     //!< Default config node for ip begin
      extern CcDhcpSHARED CcConstString_H(IpEnd);       //!< Default config node for ip end
      extern CcDhcpSHARED CcConstString_H(Subnet);      //!< Default config node for subnet
      extern CcDhcpSHARED CcConstString_H(Gateway);     //!< Default config node for gateway
      extern CcDhcpSHARED CcConstString_H(Dns);         //!< Default config node for dns
      extern CcDhcpSHARED CcConstString_H(Bootfile);    //!< Default config node for bootfile
      extern CcDhcpSHARED CcConstString_H(BootfileEfi); //!< Default config node for bootfile efi
      extern CcDhcpSHARED CcConstString_H(BootfileMbr); //!< Default config node for bootfile mbr
    }
  }

}
