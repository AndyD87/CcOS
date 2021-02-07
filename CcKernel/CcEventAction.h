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
 * @brief     Class CcEventAction
 */
#pragma once

#include "CcBase.h"
#include "CcEvent.h"
#include "CcMutex.h"
#include "CcReferenceCount.h"

/**
 * @brief Event action object wich can execute an Event with a specified
 *        Paramter.
 *
 *        This action can be used to transfer call events to other threads to enqueue it action loop.
 *
 *        @todo There might be a better way to implement events for thread safety.
 */
class CcKernelSHARED CcEventAction : public CcMutex
{
public:
  /**
   * @brief Event action with basic event and paramter to call.
   * @param oEvent:   Target event to call
   * @param pContext: Target parameter for call
   */
  CcEventAction(const CcEvent& oEvent = CcEvent(), void* pContext = nullptr);
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcEventAction)
  ~CcEventAction();

  /**
   * @brief Default compare operator. Set it always to false
   * @return always false
   */
  bool operator==(CcEventAction) { return false; }

  /**
   * @brief Copy content of another event action
   * @param oAction: Object to copy from
   */
  void operator=(const CcEventAction& oAction);

  /**
   * @brief Execute stored event call with stored context
   */
  void call();
public:
  bool    bLocked = false;  //!< Lock will be set on event call, and released if action finished.
  CcEvent oEvent;           //!< Target event to call
  void*   pContext;         //!< Target context to pass to call
};

/**
 * @brief Create Event action for event loops
 * @param CCOBJECTTYPE:     Basic object of sender
 * @param CCPARAMETERTYPE:  Paramter type of event method
 * @param CCMETHOD:         Target method to call
 * @param CCOBJECT:         Target oject to call
 * @param CONTEXT:          Context for additional informations
 */
#define CcEventAction_create(CCOBJECTTYPE,CCPARAMETERTYPE,CCMETHOD,CCOBJECT,CONTEXT) CcEventAction(CcEvent<CCOBJECTTYPE, CCPARAMETERTYPE>::create(CCOBJECT,&CCMETHOD),CONTEXT)
