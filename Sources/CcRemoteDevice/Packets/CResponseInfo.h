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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CResponseInfo
 **/
#pragma once

#include "CPaket.h"

#pragma pack(push, 2)

namespace NRemoteDevice
{

/**
 * @brief Response info paket for Remote Device Com
 */
class CResponseInfo : public CPaket
{
public:
  CResponseInfo() :
    CPaket(ECommand::Info, sizeof(*this))
  {}
};

}
