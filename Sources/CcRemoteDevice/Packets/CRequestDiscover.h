/*
 * This file is part of CRequestDiscover.
 *
 * CRequestDiscover is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CRequestDiscover is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CRequestDiscover.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CRequestDiscover
 **/
#pragma once

#include "CPaket.h"

#pragma pack(push, 2)

namespace NRemoteDevice
{

/**
 * @brief Discovery request paket for finding remote devices by broadcast
 */
class CRequestDiscover : public CPaket
{
public:
  /**
   * @brief Create empty paket
   */
  CRequestDiscover() :
    CPaket(ECommand::Discover, sizeof(*this))
  {}
};

}

#pragma pack(pop)