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

class CEventTestObject : public CcObject
{
public:
  inline CcEventHandler& getOnDeleteHandler()
  { return CcObject::getOnDeleteHandler();}
  void test(uint8*puiVal)
  {uiVal = *puiVal;}

  uint8 uiVal = 0;
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
    oHandler.append(NewCcEvent(CEventTestObject, uint8, CEventTestObject::test, &oObject));
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
  pHandler->append(NewCcEvent(CEventTestObject, uint8, CEventTestObject::test, &oObject));
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
