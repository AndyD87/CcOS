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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcEventHandler
 */
#ifndef CcEventHandler_H_
#define CcEventHandler_H_

//! Forward Declaration
#include "CcKernelBase.h"
#include "CcEvent.h"
#include "CcList.h"
#include "CcObject.h"

typedef CcEvent<CcObject, void>* CcEventHandle;
#define EventHandle(var) CcEventHandle(new var)

/**
* @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
*/
class CcKernelSHARED CcEventHandler : public CcList<CcEventHandle>
{
public:
  CcEventHandler()
    {}

  virtual ~CcEventHandler()
  {
    while (size() > 0)
    {
      CCMONITORDELETE(at(0)); delete at(0);
      remove(0);
    }
  }

  void removeObject(CcObject* pObjectToRemove)
  {
    for (size_t i = 0; i < size(); i++)
    {
      if (at(i)->getObject() == pObjectToRemove)
      {
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

#endif /* CcEventHandler_H_ */
