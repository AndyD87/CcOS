/*
 * This file is part of CcRemoteDeviceConnector.
 *
 * CcRemoteDeviceConnector is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceConnector is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceConnector.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceConnector
 **/
#pragma once

#include "CcRemoteDevice.h"
#include "CcList.h"
#include "CcDateTime.h"
#include "Network/CcSocket.h"
#include "Network/CcSocketAddressInfo.h"
#include "Network/CcCommonPorts.h"

/**
 * @brief CcRemoteDeviceConnector implementation
 *        Internal connection to a running remote device server session.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceConnector
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceConnector();
  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceConnector() = default;

private:
  CcSocket            m_oSocket;
  CcSocketAddressInfo m_oSocketAddressInfo;
};
