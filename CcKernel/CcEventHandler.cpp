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
 * @brief     Implemtation of class CcEventHandler
 */
#include "CcEventHandler.h"

CcEventHandler::~CcEventHandler()
{
  for(CcEvent& rEvent : m_oEvents)
  {
    rEvent.getObject()->deregisterOnDelete(this);
  }
}

CcEventHandler& CcEventHandler::operator=(const CcEventHandler& rEvent)
{
  CcObject::operator=(rEvent);
  m_oLock.lock();
  m_oEvents = rEvent.m_oEvents;
  m_oLock.unlock();
  return *this;
}

CcEventHandler& CcEventHandler::append(const CcEvent& pEventToAdd, bool bAppendOnDelete)
{
  m_oLock.lock();
  m_oEvents.append(pEventToAdd);
  if (bAppendOnDelete)
    m_oEvents.last().getObject()->registerOnDelete(
          NewCcEventType(CcEventHandler,
            CcObject,
            CcEventHandler::removeObjectFromOnDelete,
            this
                     )
          );
  m_oLock.unlock();
  return *this;
}

void CcEventHandler::removeObject(CcObject* pObjectToRemove)
{
  m_oLock.lock();
  for (size_t i = 0; i < size(); i++)
  {
    if (m_oEvents[i].getObject() == pObjectToRemove)
    {
      m_oEvents[i].getObject()->deregisterOnDelete(this);
      m_oEvents.remove(i);
      i--;
    }
  }
  m_oLock.unlock();
}

void CcEventHandler::call(void *pParam)
{
  m_oLock.lock();
  for(CcEvent& rEvent : m_oEvents)
  {
    rEvent.call(pParam);
  }
  m_oLock.unlock();
}

bool CcEventHandler::call(CcObject* pTarget, void *pParam)
{
  m_oLock.lock();
  for(CcEvent& rEvent : m_oEvents)
  {
    if (rEvent.getObject() == pTarget)
    {
      rEvent.call(pParam);
    }
  }
  m_oLock.unlock();
  return false;
}

void CcEventHandler::removeObjectFromOnDelete(CcObject *pObjectToRemove)
{
  m_oLock.lock();
  for(size_t i = 0; i < size(); i++)
  {
    if (m_oEvents[i].getObject() == pObjectToRemove)
    {
      m_oEvents.remove(i);
      i--;
    }
  }
  m_oLock.unlock();
}
