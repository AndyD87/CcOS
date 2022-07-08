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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IEventType
 */
#pragma once

//! Forward Declaration
#include "CcBase.h"
#include "CcObject.h"
#include "CcReferenceCount.h"

class CcEventAction;
class CcEventActionLoop;

/**
 * @brief Gobal event class for creating events to register on event handlers.
 *        For creating an object-method pair to call folr handlers, use one of the following macros:
 *
 *        NewCcEvent:         Fast way to create event with object and method, but no check for paramters.
 *        NewCcEventType:     It is a type save registration but requires more input
 *        NewCcEventTypeSave: On call it will wait until all threads are done.
 */
class CcKernelSHARED CcEvent
{
private:
  /**
   * @brief Basic event storage class with abstract interface for calling an object.
   */
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
      m_pObject = static_cast<CcObject*>(oObject);
      m_pFunc = pFunc;
    }

    virtual ~IEventType()
    {
      m_pObject = nullptr;
      m_pFunc = nullptr;
    }

    virtual void call(void* pParam) override
    {
      (*object().*m_pFunc)(static_cast<PARAMTYPE*>(pParam));
    }

    virtual CcObject* getObject() override
    {
      return m_pObject;
    }

  private:
    inline OBJECTTYPE* object()
    {
      return static_cast<OBJECTTYPE*>(m_pObject);
    }

  protected:
    CcObject*                                   m_pObject;
    void (OBJECTTYPE::*m_pFunc)(PARAMTYPE*);
  };

  class IEventObject : public IEventBase
  {
  public:
    IEventObject(CcObject* oObject, CcObject::FObjectMethod pFunc);
    virtual ~IEventObject();

    virtual void call(void* pParam) override;
    virtual CcObject* getObject() override;
  protected:
    CcObject*         m_pObject;
    #ifdef _MSC_VER
      // Avoid warning in VS
      #pragma warning (disable:4121)
      CcObject::FObjectMethod           m_pFunc;
      #pragma warning (default:4121)
    #else
      CcObject::FObjectMethod           m_pFunc;
    #endif
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

  /**
   * @brief Check if basic event interface is same
   * @param rEvent: Event to compare
   * @return True if both Events using the same basic event handle
   */
  bool operator==(const CcEvent& rEvent) const
  { return m_pEvent == rEvent.m_pEvent; }

  /**
   * @brief Copy data from another event to this.
   * @param rEvent: Event to duplicate
   * @return Handle to this.
   */
  CcEvent& operator=(const CcEvent& rEvent);

  /**
   * @brief Move data from another event to this.
   * @param rEvent: Event to move from
   * @return Handle to this.
   */
  CcEvent& operator=(CcEvent&& rEvent);

  //! @return Get target object of event, or null if not defined
  inline CcObject* getObject() const { if(m_pEvent) return m_pEvent->getObject(); return nullptr;  }
  /**
   * @brief Call method of stored object with paramter.
   * @param pParam: Paramter to pass to call method.
   */
  inline void call(void* pParam) { if(m_pEvent) m_pEvent->call(pParam); }

  /**
   * @brief Remove object and method.
   */
  void clear();

  /**
   * @brief Create a new event by object and basic method type.
   * @param pObject:    Object to call on event
   * @param pFunction:  Method in Object to call on event
   * @return Created event object.
   */
  static CcEvent create(CcObject* pObject, CcObject::FObjectMethod pFunction);

  /**
   * @brief Create new event type save by verfiying parameter type and object method
   * @param pObject   : Object to call on event
   * @param pFunction : Method in Object to call on event
   * @return Created event object.
   */
  template <typename OBJECTTYPE, typename PARAMTYPE>
  static CcEvent createType(OBJECTTYPE* pObject, void (OBJECTTYPE::*pFunction)(PARAMTYPE* pParam))
  {
    CCNEWTYPE(pEvent, IEventType<OBJECTTYPE CCCOMMA PARAMTYPE>, pObject, pFunction);
    return CcEvent(pEvent);
  }

  /**
   * @brief Threadsave event, it requires the eventloop of target thread to queue event on call.
   * @param pLoop     : Register eventloop to insert object on call
   * @param pObject   : Object to call on event
   * @param pFunction : Method in Object to call on event
   * @return Created event object.
   */
  template <typename OBJECTTYPE, typename PARAMTYPE>
  static CcEvent createSave(CcEventActionLoop* pLoop, OBJECTTYPE* pObject, void (OBJECTTYPE::*pFunction)(PARAMTYPE*))
  {
    CCNEWTYPE(pEvent, IEventType<OBJECTTYPE CCCOMMA PARAMTYPE>, pObject, pFunction);
    CCNEWTYPE(pEventSave, IEventSave<OBJECTTYPE CCCOMMA PARAMTYPE>, pLoop, pEvent, pObject, pFunction);
    return CcEvent(pEventSave);
  }

  /**
   * @brief Check if event is successfully created or empty
   * @return True if event is valid.
   */
  inline bool isValid()
  { return m_pEvent != nullptr; }

private:
  IEventBase* m_pEvent = nullptr;
};

/**
 * @brief Cast class method down to CcObject methdo to call from rom CcObject base later.
 * @param CLASS: Class to cast away from
 * @param FUNC:  Pointer to member function within CLASS
 * @return CcObject::FObjectMethod Method which is callable from CcObject
 */
#define CcEvent_EventCasting(CLASS,FUNC) static_cast<CcObject::FObjectMethod>(reinterpret_cast<void (CLASS::*)(void*)>(FUNC))

/**
 * @brief Create new event type save by verfiying parameter type and object method
 * @param CCOBJECTTYPE    : Type of object to call on event.
 * @param CCPARAMETERTYPE : Type of paramter to pass on event
 * @param CCOBJECT        : Object to call on event
 * @param CCMETHOD        : Method in Object to call on event
 * @return Created event object.
 */
#define NewCcEventType(CCOBJECTTYPE,CCPARAMETERTYPE,CCOBJECT,CCMETHOD) CcEvent::createType<CCOBJECTTYPE, CCPARAMETERTYPE>(CCOBJECT,&CCMETHOD)

/**
 * @brief Threadsave event, it requires the eventloop of target thread to queue event on call.
 * @param CCSAVELOOP      : Register eventloop to insert object on call
 * @param CCOBJECTTYPE    : Type of object to call on event.
 * @param CCPARAMETERTYPE : Type of paramter to pass on event
 * @param CCOBJECT        : Object to call on event
 * @param CCMETHOD        : Method in Object to call on event
 * @return Created event object.
 */
#define NewCcEventTypeSave(CCSAVELOOP,CCOBJECTTYPE,CCPARAMETERTYPE,CCOBJECT,CCMETHOD) CcEvent::createSave<CCOBJECTTYPE, CCPARAMETERTYPE>(CCSAVELOOP,CCOBJECT,&CCMETHOD)

/**
 * @brief Create new event type save by verfiying parameter type and object method
 * @param CCOBJECT   : Object to call on event
 * @param CCMETHOD   : Method in Object to call on event
 * @return Created event object.
 */
#define NewCcEvent(CCOBJECT,CCMETHOD) \
  CcEvent::create(CCOBJECT,CcEvent_EventCasting(CLASSOFP(CCOBJECT),&CCMETHOD))

/**
 * @brief Create new empty event with no target to call
 * @return Created event object.
 */
#define NewCcEventEmpty() \
  CcEvent::create(nullptr,nullptr)
