/*
 *
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
 * @page      Linux
 * @subpage   CNetworkManager
 *
 * @page      CNetworkManager
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CNetworkManager
 */

#ifndef H_CNetworkManager_H_
#define H_CNetworkManager_H_

#include "../ILinuxDbus.h"

class CcStringList;

namespace NLinuxDbus
{
class CNetworkManager : public ILinuxDbus
{
public:
  enum class EDeviceType
  {
    NM_DEVICE_TYPE_UNKNOWN          = 0,
    NM_DEVICE_TYPE_ETHERNET         = 1,
    NM_DEVICE_TYPE_WIFI             = 2,
    NM_DEVICE_TYPE_UNUSED1          = 3,
    NM_DEVICE_TYPE_UNUSED2          = 4,
    NM_DEVICE_TYPE_BT               = 5,
    NM_DEVICE_TYPE_OLPC_MESH        = 6,
    NM_DEVICE_TYPE_WIMAX            = 7,
    NM_DEVICE_TYPE_MODEM            = 8,
    NM_DEVICE_TYPE_INFINIBAND       = 9,
    NM_DEVICE_TYPE_BOND             = 10,
    NM_DEVICE_TYPE_VLAN             = 11,
    NM_DEVICE_TYPE_ADSL             = 12,
    NM_DEVICE_TYPE_BRIDGE           = 13,
    NM_DEVICE_TYPE_GENERIC          = 14,
    NM_DEVICE_TYPE_TEAM             = 15,
    NM_DEVICE_TYPE_TUN              = 16,
    NM_DEVICE_TYPE_IP_TUNNEL        = 17,
    NM_DEVICE_TYPE_MACVLAN          = 18,
    NM_DEVICE_TYPE_VXLAN            = 19,
    NM_DEVICE_TYPE_VETH             = 20,
    NM_DEVICE_TYPE_MACSEC           = 21,
    NM_DEVICE_TYPE_DUMMY            = 22,
    NM_DEVICE_TYPE_PPP              = 23,
    NM_DEVICE_TYPE_OVS_INTERFACE    = 24,
    NM_DEVICE_TYPE_OVS_PORT         = 25,
    NM_DEVICE_TYPE_OVS_BRIDGE       = 26,
    NM_DEVICE_TYPE_WPAN             = 27,
    NM_DEVICE_TYPE_6LOWPAN          = 28,
    NM_DEVICE_TYPE_WIREGUARD        = 29,
    NM_DEVICE_TYPE_WIFI_P2P         = 30,
    NM_DEVICE_TYPE_VRF              = 31
  };
  CNetworkManager();
  virtual ~CNetworkManager();

  void init();
  void deinit();

  CcStringList getDevices();
  EDeviceType getDeviceType(const CcString& sDevice);
  CcStringList getWifiAccessPoints(const CcString& sDevice);


private:
  class CPrivate;
  CPrivate* m_pPrivate;
};
}
#endif // H_CNetworkManager_H_
