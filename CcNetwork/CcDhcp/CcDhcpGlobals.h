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
 * @page      CcDhcp
 * @subpage   CcDhcpGlobals
 *
 * @page      CcDhcpGlobals
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpGlobals
 **/
#ifndef H_CcDhcpGlobals_H_
#define H_CcDhcpGlobals_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcIp.h"
#include "CcString.h"

namespace CcDhcpGlobals
{
  namespace Server
  {
    extern const CcDhcpSHARED CcIp DefaultServerIp;
    extern const CcDhcpSHARED CcIp DefaultIpBegin;
    extern const CcDhcpSHARED CcIp DefaultIpEnd;
    extern const CcDhcpSHARED CcIp DefaultSubnet;
    extern const CcDhcpSHARED uint32 DefaultLeaseTime;
    extern const CcDhcpSHARED uint32 DefaultRenewTime;
    extern const CcDhcpSHARED uint32 DefaultRebindTime;
    namespace Config
    {
      extern CcDhcpSHARED CcConstString_H(RootNode);
      extern CcDhcpSHARED CcConstString_H(IpBegin);
      extern CcDhcpSHARED CcConstString_H(IpEnd);
      extern CcDhcpSHARED CcConstString_H(Subnet);
      extern CcDhcpSHARED CcConstString_H(Gateway);
      extern CcDhcpSHARED CcConstString_H(Dns);
      extern CcDhcpSHARED CcConstString_H(Bootfile);
      extern CcDhcpSHARED CcConstString_H(BootfileEfi);
      extern CcDhcpSHARED CcConstString_H(BootfileMbr);
    }
  }

}

#endif // H_CcDhcpGlobals_H_
