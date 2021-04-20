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
 * @brief     Implemtation of class CcDhcpGlobals
 */
#include "CcDhcpGlobals.h"

namespace CcDhcpGlobals
{
  namespace Strings
  {
    CcConstString_C(ServerName, "CcDhcpServer");
    CcConstString_C(ClientName, "CcDhcpClient");
  }
  namespace Server
  {
    const CcIp DefaultServerIp(10, 0, 1, 1);
    const CcIp DefaultIpBegin (10, 0, 1, 10);
    const CcIp DefaultIpEnd   (10, 0, 1, 100);
    const CcIp DefaultSubnet  (255, 255, 255, 0);
    const uint32 DefaultLeaseTime = 691200; //!< Default Lease Time 8 Days
    const uint32 DefaultRenewTime = 345600; //!< Defautl Renew Time half of LeaseTime 4 Days
    const uint32 DefaultRebindTime= 604800; //!< Defautl Renew Time 7/8 of LeaseTime 7 Days
    namespace Config
    {
      CcConstString_C(RootNode   ,"CcDhcpServer");
      CcConstString_C(IpBegin    ,"IpBegin");
      CcConstString_C(IpEnd      ,"IpEnd");
      CcConstString_C(Subnet     ,"Subnet");
      CcConstString_C(Gateway    ,"Gateway");
      CcConstString_C(Dns        ,"Dns");
      CcConstString_C(Bootfile   ,"Bootfile");
      CcConstString_C(BootfileMbr,"BootfileMbr");
      CcConstString_C(BootfileEfi,"BootfileEfi");
    }
  }
}
