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
 * @page      CcDhcp
 * @subpage   CcDhcpPacketData
 *
 * @page      CcDhcpPacketData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpPacketData
 **/
#ifndef CcDhcpPacketData_H_
#define CcDhcpPacketData_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcByteArray.h"

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpPacketData 
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpPacketData( void );

  /**
   * @brief Destructor
   */
  ~CcDhcpPacketData( void );

private:

public:
  uint8 op;
  uint8 htype;
  uint8 hlen;
  uint8 hops;
  uint32 xid;
  uint16 secs;
  uint16 flags;
  uint32 ciaddr;
  uint32 yiaddr;
  uint32 siaddr;
  uint32 giaddr;
  uint8 chaddr[16];
  char  sname[64];
  char  file[128];
  uint32  uiMagicCookie;
  uchar  options[312];
};
#endif /* CcDhcpPacketData_H_ */
