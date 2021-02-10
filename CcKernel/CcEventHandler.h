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
 * @brief     Class CcEventHandler
 */
#pragma once

// Forward Declaration
#include "CcBase.h"
#include "CcEvent.h"
#include "CcVector.h"
#include "CcObject.h"
#include "CcMutex.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<CcEvent>;
#endif

/**
 * @brief Event handler list.
 */
class CcKernelSHARED CcEventHandler : public CcObject
{
public:
  CcEventHandler()
  {}
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcEventHandler)
  virtual ~CcEventHandler();

  /**
   * @brief Clear this list and copy content of another event handler to this
   * @param oToCopy: Handler to copy from
   * @return Handle to this
   */
  CcEventHandler& operator=(const CcEventHandler& oToCopy);

  /**
   * @brief Append an event to this list.
   * @param rEvent: Event to copy
   * @return Handle to this
   */
  CcEventHandler& operator+=(const CcEvent& rEvent)
  { return CcEventHandler::append(rEvent); }

  /**
   * @brief Append an event to this list.
   * @param pEventToAdd: Event to copy
   * @param bAppendOnDelete: If true, append this event to auto delete on if target object is removed.
   * @return Handle to this
   */
  CcEventHandler& append(const CcEvent&  pEventToAdd, bool bAppendOnDelete = true);

  /**
   * @brief Remove all event related to an object from this list
   * @param pObjectToRemove: Object to remove from list
   */
  void removeObject(CcObject* pObjectToRemove);

  /**
   * @brief Call all stored events
   * @param pParam: Param to pass to all events.
   */
  void call(void *pParam);

  /**
   * @brief Call all events of specific objet.
   * @param pTarget:  Target object to search for and call
   * @param pParam:   Param to pass to all events.
   * @return
   */
  bool call(CcObject* pTarget, void *pParam);

  //! @return Current size of event list
  size_t size() const
  { return m_oEvents.size(); }
private:
  void removeObjectFromOnDelete(CcObject* pObjectToRemove);
private:
  CcMutex m_oLock;
  CcVector<CcEvent> m_oEvents;
};
