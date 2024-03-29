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
 * @brief     Class CcIpInterface
 */
#pragma once

#include "Network/CcMacAddress.h"
#include "CcBase.h"
#include "CcIp.h"
#include "CcList.h"

class INetwork;
class CcDeviceNetwork;

/**
 * @brief Interface for ip addresses on network devices.
 */
class CcKernelSHARED CcIpInterface
{
public: // Methods
  CcIpInterface() = default;
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcIpInterface)

  /**
   * @brief Copy interface info from another interface.
   * @param oToCopy: Object to copy from
   * @return Handle to this.
   */
  CcIpInterface& operator=(const CcIpInterface& oToCopy);

  /**
   * @brief Move interface info from another interface.
   * @param oToMove: Object to move from
   * @return Handle to this.
   */
  CcIpInterface& operator=(CcIpInterface&& oToMove);

  /**
   * @brief Compare to interfaces if they are same
   * @param oToCompare: Interface to compare this to
   * @return True if all is even.
   */
  bool operator==(const CcIpInterface& oToCompare) const;

  /**
   * @brief Set subnet value by ip like 255.255.255.0 = /24
   * @param oSubnet: Subnet to parse for subnet
   */
  void setSubnet(const CcIp& oSubnet);

  /**
   * @brief Set subnet mask by defining number of leading 1
   * @param uiSubnetToSet: Number of leading 1 of an ip address.
   */
  void setSubnet(uint8 uiSubnetToSet)
  { uiSubnet = uiSubnetToSet; }

  //! @return Get device for this interface
  CcDeviceNetwork getDevice() const;

  //! @return Get device for this interface
  const CcIp& getIp() const
  { return oIpAddress; }

  //! @return Get device for this interface
  const CcIp& getGateway() const
  { return oGateway; }

  //! @return Current stored subnet as IP address like 255.255.255.0
  CcIp getSubnetIp();

  //! @return Get Subnet as /XX value
  uint32 getSubnet() const
  { return uiSubnet; }

  /**
   * @brief Set a number upper bits of a byte.
   * @param uiNumber: number of bits to set. A number larger than 7 will generate 0xff and waste time.
   * @return Generated byte
   */
  static uint8 setTopBits(uint8 uiNumber);

  /**
   * @brief Check if ip is matching interface ip and subnet.
   * @param oIp: Ip to verify
   * @return True if it is in interface range
   */
  bool isInSubnet(const CcIp& oIp);

public: // Types
  INetwork* pDevice    = nullptr; //!< Parent network device of this interface
  CcIp oIpAddress;                //!< IP of this interface
  uint8 uiSubnet=24;              //!< Subnet of this interface
  CcIp oGateway;                  //!< Default gateway if it is a valid address valid.
};
