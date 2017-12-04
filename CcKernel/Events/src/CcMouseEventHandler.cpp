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
 * @brief     Implemtation of class CcMouseEventHandler
 */
#include "Events/CcMouseEventHandler.h"
#include "CcInputEvent.h"
#include "CcMap.h"
#include "CcEventHandler.h"

class CcMouseEventHandlerPrivate
{
public:
  CcMap<EMouseEventType, CcMap<CcObject*, CcEventHandle>> oEventMap;
  CcObject* m_pLastLeftButtonDown = nullptr;
  CcObject* m_pLastRightButtonDown = nullptr;
  CcObject* m_pLastMiddleButtonDown = nullptr;
  CcObject* m_pLastHovered = nullptr;
};

CcMouseEventHandler::CcMouseEventHandler( void )
{
  m_pPrivate = new CcMouseEventHandlerPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcMouseEventHandler::~CcMouseEventHandler( void )
{
  CCDELETE(m_pPrivate);
}

bool CcMouseEventHandler::call(CcObject* pTarget, CcMouseEvent *pParam)
{
  bool bSuccess = false;
  switch (pParam->eType)
  {
    case EMouseEventType::LeftDown:
    {
      bSuccess = callExisting(pTarget, pParam);
      m_pPrivate->m_pLastLeftButtonDown = pTarget;
      break;
    }
    case EMouseEventType::LeftUp:
    {
      if (m_pPrivate->m_pLastLeftButtonDown != nullptr)
      {
        if (m_pPrivate->m_pLastLeftButtonDown == pTarget)
        {
          CcMouseEvent oMouseEvent(*pParam);
          oMouseEvent.eType = EMouseEventType::ClickLeft;
          callExisting(pTarget, &oMouseEvent);
          m_pPrivate->m_pLastLeftButtonDown = nullptr;
        }
      }
      bSuccess = callExisting(pTarget, pParam);
    }
    case EMouseEventType::Move:
    {
      if (pTarget != m_pPrivate->m_pLastHovered)
      {
        if (pTarget != nullptr)
        {
          CcMouseEvent oMouseEvent(*pParam);
          oMouseEvent.eType = EMouseEventType::Leave;
          callExisting(m_pPrivate->m_pLastHovered, &oMouseEvent);
        }
        CcMouseEvent oMouseEvent(*pParam);
        oMouseEvent.eType = EMouseEventType::Hover;
        callExisting(pTarget, &oMouseEvent);
        m_pPrivate->m_pLastHovered = pTarget;
      }
      bSuccess = callExisting(pTarget, pParam);
      break;
    }
    case EMouseEventType::Leave:
    {
      if (m_pPrivate->m_pLastHovered != nullptr)
      {
        bSuccess = callExisting(pTarget, pParam);
      }
      break;
    }
    case EMouseEventType::MiddleDown:
    case EMouseEventType::RightDown:
    case EMouseEventType::MiddleUp:
    default:
      break;
  }
  return bSuccess;
}

bool CcMouseEventHandler::callExisting(CcObject* pTarget, CcMouseEvent *pParam)
{
  if (pTarget != nullptr)
  {
    if (m_pPrivate->oEventMap.containsKey(pParam->eType))
    {
      CcMap<CcObject*, CcEventHandle>& oTargetMap = m_pPrivate->oEventMap.getValue(pParam->eType);
      if (oTargetMap.containsKey(pTarget))
      {
        oTargetMap.getValue(pTarget)->call(pParam);
      }
    }
  }
  return false;
}

void CcMouseEventHandler::registerMouseEvent(EMouseEventType eType, CcEventHandle oNewEventHandle)
{
  if (m_pPrivate->oEventMap.containsKey(eType))
  {
    CcMap<CcObject*, CcEventHandle>& oTargetMap =  m_pPrivate->oEventMap.getValue(eType);
    oTargetMap.append(oNewEventHandle->getObject(), oNewEventHandle);
  }
  else
  {
    CcMap<CcObject*, CcEventHandle> oMap;
    oMap.append(oNewEventHandle->getObject(), oNewEventHandle);
    m_pPrivate->oEventMap.append(eType, oMap);
  }
}