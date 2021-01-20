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
 * @brief     Class IWlan
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"

class IWlanClient;
class IWlanAccessPoint;

/**
 * @brief Interface of parent wlan devices wich can generate access points or clients.
 */
class CcKernelSHARED IWlan : public IDevice
{
public: // Types
  /**
   * @brief Capabilities of Wlan device
   */
  class CCapabilities
  {
  public:
    /**
     * @brief Create capabilities and optionaly set first flags.
     * @param uiCapabilities
     */
    CCapabilities(uint16 uiCapabilities = 0) : uiCapabilities(uiCapabilities){}

    //! return true if CCapabilities::AccesssPoint is set
    bool hasAccessPoint()
    { return (uiCapabilities & AccesssPoint) != 0; }

    //! return true if CCapabilities::Client is set
    bool hasClient()
    { return (uiCapabilities & Client) != 0; }

  public:
    uint16 uiCapabilities;  //!< Storage of capability flags

  public:
    static const uint16 AccesssPoint = 1; //!< Wlan device can connect create a Wlan network
    static const uint16 Client       = 2; //!< Wlan device can connect to other Wlan networks
  };

public:
  virtual ~IWlan() = default;

  /**
   * @brief If capability CCapabilities::AccesssPoint is set, Wlan Accesspoint device could
   *        be generated from Wlan device.
   * @return Generated access point device
   */
  virtual IWlanAccessPoint* getAccessPoint() = 0;

  /**
   * @brief If capability CCapabilities::Client is set, Wlan client device could
   *        be generated from Wlan device.
   * @return Generated client device
   */
  virtual IWlanClient* getClient() = 0;

  /**
   * @brief Get capabilities for this wlan device
   * @return Capability flags from CCapabilities
   */
  virtual CCapabilities getCapabilities() = 0;
};
