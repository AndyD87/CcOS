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
 * @page      Network
 * @subpage   CcIpInterface
 *
 * @page      CcIpInterface
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIpInterface
 */
#ifndef H_CcIpInterface_H_
#define H_CcIpInterface_H_

#include "Network/CcMacAddress.h"
#include "CcBase.h"
#include "CcBase.h"
#include "CcIp.h"
#include "CcList.h"

class INetwork;

class CcKernelSHARED CcIpInterface
{
public: // Methods
  CcIpInterface() = default;
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcIpInterface);

  CcIpInterface& operator=(const CcIpInterface& oToCopy);
  CcIpInterface& operator=(CcIpInterface&& oToMove);

  bool operator==(const CcIpInterface& oToCompare) const;
  bool operator!=(const CcIpInterface& oToCompare) const
  { return !operator==(oToCompare); }

  /**
   * @brief Set subnet value by ip like 255.255.255.0 = /24
   * @param oSubnet: Subnet to parse for subnet
   */
  void setSubnet(const CcIp& oSubnet);
  void setSubnet(uint8 uiSubnetToSet)
    { uiSubnet = uiSubnetToSet; }
  CcIp getSubnetIp();
  static uint8 setTopBits(uint8 uiNumber);
  bool isInSubnet(const CcIp& oIp);

public: // Types
  INetwork* pInterface = nullptr;
  CcIp oIpAddress;
  uint8 uiSubnet=24;
  CcIp oGateway;
};

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcIpInterface>;
#endif
using CcIpInterfaceList = CcList<CcIpInterface>;

#endif //H_CcIpInterface_H_
