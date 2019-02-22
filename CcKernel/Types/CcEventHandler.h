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
 * @subpage   CcEventHandler
 *
 * @page      CcEventHandler
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEventHandler
 */
#ifndef _CcEventHandler_H_
#define _CcEventHandler_H_

//! Forward Declaration
#include "CcKernelBase.h"
#include "CcEvent.h"
#include "CcList.h"
#include "CcObject.h"
#include "CcSharedPointer.h"

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
class CcKernelSHARED CcEventHandler : public CcList<CcEventHandle>
{
public:
  CcEventHandler() = default;

  ~CcEventHandler()
  {
    while (size() > 0)
    {
      CCMONITORDELETE(at(0));
      delete at(0);
      remove(0);
    }
  }

  void removeObject(CcObject* pObjectToRemove)
  {
    for (size_t i = 0; i < size(); i++)
    {
      if (at(i)->getObject() == pObjectToRemove)
      {
        CcEventHandle pObject = at(i);                                                                                                                                         
        delete pObject;
        remove(i);
        i--;
      }
    }
  }

  void call(void *pParam)
  {
    for (size_t i = 0; i < size(); i++)
      at(i)->call(pParam);
  }

  bool call(CcObject* pTarget, void *pParam)
  {
    for (size_t i = 0; i < size(); i++)
    {
      if (at(i)->getObject() == pTarget)
      {
        at(i)->call(pParam);
        return true;
      }
    }
    return false;
  }

};

#endif /* _CcEventHandler_H_ */
