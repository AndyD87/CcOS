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
 * @subpage   CcEventHandleMap
 *
 * @page      CcEventHandleMap
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEventHandleMap
 */
#ifndef H_CcEventHandleMap_H_
#define H_CcEventHandleMap_H_

//! Forward Declaration
#include "CcKernelBase.h"
#include "CcEvent.h"
#include "CcEventHandler.h"
#include "CcMap.h"

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
template <typename TYPE>
class CcEventHandleMap : public CcMap<TYPE, IEvent*>
{
public:
  CcEventHandleMap() = default;

  ~CcEventHandleMap()
  {
    clear();
  }

  void call(const TYPE& oType, void* pParam)
  {
    for (size_t uIndex=0; uIndex<CcMap<TYPE, IEvent*>::size(); uIndex++)
    {
      if (this->at(uIndex).getKey() == oType)
      {
        this->at(uIndex).value()->call(pParam);
      }
    }
  }

  void removeObject(const TYPE& oType, CcObject* pObject)
  {
    for (size_t uIndex = 0; uIndex<CcMap<TYPE, IEvent*>::size(); uIndex++)
    {
      if (this->at(uIndex).getKey() == oType)
      {
        if (pObject == this->at(uIndex).getValue()->getObject())
        {
          delete pObject;
          CcMap<TYPE, IEvent*>::remove(uIndex);
          uIndex--;
        }
      }
    }
  }

  void clear()
  {
    for (CcPair<TYPE, IEvent*>& oPair : *this)
    {
      CCDELETE(oPair.value());
    }
    CcMap::clear();
  }
};

#endif // H_CcEventHandleMap_H_
