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
 * @brief     Class CcAppKnown
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcUuid.h"

/**
 * @brief Known Applications with their names and guid
 */
namespace CcAppKnown
{
  //! Sync server name
  static const CcString CcSyncServerName ("CcSyncServer");
  //! Sync server uuid
  //! BEB029A0-5F58-4821-AC5B-988777115402
  static const CcUuid   CcSyncServerUuid (0xbeb029a0, 0x5f58, 0x4821, 0xac, 0x5b, 0x98, 0x87, 0x77, 0x11, 0x54, 0x02);

  //! Sync client name
  static const CcString CcSyncClientName ("CcSyncClient");
  //! Sync client uuid
  //! 8DF741E3-BB7D-4030-919D-4B2C835D5248
  static const CcUuid   CcSyncClientUuid (0x8df741e3, 0xbb7d, 0x4030, 0x91, 0x9d, 0x4b, 0x2c, 0x83, 0x5d, 0x52, 0x48);

  //! Sync client gui name
  static const CcString CcSyncClientGuiName ("CcSyncClientGui");
  //! Sync client gui uuid
  //! 130598CB-8622-4069-9CED-BA75D2CD2992
  static const CcUuid   CcSyncClientGuiUuid (0x130598cb, 0x8622, 0x4069, 0x9c, 0xed, 0xba, 0x75, 0xd2, 0xcd, 0x29, 0x92);
};