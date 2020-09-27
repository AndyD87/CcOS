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
 * @brief     Implemtation of class CEventTest
 */
#include "CEventTest.h"
#include "CcPassword.h"
#include "CcConsole.h"
#include "CcObject.h"
#include "CcEventHandler.h"
#include "CcEventActionLoop.h"
#include "IThread.h"
#include "CcKernel.h"

class CEventTestObject : public CcObject
{
public:
  inline CcEventHandler& getOnDeleteHandler()
  { return CcObject::getOnDeleteHandler();}
  void test(uint8*puiVal)
  {uiVal = *puiVal;}

  uint8 uiVal = uiDefaultValue;
  static const uint8 uiDefaultValue;
};

const uint8 CEventTestObject::uiDefaultValue = 0;

class CLoopTest : public CcEventActionLoop, public CcObject
{
public:
  virtual bool onLoop() override
  {
    if (bDone == false)
    {
      if (uiTimeout > 0)
      {
        uiTimeout--;
        if (bFirst)
          bFirst = false;
        else
          CcKernel::sleep(1000);
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      bSuccess = true;
    }
    return false;
  }

  void SetDone(void*)
  { bDone = true; }

  bool bDone        = false;  //!< Default Run
  uint32 uiTimeout  = 30;     //!< Wait maximum 30 seconds!
  bool bSuccess     = false;
  bool bFirst       = true;
};

class CEventThread : public IThread
{
public:
  CcEventHandler oHandler;

  virtual void run() override
  {
    oHandler.call(nullptr);
  }
};

class CEventTestHandler : public CcEventHandler
{
public:
  void testCall(uint8* pValue)
  { call(pValue); }
};

CEventTest::CEventTest() :
  CcTest<CEventTest>("CEventTest")
{
  appendTestMethod("Test auto remove on delete", &CEventTest::testAutoRemove);
  appendTestMethod("Test auto remove on delete of handler", &CEventTest::testAutoRemoveHandler);
  appendTestMethod("Test thread based event calls", &CEventTest::testThreadSaveEvent);
  appendTestMethod("Test short event declarations", &CEventTest::testShortEvent);
}

CEventTest::~CEventTest()
{
}

bool CEventTest::testAutoRemove()
{
  bool bSuccess = false;
  CEventTestHandler oHandler;
  // Test by deleting on leaving scope
  {
    CEventTestObject oObject;
    oHandler.append(NewCcEventType(CEventTestObject, uint8, CEventTestObject::test, &oObject));
    if(oHandler.size() > 0)
    {
      bSuccess = true;
    }
  }
  if(bSuccess)
  {
    if(oHandler.size() > 0)
    {
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool CEventTest::testAutoRemoveHandler()
{
  bool bSuccess = false;
  CCNEWTYPE(pHandler, CEventTestHandler);
  CEventTestObject oObject;
  pHandler->append(NewCcEventType(CEventTestObject, uint8, CEventTestObject::test, &oObject));
  if (pHandler->size() > 0 &&
      oObject.uiVal == 0)
  {
    // Test set value on Sender
    uint8 uiValue = 0xdd;
    pHandler->testCall(&uiValue);
    // Check value on Receiver
    if(oObject.uiVal == 0xdd)
    {
      if (oObject.getOnDeleteHandler().size() > 0)
      {
        CCDELETE(pHandler);
        // This is a test for CCDELETE itself
        if(pHandler == nullptr)
        {
          if (oObject.getOnDeleteHandler().size() == 0)
          {
            bSuccess = true;
          }
        }
      }
    }
  }
  if(pHandler != nullptr)
  {
    CCDELETE(pHandler);
  }
  return bSuccess;
}

bool CEventTest::testThreadSaveEvent()
{
  CLoopTest oLoopTest;
  CEventThread oThread;
  oThread.oHandler.append(NewCcEventTypeSave(&oLoopTest, CLoopTest, void, CLoopTest::SetDone, &oLoopTest));
  oThread.start();
  CcKernel::sleep(100);
  oLoopTest.loop();
  return oLoopTest.bSuccess;
}

bool CEventTest::testShortEvent()
{
  bool bSuccess = false;
  CcEventHandler oHandler;
  CEventTestObject oObject;
  CcEvent oEvent = NewCcEvent(&oObject, CEventTestObject::test);
  oHandler.append(NewCcEvent(&oObject, CEventTestObject::test));
  if(oObject.uiVal == oObject.uiDefaultValue)
  {
    uint8 uiNewValue = 1;
    oHandler.call(&uiNewValue);
    if(oObject.uiVal != oObject.uiDefaultValue &&
       oObject.uiVal == uiNewValue)
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}
