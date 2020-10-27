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
#include "CcObject.h"

void CcEvent::call(CcEventActionLoop* pSave, IEventBase *pEvent, void* pParam)
{
  CcEventAction oAction(pEvent, pParam);
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

CcEvent CcEvent::create(CcObject* pObject, CcObject::FObjectMethod pFunction)
{
  CCNEWTYPE(pEvent, IEventObject, pObject, pFunction);
  return CcEvent(pEvent);
}

CcEvent::IEventObject::IEventObject(CcObject* oObject, CcObject::FObjectMethod pFunc)
{
  m_pObject = oObject;
  m_pFunc = pFunc;
}

CcEvent::IEventObject::~IEventObject()
{
  m_pObject = nullptr;
  m_pFunc = nullptr;
}

void CcEvent::IEventObject::call(void* pParam)
{
  m_pObject->objectBaseCall(m_pFunc, pParam);
}

CcObject* CcEvent::IEventObject::getObject()
{
  return m_pObject;
}
