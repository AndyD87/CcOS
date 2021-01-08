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
 * @brief     Implementation of class CcCommonIps
 */
#include "../Network/CcCommonIps.h"

namespace CcCommonIps
{
  const CcIp Broadcast(0xff, 0xff, 0xff, 0xff);
  const CcIp AnyAddress(0,0,0,0);
  const CcIp Localhost(127, 0, 0, 1);
  const CcIp MulticastBegin(224,0,0,0);
  const CcIp MulticastEnd(240,0,0,0);
  const CcIp MulticastPtp(224,0,0,107);
  const CcIp MulticastNtp(224,0,1,1);
  const CcIp MulticastCcRemoteDevice(230,27,5,87);
}
