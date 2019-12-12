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
 * @page      CcRemoteDeviceDiscovery
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceDiscovery
 **/
#ifndef H_CcRemoteDeviceDiscovery_H_
#define H_CcRemoteDeviceDiscovery_H_

#include "CcRemoteDevice.h"

/**
 * @brief CcRemoteDeviceDiscovery implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceDiscovery
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceDiscovery() = default;

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceDiscovery() = default;

  void findAllDevices();
};

#endif // H_CcRemoteDeviceDiscovery_H_
