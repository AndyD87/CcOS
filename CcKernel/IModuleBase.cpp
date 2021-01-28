/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the free Software Foundation, either version 3 of the License, or
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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Redirect new and delete to main application.
 *            This file must be included once on every Module
 */
#include "CcKernel.h"
#include "IModuleBase.h"
#include "IKernel.h"
#include <cstdlib>

void IModuleBase::registerOnUnload(const CcEvent& oUnloadEvent)
{
  m_oUnloadEvent.append(oUnloadEvent);
}

void IModuleBase::deregisterOnUnload(CcObject* pUnregister)
{
  m_oUnloadEvent.removeObject(pUnregister);
}
