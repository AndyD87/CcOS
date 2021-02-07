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
 * @brief     Class CcDhcpPacketData
 **/
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "CcByteArray.h"

/**
 * @brief Control openssl library
 */
class CcKernelSHARED CcDhcpPacketData 
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpPacketData();

  /**
   * @brief Destructor
   */
  ~CcDhcpPacketData();

private:

public:
  uint8 op;               //!< OP Code
  uint8 htype;            //!< Hardware Type
  uint8 hlen;             //!< Hardware Length
  uint8 hops;             //!< Hops
  uint32 xid;             //!< Tranaction Id
  uint16 secs;            //!< Seconds
  uint16 flags;           //!< Additional Flags
  uint32 ciaddr;          //!< Client IP
  uint32 yiaddr;          //!< Your IP
  uint32 siaddr;          //!< Server IP
  uint32 giaddr;          //!< Gateway IP
  uint8 chaddr[16];       //!< Client Hardware IP Address
  char  sname[64];        //!< Servername
  char  file[128];        //!< Bootfile
  uint32  uiMagicCookie;  //!< Magic cookie for verification
  uchar  options[312];    //!< Options
};
