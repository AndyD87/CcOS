/*
 * This file is part of CcRemoteDeviceDiscovery.
 *
 * CcRemoteDeviceDiscovery is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceDiscovery is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceDiscovery.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceDiscovery
 **/
#ifndef H_CcRemoteDeviceDiscovery_H_
#define H_CcRemoteDeviceDiscovery_H_

#include "CcRemoteDevice.h"
#include "CcList.h"
#include "CcDateTime.h"
#include "Network/CcSocket.h"
#include "Network/CcSocketAddressInfo.h"
#include "Network/CcCommonPorts.h"

#ifdef _MSC_VER
template class CcRemoteDeviceSHARED CcList<CcSocketAddressInfo>;
#endif

/**
 * @brief CcRemoteDeviceDiscovery implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceDiscovery : CcList<CcSocketAddressInfo>
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceDiscovery():
    m_oSocket(ESocketType::UDP)
  {}

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceDiscovery() = default;

  size_t findAllDevices(uint16 uiPort = CcCommonPorts::CcRemoteDevice, const CcDateTime& oWaitTime = CcDateTimeFromSeconds(5));
  bool bind();

private:
  CcSocket m_oSocket;
};

#endif // H_CcRemoteDeviceDiscovery_H_
