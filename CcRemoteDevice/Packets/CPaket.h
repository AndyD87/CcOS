/*
 * This file is part of CPaket.
 *
 * CPaket is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CPaket is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CPaket.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CPaket
 **/
#pragma once

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"

#pragma pack(push, 2)

namespace NRemoteDevice
{
/**
 * @brief Command type definitions for remote packets
 */
enum class ECommand : uint16
{
  Discover = 0, //!< Disovery packet for broadcast
  Info          //!< Information packet for remote devices
};

/**
 * @brief Basic packet type for remote devices.
 */
class CPaket
{
public:
  /**
   * @brief Setup basic packet by type and size
   * @param eCommand: Command to init with.
   * @param uiSize:   Size of packet to allocate, inclusiv eCommand.
   */
  CPaket(ECommand eCommand, uint16 uiSize) :
    m_uiCommand(static_cast<uint16>(eCommand)),
    m_uiSize(uiSize)
  {}

  //! @return Get current command as enum
  ECommand getCommand()
  { return static_cast<ECommand>(m_uiCommand); }
  //! @return Get current size of packet
  uint16 getSize()
  { return m_uiSize; }

private:
  uint16 m_uiCommand;
  uint16 m_uiSize;
};

}

#pragma pack(pop)
