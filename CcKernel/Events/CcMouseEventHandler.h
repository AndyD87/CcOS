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
 * @page      Events
 * @subpage   CcMouseEventHandler
 *
 * @page      CcMouseEventHandler
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMouseEventHandler
 **/
#ifndef H_CcMouseEventHandler_H_
#define H_CcMouseEventHandler_H_

#include "CcBase.h"
#include "CcEventHandler.h"
#include "CcInputEvent.h"

/**
 * @brief Class implementation
 */
class CcKernelSHARED CcMouseEventHandler
{
public:
  /**
   * @brief Constructor
   */
  CcMouseEventHandler();

  /**
   * @brief Destructor
   */
  ~CcMouseEventHandler();

  bool call(CcObject* pTarget, CcMouseEvent* pParam);
  bool callExisting(CcObject* pTarget, CcMouseEvent* pParam);

  void registerMouseEvent(EMouseEventType eType, IEvent* oNewCcEventHandle);
  inline void registerOnHover(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::Hover, oNew); }
  inline void registerOnClick(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::ClickLeft, oNew); }
  inline void registerOnLeave(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::Leave, oNew); }
  inline void registerOnDoubleClick(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::ClickDoubleLeft, oNew); }
  inline void registerOnLeftDown(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::LeftDown, oNew); }
  inline void registerOnLeftUp(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::LeftUp, oNew); }
  inline void registerOnRightDown(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::RightDown, oNew); }
  inline void registerOnRightUp(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::RightUp, oNew); }
  inline void registerOnMiddleDown(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::MiddleDown, oNew); }
  inline void registerOnMiddleUp(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::MiddleUp, oNew); }
  inline void registerOnMove(IEvent* oNew)
    { registerMouseEvent(EMouseEventType::Move, oNew); }

  void removeObject(EMouseEventType eType, CcObject* pObjectToRemove);

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};

#endif /* H_CcMouseEventHandler_H_ */
