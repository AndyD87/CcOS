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
#ifndef _CcEventHandler_H_
#define _CcEventHandler_H_

//! Forward Declaration
#include "CcKernelBase.h"
#include "CcEvent.h"
#include "CcVector.h"
#include "CcObject.h"
#include "CcSharedPointer.h"
#include "CcEvent.h"

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
    CcVector<IEvent*>::append(pEventToAdd);
    if(bAppendOnDelete) pEventToAdd->getObject()->insertOnDelete(NewCcEvent(CcEventHandler, CcObject, CcEventHandler::removeObject, this));
  }

  void removeObject(CcObject* pObjectToRemove)
  {
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
  }

  void call(void *pParam)
  {
    for (size_t i = 0; i < size(); i++)
      at(i)->call(pParam);
  }

  bool call(CcObject* pTarget, void *pParam)
  {
    for (size_t i = 0; i < size(); i++)
    {
      if (at(i)->getObject() == pTarget)
      {
        at(i)->call(pParam);
        return true;
      }
    }
    return false;
  }
};

#endif /* _CcEventHandler_H_ */
