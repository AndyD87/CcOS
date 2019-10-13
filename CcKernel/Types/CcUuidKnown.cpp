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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcUuidKnown
 *            Possible generators: https://www.uuidgenerator.net/version4
 */
#include "CcUuidKnown.h"

namespace CcUuidKnown
{
  //! Known ids for CcOS and related projects
  namespace CcOS
  {
    const CcUuid CcVendor       (0xa87f5d15, 0x6a3c, 0x47a9, 0x8254, 0x826347189ced);
    const CcUuid CcRemoteDevice (0xb0142021, 0xc951, 0x4ccd, 0xb8c2, 0xb55adb1f3d98);
  }
}
