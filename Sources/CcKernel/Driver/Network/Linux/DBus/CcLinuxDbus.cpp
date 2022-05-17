/**
 * @copyright  Andreas Dirmeier (C) 2022
 *
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
 * @brief     Implementation of Class CcLinuxDbus
 */
#include "CcLinuxDbus.h"
#include "CcKernel.h"
#include "CcByteArray.h"
#include "Objects/CNetworkManager.h"

class CcLinuxDbus::CPrivate
{
public:
#ifdef NetworkManager_FOUND
  NLinuxDbus::CNetworkManager oNetworkManager;
#endif
};

CcLinuxDbus::CcLinuxDbus()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcLinuxDbus::~CcLinuxDbus()
{
  CCDELETE(m_pPrivate);
}

void CcLinuxDbus::init()
{
#ifdef NetworkManager_FOUND
  m_pPrivate->oNetworkManager.init();
#endif
}

void CcLinuxDbus::deinit()
{
#ifdef NetworkManager_FOUND
  m_pPrivate->oNetworkManager.deinit();
#endif
}
