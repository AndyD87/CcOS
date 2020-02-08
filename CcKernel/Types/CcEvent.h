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
 * @subpage   IEvent
 *
 * @page      IEvent
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IEvent
 */
#ifndef H_IEvent_H_
#define H_IEvent_H_

//! Forward Declaration
#include "CcBase.h"
#include "CcObject.h"
#include "CcReferenceCount.h"

class CcEvent
{
private:
  class IEventBase : public CcReferenceCount
  {
  public:
    virtual ~IEventBase() = default;
    virtual void call(void* pParam) = 0;
    inline CcObject* getObject() { return static_cast<CcObject*>(m_oObject); }
  protected:
    CcObject*         m_oObject = nullptr;
  };

  /**
   * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
   */
  template <typename OBJECTTYPE, typename PARAMTYPE>
  class IEvent : public IEventBase
  {
  public:
    using CallbackFunction = void (OBJECTTYPE::*)(PARAMTYPE* pParam);

    IEvent(OBJECTTYPE* oObject, CallbackFunction pFunc)
    {
      m_oObject = static_cast<CcObject*>(oObject);
      m_func = pFunc;
    }

    IEvent(const IEvent<CcObject, void>& oToCopy)
    {
      m_oObject = oToCopy.m_oObject;
      m_func = oToCopy.m_func;
    }

    IEvent(IEvent<CcObject, void>&& oToMove)
    {
      m_oObject = oToMove.m_oObject;
      m_func = oToMove.m_func;
      oToMove.m_oObject = nullptr;
      oToMove.m_func = nullptr;
    }


    ~IEvent()
    {
      m_oObject = nullptr;
      m_func = nullptr;
    }

    virtual void call(void* pParam) override
    {
      (*object().*m_func)(static_cast<PARAMTYPE*>(pParam));
    }

  private:
    inline OBJECTTYPE* object()
    {
      return static_cast<OBJECTTYPE*>(m_oObject);
    }

    CallbackFunction  m_func;
  };

  CcEvent(IEventBase* pEvent) :
    m_pEvent(pEvent)
  { }

public:
  CcEvent() = default;
  CcEvent(const CcEvent& rEvent)
  { operator=(rEvent); }
  virtual ~CcEvent()
  { clear();}

  template <typename OBJECTTYPE, typename PARAMTYPE>
  static CcEvent create(OBJECTTYPE* pObject, void (OBJECTTYPE::*pFunction)(PARAMTYPE* pParam))
  {
    CCNEWTYPE(pEvent, IEvent<OBJECTTYPE CCCOMMA PARAMTYPE>, pObject, pFunction);
    return CcEvent(pEvent);
  }

  CcEvent& operator=(const CcEvent& rEvent)
  { clear(); m_pEvent = rEvent.m_pEvent; m_pEvent->referenceCountIncrement(); return *this; }

  inline CcObject* getObject() { return m_pEvent ? m_pEvent->getObject() : nullptr;  }
  inline void call(void* pParam) { if(m_pEvent) m_pEvent->call(pParam); }
  void clear()
    { CCDELETEREF(m_pEvent); }
private:
  IEventBase* m_pEvent = nullptr;
};

#define NewCcEvent(CCOBJECTTYPE,CCPARAMETERTYPE,CCMETHOD,CCOBJECT) CcEvent::create<CCOBJECTTYPE, CCPARAMETERTYPE>(CCOBJECT,&CCMETHOD)

#endif // H_IEvent_H_
