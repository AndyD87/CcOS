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
#include "CcBase.h"
#include "CcEvent.h"
#include "CcVector.h"
#include "CcObject.h"
#include "CcMutex.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<CcEvent>;
#endif

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
class CcKernelSHARED CcEventHandler : public CcObject
{
public:
  CcEventHandler()
  {}
  CcEventHandler(const CcEventHandler& oToCopy) :
    CcObject(oToCopy)
  { operator=(oToCopy); }
  virtual ~CcEventHandler();

  CcEventHandler& operator=(const CcEventHandler& oToCopy);
  CcEventHandler& operator+=(const CcEvent& rEvent)
  { return CcEventHandler::append(rEvent); }

  CcEventHandler& append(const CcEvent&  pEventToAdd, bool bAppendOnDelete = true);
  void removeObject(CcObject* pObjectToRemove);
  void call(void *pParam);
  bool call(CcObject* pTarget, void *pParam);
  size_t size() const
  { return m_oEvents.size(); }
private:
  void removeObjectFromOnDelete(CcObject* pObjectToRemove);
private:
  CcMutex m_oLock;
  CcVector<CcEvent> m_oEvents;
};

#endif // H_CcEventHandler_H_
