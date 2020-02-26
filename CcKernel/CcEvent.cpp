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
 * @brief     Implemtation of class CcEvent
 */
#include "CcEvent.h"
#include "CcEventAction.h"
#include "CcEventActionLoop.h"
#include "CcKernel.h"

void CcEvent::call(CcEventActionLoop* pSave, CcEvent oEvent, void* pParam)
{
  CcEventAction oAction(oEvent, pParam);
  oAction.bLocked = true;
  pSave->appendAction(&oAction);
  while (oAction.bLocked == true)
    CcKernel::delayMs(1);
}

void CcEvent::clear()
{
  CCDELETEREF(m_pEvent);
}

CcEvent& CcEvent::operator=(const CcEvent& rEvent)
{
  clear();
  m_pEvent = rEvent.m_pEvent;
  m_pEvent->referenceCountIncrement();
  return *this;
}

CcEvent& CcEvent::operator=(CcEvent&& rEvent)
{
  clear();
  m_pEvent = rEvent.m_pEvent;
  rEvent.m_pEvent = nullptr;
  return *this;
}
