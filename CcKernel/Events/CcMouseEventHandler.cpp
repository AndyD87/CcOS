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

class CcMouseEventHandler::CPrivate
{
public:
  CcMap<EEventType, CcMap<CcObject*, IEvent*>> oEventMap;
  CcObject* m_pLastLeftButtonDown = nullptr;
  CcObject* m_pLastRightButtonDown = nullptr;
  CcObject* m_pLastMiddleButtonDown = nullptr;
  CcObject* m_pLastHovered = nullptr;
};

CcMouseEventHandler::CcMouseEventHandler()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcMouseEventHandler::~CcMouseEventHandler()
{
  CCDELETE(m_pPrivate);
}

bool CcMouseEventHandler::call(CcObject* pTarget, CcMouseEvent *pParam)
{
  bool bSuccess = false;
  switch (pParam->eType)
  {
    case EEventType::MouseLeftDown:
    {
      bSuccess = callExisting(pTarget, pParam);
      m_pPrivate->m_pLastLeftButtonDown = pTarget;
      break;
    }
    case EEventType::MouseLeftUp:
    {
      if (m_pPrivate->m_pLastLeftButtonDown != nullptr)
      {
        if (m_pPrivate->m_pLastLeftButtonDown == pTarget)
        {
          CcMouseEvent oMouseEvent(*pParam);
          oMouseEvent.eType = EEventType::MouseLeftDown;
          callExisting(pTarget, &oMouseEvent);
          m_pPrivate->m_pLastLeftButtonDown = nullptr;
        }
      }
      bSuccess = callExisting(pTarget, pParam);
      break;
    }
    case EEventType::MouseMove:
    {
      if (pTarget != m_pPrivate->m_pLastHovered)
      {
        if (pTarget != nullptr)
        {
          CcMouseEvent oMouseEvent(*pParam);
          oMouseEvent.eType = EEventType::MouseLeave;
          callExisting(m_pPrivate->m_pLastHovered, &oMouseEvent);
        }
        CcMouseEvent oMouseEvent(*pParam);
        oMouseEvent.eType = EEventType::MouseHover;
        callExisting(pTarget, &oMouseEvent);
        m_pPrivate->m_pLastHovered = pTarget;
      }
      bSuccess = callExisting(pTarget, pParam);
      break;
    }
    case EEventType::MouseLeave:
    {
      if (m_pPrivate->m_pLastHovered != nullptr)
      {
        bSuccess = callExisting(pTarget, pParam);
      }
      break;
    }
    case EEventType::MouseMiddleDown:
    case EEventType::MouseRightDown:
    case EEventType::MouseMiddleUp:
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
      CcMap<CcObject*, IEvent*>& oTargetMap = m_pPrivate->oEventMap.getValue(pParam->eType);
      if (oTargetMap.containsKey(pTarget))
      {
        oTargetMap.getValue(pTarget)->call(pParam);
      }
    }
  }
  return false;
}

void CcMouseEventHandler::registerMouseEvent(EEventType eType, IEvent* oNewCcEventHandle)
{
  if (m_pPrivate->oEventMap.containsKey(eType))
  {
    CcMap<CcObject*, IEvent*>& oTargetMap =  m_pPrivate->oEventMap.getValue(eType);
    oTargetMap.append(oNewCcEventHandle->getObject(), oNewCcEventHandle);
  }
  else
  {
    CcMap<CcObject*, IEvent*> oMap;
    oMap.append(oNewCcEventHandle->getObject(), oNewCcEventHandle);
    m_pPrivate->oEventMap.append(eType, oMap);
  }
}

void CcMouseEventHandler::removeObject(EEventType eType, CcObject* pObjectToRemove)
{
  for (size_t i = 0; i < m_pPrivate->oEventMap.size(); i++)
  {
    if (m_pPrivate->oEventMap.at(i).getKey() == eType)
    {
      CcMap<CcObject*, IEvent*>& oMap = m_pPrivate->oEventMap.at(i).value();
      for (size_t j = 0; j < oMap.size(); j++)
      {
        if (oMap.at(j).getKey() == pObjectToRemove)
        {
          CcObject*pObject = reinterpret_cast<CcObject*>(oMap.at(j).value()->getObject());
          CCDELETE(pObject);
          oMap.remove(j);
          j--;
        }
      }
    }
  }
}
