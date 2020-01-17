/*
 * This file is part of CResponseInfo.
 *
 * CResponseInfo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CResponseInfo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CResponseInfo.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CResponseInfo
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CResponseInfo
 **/
#ifndef H_CResponseInfo_H_
#define H_CResponseInfo_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcVersion.h"
#include "CcString.h"
#include "CcUuid.h"

namespace NRemoteDeviceServerConfig
{

class CResponseInfo
{
public:
  class CInterface
  {
  public:
    uint32 uiId;
    uint16 uiFlags;
    uint16 uiPort;

    const static uint16 SecureFlag;
  };

  uint16    uiPacketId;         //!< Packet id for response to
  uint16    uiVersion;          //!< PacketVersion
  CcUuid    oProductId;         //!< Uuid of current running product
  CcVersion oProductVersion;    //!< 64bit version number of current product
  uint16    uiIpOffset;
  uint16    uiIpLength;
  uint16    uiNameOffset;
  uint16    uiNameLength;
  uint16    uiInterfacesOffset;
  uint16    uiInterfacesLength;
  uint8     pData[1];
};

}

#endif // H_CResponseInfo_H_
