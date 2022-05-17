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
 * @brief     Class CcMouseEventHandler
 **/
#pragma once

#include "CcBase.h"
#include "CcEventHandler.h"
#include "CcInputEvent.h"

/**
 * @brief Mouse event handler for easier handling of mouse events
 *        by enum values. On button press it will send global mouse event too.
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

  /**
   * @brief Call specific objects wit mouse events
   * @param pTarget:  Object to search for
   * @param pParam:   Paramter to pass to object
   * @return True if at least one event was called.
   */
  bool call(CcObject* pTarget, CcMouseEvent* pParam);

  /**
   * @brief Call specific objects with mouse events
   * @param pTarget:  Object to search for
   * @param pParam:   Paramter to pass to object
   * @return True if at least one event was called.
   */
  bool callExisting(CcObject* pTarget, CcMouseEvent* pParam);

  /**
   * @brief Register mouse event of specific type
   * @param eType:              Target type of event as enum
   * @param oNewCcEventHandle:  Target event to call an event
   */
  void registerMouseEvent(EEventType eType, const CcEvent& oNewCcEventHandle);
  //! @param oNew: Call this event on hover
  inline void registerOnHover(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseHover, oNew); }
  //! @param oNew: Call this event on click
  inline void registerOnClick(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseLeftDown, oNew); }
  //! @param oNew: Call this event on leave
  inline void registerOnLeave(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseLeave, oNew); }
  //! @param oNew: Call this event on double click
  inline void registerOnDoubleClick(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseLeftDoubleClick, oNew); }
  //! @param oNew: Call this event on left button down
  inline void registerOnLeftDown(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseLeftDown, oNew); }
  //! @param oNew: Call this event on left button up
  inline void registerOnLeftUp(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseLeftUp, oNew); }
  //! @param oNew: Call this event on right button down
  inline void registerOnRightDown(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseRightDown, oNew); }
  //! @param oNew: Call this event on right button up
  inline void registerOnRightUp(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseRightUp, oNew); }
  //! @param oNew: Call this event on middle button down
  inline void registerOnMiddleDown(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseMiddleDown, oNew); }
  //! @param oNew: Call this event on middle button up
  inline void registerOnMiddleUp(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseMiddleUp, oNew); }
  //! @param oNew: Call this event on mouse move
  inline void registerOnMove(const CcEvent& oNew)
  { registerMouseEvent(EEventType::MouseMove, oNew); }

  /**
   * @brief Remove events from an object for specific input types
   * @param eType:            Type of event to remove
   * @param pObjectToRemove:  Target object of event to remove
   */
  void removeObject(EEventType eType, CcObject* pObjectToRemove);

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};
