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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcEventAction
 */
#include "CcEventAction.h"

CcEventAction::CcEventAction(IEvent* pEvent, void* pContext) :
  pEvent(pEvent),
  pContext(pContext)
{
  pReferenceCnt = new int;
  *pReferenceCnt = 1;
}

CcEventAction::~CcEventAction()
{
  if (*pReferenceCnt <= 1)
  {
    CCDELETE(pEvent);
    CCDELETE(pReferenceCnt);
  }
  else
  {
    (*pReferenceCnt)--;
  }
}

void CcEventAction::call()
{
  if (pEvent) pEvent->call(pContext);
}

void CcEventAction::operator=(const CcEventAction& oAction)
{
  pEvent = oAction.pEvent;
  pContext = oAction.pContext;
  pReferenceCnt = oAction.pReferenceCnt;
  (*pReferenceCnt)++;
}

