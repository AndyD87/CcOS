/*
 * This file is part of CResponseDiscover.
 *
 * CResponseDiscover is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CResponseDiscover is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CResponseDiscover.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CResponseDiscover
 **/
#pragma once

#include "CPaket.h"

#pragma pack(push, 2)

namespace NRemoteDevice
{
/**
 * @brief Object to create or read a discover response message.
 */
class CResponseDiscover
{
public:
  /**
   * @brief Define interface list item in response storage.
   */
  class CInterface
  {
  public:
    uint32 uiId;    //!< Interface-Guid
    uint16 uiFlags; //!< Additional flags
    uint16 uiPort;  //!< Port of Interface if available

    const static uint16 SecureFlag; //!< Security flags for ssl connections
  };

  uint16    uiPacketId;         //!< Packet id for response to
  uint16    uiVersion;          //!< PacketVersion
  CcUuid    oProductId;         //!< Uuid of current running product
  CcVersion oProductVersion;    //!< 64bit version number of current product
  //! Union of Ipv4 and Ipv6, check for @ref uiFlags for right one
  union _uIp
  {
    uint32     uiV4;            //!< Ipv4 value
    uint16    puiV6[8];         //!< Ipv6 array with eight 16bit unsigned integer
  } uIp;                        //!< Instance of _uIp
  uint32    uiFlags;            //!< Flags of @ref NRemoteDevice_CResponseDiscover_Flags
  uint16    uiNameOffset;       //!< Position of System name in @ref pData
  uint16    uiNameLength;       //!< Length of System name in @ref pData
  uint16    uiInterfacesOffset; //!< Position of first Interface in @ref pData
  uint16    uiInterfacesLength; //!< Length of Buffer for @ref uiInterfacesOffset
  uint8     pData[1];           //!< Start of Data


  //! @addtogroup NRemoteDevice_CResponseDiscover_Flags
  //! @{
  static const uint32 uiFlagIpv6 = 0x1; //!< if this flag is available in @ref uiFlags,
                                        //!< than @ref _uIp.puiV6 is valid, otherwise @ref _uIp.uiV4
  //! @}
};

}

#pragma pack(pop)
