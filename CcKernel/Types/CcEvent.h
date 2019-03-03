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
 * @subpage   CcEvent
 *
 * @page      CcEvent
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEvent
 */
#ifndef _CcEvent_H_
#define _CcEvent_H_

//! Forward Declaration
#include "CcKernelBase.h"
#include "CcObject.h"
#include "CcString.h"

class CcKernelSHARED IEvent
{
public:
  IEvent() = default;
  virtual ~IEvent() = default;

  virtual void call(void*)
  {
    CCDEBUG("That would be a pure call");
  }

  inline CcObject* getObject() { return static_cast<CcObject*>(m_oObject); }

  CcObject* m_oObject = nullptr;
};

typedef IEvent* CcEventHandle;

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */

template <typename OBJECTTYPE, typename PARAMTYPE>
class CcEvent : public IEvent
{
  typedef void (OBJECTTYPE::*CallbackFunction)(PARAMTYPE* pParam);

public:
  CcEvent(OBJECTTYPE* oObject, CallbackFunction pFunc)
  {
    m_oObject = static_cast<CcObject*>(oObject);
    m_func = pFunc;
  }

  CcEvent(const CcEvent<CcObject, void>& oToCopy)
  {
    m_oObject = oToCopy.m_oObject;
    m_func = oToCopy.m_func;
  }

  CcEvent(CcEvent<CcObject, void>&& oToMove)
  {
    m_oObject = oToMove.m_oObject;
    m_func = oToMove.m_func;
    oToMove.m_oObject = nullptr;
    oToMove.m_func = nullptr;
  }


  virtual ~CcEvent()
  {
    m_oObject = nullptr;
    m_func = nullptr;
  }

  virtual void call(void* pParam) override
  {
    (*object().*m_func)(static_cast<PARAMTYPE*>(pParam));
  }

  static CcEventHandle create(OBJECTTYPE* pObject, CallbackFunction pFunction)
  {
    return ((CcEventHandle)(static_cast<void*>(new CcEvent<OBJECTTYPE, PARAMTYPE>(pObject, pFunction))));
  }
private:
  inline OBJECTTYPE* object()
  {
    return static_cast<OBJECTTYPE*>(m_oObject);
  }

  CallbackFunction m_func;
};

#define NewCcEvent(CCOBJECTTYPE,CCPARAMETERTYPE,CCMETHOD,CCOBJECT) CcEvent<CCOBJECTTYPE, CCPARAMETERTYPE>::create(CCOBJECT,&CCMETHOD)

#endif /* _CcEvent_H_ */
