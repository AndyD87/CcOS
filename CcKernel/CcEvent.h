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
 * @subpage   IEventType
 *
 * @page      IEventType
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IEventType
 */
#ifndef H_IEvent_H_
#define H_IEvent_H_

//! Forward Declaration
#include "CcBase.h"
#include "CcReferenceCount.h"

class CcObject;
class CcEventAction;
class CcEventActionLoop;

class CcKernelSHARED CcEvent
{
private:
  class IEventBase : public CcReferenceCount
  {
  public:
    IEventBase() : CcReferenceCount(0)
    {}
    virtual ~IEventBase() = default;
    virtual void call(void* pParam) = 0;
    virtual CcObject* getObject() = 0;
  };

  /**
   * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
   */
  template <typename OBJECTTYPE, typename PARAMTYPE>
  class IEventType : public IEventBase
  {
  public:
    IEventType(OBJECTTYPE* oObject, void (OBJECTTYPE::*pFunc)(PARAMTYPE*))
    {
      m_oObject = static_cast<CcObject*>(oObject);
      m_func = pFunc;
    }

    virtual ~IEventType()
    {
      m_oObject = nullptr;
      m_func = nullptr;
    }

    virtual void call(void* pParam) override
    {
      (*object().*m_func)(static_cast<PARAMTYPE*>(pParam));
    }

    virtual CcObject* getObject() override
    {
      return m_oObject;
    }

  private:
    inline OBJECTTYPE* object()
    {
      return static_cast<OBJECTTYPE*>(m_oObject);
    }

  protected:
    CcObject*                                   m_oObject;
    void (OBJECTTYPE::*m_func)(PARAMTYPE*);
  };

  class IEventTest : public IEventBase
  {
  public:
    IEventTest(CcObject* oObject, void (CcObject::*pFunc)(void*))
    {
      m_oObject = static_cast<CcObject*>(oObject);
      m_func = pFunc;
    }

    virtual ~IEventTest()
    {
      m_oObject = nullptr;
      m_func = nullptr;
    }

    virtual void call(void* pParam) override
    {
      (*object().*m_func)(pParam);
    }

    virtual CcObject* getObject() override
    {
      return m_oObject;
    }

  private:
    inline CcObject* object()
    {
      return m_oObject;
    }

  protected:
    CcObject*         m_oObject;
    void (CcObject::* m_func)(void*);
  };

  template <typename OBJECTTYPE, typename PARAMTYPE>
  class IEventSave : public IEventType<OBJECTTYPE,PARAMTYPE>
  {
  public:
    IEventSave(CcEventActionLoop* pSave, IEventType<OBJECTTYPE, PARAMTYPE>* pEvent, OBJECTTYPE* oObject, void (OBJECTTYPE::*pFunction)(PARAMTYPE*)) :
      IEventType<OBJECTTYPE,PARAMTYPE>(oObject, pFunction),
      m_pSave(pSave),
      m_pEvent(pEvent)
    { m_pEvent->referenceCountIncrement(); }

    virtual ~IEventSave()
    { CCDELETEREF(m_pEvent); }

    virtual void call(void* pParam) override
    { CcEvent::call(m_pSave, m_pEvent, pParam); }

  protected:
    CcEventActionLoop*              m_pSave;
    IEventType<OBJECTTYPE, PARAMTYPE>*  m_pEvent;
  };

  CcEvent(IEventBase* pEvent) :
    m_pEvent(pEvent)
  { m_pEvent->referenceCountIncrement(); }

  static void call(CcEventActionLoop* pSave, IEventBase* pEvent, void* pParam);

public:
  CcEvent()
  { }
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcEvent)
  ~CcEvent()
  { clear();}

  bool operator==(const CcEvent& rEvent) const
  { return m_pEvent == rEvent.m_pEvent; }
  bool operator!=(const CcEvent& rEvent) const
  { return m_pEvent != rEvent.m_pEvent; }
  CcEvent& operator=(const CcEvent& rEvent);
  CcEvent& operator=(CcEvent&& rEvent);

  inline CcObject* getObject() { if(m_pEvent) return m_pEvent->getObject(); return nullptr;  }
  inline void call(void* pParam) { if(m_pEvent) m_pEvent->call(pParam); }
  void clear();

  static CcEvent create(CcObject* pObject, void (CcObject::*pFunction)(void* pParam))
  {
    CCNEWTYPE(pEvent, IEventTest, pObject, pFunction);
    return CcEvent(pEvent);
  }

  template <typename OBJECTTYPE, typename PARAMTYPE>
  static CcEvent createType(OBJECTTYPE* pObject, void (OBJECTTYPE::*pFunction)(PARAMTYPE* pParam))
  {
    CCNEWTYPE(pEvent, IEventType<OBJECTTYPE CCCOMMA PARAMTYPE>, pObject, pFunction);
    return CcEvent(pEvent);
  }

  template <typename OBJECTTYPE, typename PARAMTYPE>
  static CcEvent createSave(CcEventActionLoop* pLoop, OBJECTTYPE* pObject, void (OBJECTTYPE::*pFunction)(PARAMTYPE*))
  {
    CCNEWTYPE(pEvent, IEventType<OBJECTTYPE CCCOMMA PARAMTYPE>, pObject, pFunction);
    CCNEWTYPE(pEventSave, IEventSave<OBJECTTYPE CCCOMMA PARAMTYPE>, pLoop, pEvent, pObject, pFunction);
    return CcEvent(pEventSave);
  }

private:
  IEventBase* m_pEvent = nullptr;
};

#define __NewCcEventType(CCOBJECTTYPE,CCPARAMETERTYPE,CCMETHOD,CCOBJECT) CcEvent::createType<CCOBJECTTYPE, CCPARAMETERTYPE>(CCOBJECT,&CCMETHOD)
#define __NewCcEventTypeSave(CCSAVELOOP,CCOBJECTTYPE,CCPARAMETERTYPE,CCMETHOD,CCOBJECT) CcEvent::createSave<CCOBJECTTYPE, CCPARAMETERTYPE>(CCSAVELOOP,CCOBJECT,&CCMETHOD)
#define NewCcEvent(CCOBJECT,CCMETHOD) \
  CcEvent::create(CCOBJECT,reinterpret_cast<void (CcObject::*)(void*)>(&CCMETHOD))

#endif // H_IEvent_H_
