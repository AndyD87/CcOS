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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEventActionLoop
 */
#pragma once

//! Forward Declaration
#include "CcBase.h"
#include "CcMutex.h"
#include "CcEventActionList.h"

class CcEventAction;

/**
 * @brief Loop of events to do
 *        The might be enqueued from other Threads or input events.
 */
class CcKernelSHARED CcEventActionLoop : CcMutex
{
public:
  /**
   * @brief Loop until onLoop failes
   */
  void loop();

  /**
   * @brief Add event to event loop
   * @param pAction: Target event action to call on next loop.
   */
  void appendAction(CcEventAction* pAction);

  /**
   * @brief Loop to overload from inerheriting class.
   *        This method will be called after every called event action.
   *
   *        The inheriting class can run idle processes here, but it should not execute long runs.
   * @return If return false, loop will be canceled and closed.
   */
  virtual bool onLoop() = 0;
  
private:
  CcEventActionList m_oEvents;
};
