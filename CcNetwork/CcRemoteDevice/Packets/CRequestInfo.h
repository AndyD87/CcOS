/*
 * This file is part of CRequestInfo.
 *
 * CRequestInfo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CRequestInfo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CRequestInfo.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CRequestInfo
 **/
#ifndef H_CRequestInfo_H_
#define H_CRequestInfo_H_

#include "CPaket.h"

#pragma pack(push, 2)

namespace NRemoteDevice
{

class CRequestInfo : public CPaket
{
public:
  CRequestInfo() :
    CPaket(ECommand::Info, sizeof(*this))
  {}
};

}

#pragma pack(pop)

#endif // H_CRequestInfo_H_
