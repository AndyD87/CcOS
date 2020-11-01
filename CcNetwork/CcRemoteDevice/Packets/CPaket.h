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
 * @page      CPaket
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CPaket
 **/
#ifndef H_CPaket_H_
#define H_CPaket_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"

#pragma pack(push, 2)

namespace NRemoteDeviceServer
{

enum class ECommand : uint16
{
  Discover = 0,
  Info
};

class CPaket
{
public:
  CPaket(ECommand eCommand, uint16 uiSize) :
    uiCommand(static_cast<uint16>(uiCommand)),
    uiSize(uiSize)
  {}

  ECommand getCommand()
  { return static_cast<ECommand>(uiCommand); }

  uint16 uiCommand;
  uint16 uiSize;
};

}

#pragma pack(pop)

#endif // H_CPaket_H_
