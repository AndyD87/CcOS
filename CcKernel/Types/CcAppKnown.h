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
 * @page      Types
 * @subpage   CcAppKnown
 *
 * @page      CcAppKnown
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcAppKnown
 */
#ifndef CcAppKnown_H_
#define CcAppKnown_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcUuid.h"

/**
 * @brief Default Class to create a Application
 */
namespace CcAppKnown
{
  static const CcString CcSyncServerName = "CcSyncServer";
  static const CcUuid   CcSyncServerUuid = CcUuid(0xbeb029a0, 0x5f58, 0x4821, 0xac, 0x5b, 0x98, 0x87, 0x77, 0x11, 0x54, 0x02);  // BEB029A0-5F58-4821-AC5B-988777115402
  static const CcString CcSyncClientName = "CcSyncClient";
  static const CcUuid   CcSyncClientUuid = CcUuid(0x8df741e3, 0xbb7d, 0x4030, 0x91, 0x9d, 0x4b, 0x2c, 0x83, 0x5d, 0x52, 0x48);  // 8DF741E3-BB7D-4030-919D-4B2C835D5248
  static const CcString CcSyncClientGuiName = "CcSyncClientGui";
  static const CcUuid   CcSyncClientGuiUuid = CcUuid(0x130598cb, 0x8622, 0x4069, 0x9c, 0xed, 0xba, 0x75, 0xd2, 0xcd, 0x29, 0x92);  // 130598CB-8622-4069-9CED-BA75D2CD2992
};

#endif /* CcAppKnown_H_ */
