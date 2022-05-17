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
 * @brief     Class CcCommonPorts
 */
#pragma once

#include "CcBase.h"

namespace CcCommonPorts
{
  static const uint16 FTP             =    21;  //!< Default FTP port
  static const uint16 SSH             =    22;  //!< Default SSH port
  static const uint16 TELNET          =    23;  //!< Default telnet port
  static const uint16 DHCP_SRV        =    67;  //!< Default DHCP Server port
  static const uint16 DHCP_CLI        =    68;  //!< Default DHCP Client port
  static const uint16 TFTP            =    69;  //!< Default TFTP port
  static const uint16 HTTP            =    80;  //!< Default HTTP port
  static const uint16 HTTPS           =   443;  //!< Default HTTPS port
  static const uint16 MQTT            =  1883;  //!< Default MQTT port
  static const uint16 BITCOIN_RPC     =  8332;  //!< Default MQTTS port
  static const uint16 BITCOIN_CHAIN   =  8333;  //!< Default MQTTS port
  static const uint16 MQTT_SSL        =  8883;  //!< Default MQTTS port

  static const uint16 CcOSOffset      = 27500;  //!< This offset for all CcOS default used ports
  static const uint16 CcSync          = 27500;  //!< First used port of CcOS is for CcSync
  static const uint16 CcRemoteDevice  = 27510;  //!< Default port for CcRemoteDevice requests
  static const uint16 CcRemoteDeviceIfc  = 27511;  //!< Default port for CcRemoteDevice interface at ip 0.0.0.0
  static const uint16 YobitTrader     = 27520;  //!< Yobit trader application interface

  static const uint16 CcTestBase      = 10000;  //!< Test port base offset
  static const uint16 CcSourceBase    = 20000;  //!< CcSource base offset

  static const uint16 InvalidPort     = UINT16_MAX; //!< Invalid port value
}
