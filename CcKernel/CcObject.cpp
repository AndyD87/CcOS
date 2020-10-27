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
 * @brief     Class CcObject
 */
#include "CcObject.h"
#include "CcEvent.h"
#include "CcEventHandler.h"

CcObject::~CcObject()
{
  clear();
}

void CcObject::insertOnDelete(const CcEvent& pEventHandle)
{
  getOnDeleteHandler().append(pEventHandle, false);
}

void CcObject::removeOnDelete(CcObject* pObject)
{
  if (m_pOnDeleteHandler != nullptr)
  {
    m_pOnDeleteHandler->removeObject(pObject);
  }
}

CcEventHandler& CcObject::getOnDeleteHandler()
{
  if(m_pOnDeleteHandler == nullptr)
  {
    CCNEW(m_pOnDeleteHandler,CcEventHandler);
  }
  return *m_pOnDeleteHandler;
}

void CcObject::clear()
{
  if (m_pOnDeleteHandler != nullptr)
  {
    m_pOnDeleteHandler->call(this);
    CCDELETE(m_pOnDeleteHandler);
  }
}
