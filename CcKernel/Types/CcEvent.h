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

class CcKernelSHARED CcEvent
{
private:
  class IEventBase : public CcReferenceCount
  {
  public:
    virtual ~IEventBase() = default;
    virtual void call(void* pParam) = 0;
    virtual CcObject* getObject() = 0;
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

    ~IEvent()
    {
      m_oObject = nullptr;
      m_func = nullptr;
    }

    virtual void call(void* pParam) override
    {
      (*object().*m_func)(static_cast<PARAMTYPE*>(pParam));
    }

    virtual CcObject* getObject()
    {
      return m_oObject;
    }

  private:
    inline OBJECTTYPE* object()
    {
      return static_cast<OBJECTTYPE*>(m_oObject);
    }

  protected:
    CcObject*         m_oObject = nullptr;
    CallbackFunction  m_func;
  };

  CcEvent(IEventBase* pEvent) :
    m_pEvent(pEvent)
  { }

public:
  CcEvent()
  { }
  CcEvent(const CcEvent& rEvent)
  { operator=(rEvent); }
  CcEvent(CcEvent&& rEvent)
  { operator=(CCMOVE(rEvent)); }
  virtual ~CcEvent()
  { clear();}

  template <typename OBJECTTYPE, typename PARAMTYPE>
  static CcEvent create(OBJECTTYPE* pObject, void (OBJECTTYPE::*pFunction)(PARAMTYPE* pParam))
  {
    CCNEWTYPE(pEvent, IEvent<OBJECTTYPE CCCOMMA PARAMTYPE>, pObject, pFunction);
    return CcEvent(pEvent);
  }

  bool operator==(const CcEvent& rEvent) const
  { return m_pEvent == rEvent.m_pEvent; }
  bool operator!=(const CcEvent& rEvent) const
  { return m_pEvent != rEvent.m_pEvent; }
  CcEvent& operator=(const CcEvent& rEvent);
  CcEvent& operator=(CcEvent&& rEvent);

  inline CcObject* getObject() { if(m_pEvent) return m_pEvent->getObject(); return nullptr;  }
  inline void call(void* pParam) { if(m_pEvent) m_pEvent->call(pParam); }
  void clear();
private:
  IEventBase* m_pEvent = nullptr;
};

#define NewCcEvent(CCOBJECTTYPE,CCPARAMETERTYPE,CCMETHOD,CCOBJECT) CcEvent::create<CCOBJECTTYPE, CCPARAMETERTYPE>(CCOBJECT,&CCMETHOD)

#endif // H_IEvent_H_
