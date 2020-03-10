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
 * @subpage   CcEventAction
 *
 * @page      CcEventAction
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEventAction
 */
#ifndef H_CcEventAction_H_
#define H_CcEventAction_H_

//! Forward Declaration
#include "CcBase.h"
#include "CcEvent.h"
#include "CcMutex.h"
#include "CcReferenceCount.h"

class CcKernelSHARED CcEventAction : public CcMutex
{
public:
  CcEventAction(const CcEvent& oEvent = CcEvent(), void* pContext = nullptr);
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcEventAction)
  ~CcEventAction();

  CCDEFINE_EQUAL_OPERATORS(CcEventAction)
  void operator=(const CcEventAction& oAction);

  void call();
public:
  bool    bLocked = false;
  CcEvent oEvent;
  void*   pContext;
};

#define CcEventAction_create(CCOBJECTTYPE,CCPARAMETERTYPE,CCMETHOD,CCOBJECT,CONTEXT) CcEventAction(CcEvent<CCOBJECTTYPE, CCPARAMETERTYPE>::create(CCOBJECT,&CCMETHOD),CONTEXT)


#endif // H_CcEventAction_H_
