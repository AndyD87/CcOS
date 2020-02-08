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

  void registerMouseEvent(EEventType eType, CcEvent oNewCcEventHandle);
  inline void registerOnHover(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseHover, oNew); }
  inline void registerOnClick(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseLeftDown, oNew); }
  inline void registerOnLeave(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseLeave, oNew); }
  inline void registerOnDoubleClick(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseLeftDoubleClick, oNew); }
  inline void registerOnLeftDown(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseLeftDown, oNew); }
  inline void registerOnLeftUp(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseLeftUp, oNew); }
  inline void registerOnRightDown(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseRightDown, oNew); }
  inline void registerOnRightUp(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseRightUp, oNew); }
  inline void registerOnMiddleDown(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseMiddleDown, oNew); }
  inline void registerOnMiddleUp(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseMiddleUp, oNew); }
  inline void registerOnMove(CcEvent oNew)
    { registerMouseEvent(EEventType::MouseMove, oNew); }

  void removeObject(EEventType eType, CcObject* pObjectToRemove);

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};

#endif // H_CcMouseEventHandler_H_
