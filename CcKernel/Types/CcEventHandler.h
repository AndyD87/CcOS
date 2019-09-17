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
 * @page      Types
 * @subpage   CcEventHandler
 *
 * @page      CcEventHandler
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEventHandler
 */
#ifndef H_CcEventHandler_H_
#define H_CcEventHandler_H_

//! Forward Declaration
#include "CcKernelBase.h"
#include "CcEvent.h"
#include "CcVector.h"
#include "CcObject.h"
#include "CcSharedPointer.h"
#include "CcEvent.h"
#include "CcMutex.h"

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
class CcKernelSHARED CcEventHandler : public CcObject, public CcVector<IEvent*>
{
public:
  virtual ~CcEventHandler()
  {
    while (size() > 0)
    {
      IEvent* pEvent = at(0);
      pEvent->getObject()->removeOnDelete(this);
      CCDELETE(pEvent);
      remove(0);
    }
  }

  void append(IEvent* pEventToAdd, bool bAppendOnDelete = true)
  {
    m_oLock.lock();
    CcVector<IEvent*>::append(pEventToAdd);
    if (bAppendOnDelete) pEventToAdd->getObject()->insertOnDelete(NewCcEvent(CcEventHandler, CcObject, CcEventHandler::removeObject, this));
    m_oLock.unlock();
  }

  void removeObject(CcObject* pObjectToRemove)
  {
    m_oLock.lock();
    for (size_t i = 0; i < size(); i++)
    {
      if (at(i)->getObject() == pObjectToRemove)
      {
        IEvent* pEvent = at(i);
        CCDELETE(pEvent);
        remove(i);
        i--;
      }
    }
    m_oLock.unlock();
  }

  void call(void *pParam)
  {
    for (size_t i = 0; i < size(); i++)
      at(i)->call(pParam);
  }

  bool call(CcObject* pTarget, void *pParam)
  {
    m_oLock.lock();
    for (size_t i = 0; i < size(); i++)
    {
      if (at(i)->getObject() == pTarget)
      {
        at(i)->call(pParam);
        return true;
      }
    }
    m_oLock.unlock();
    return false;
  }
private:
  CcMutex m_oLock;
};

#endif // H_CcEventHandler_H_
