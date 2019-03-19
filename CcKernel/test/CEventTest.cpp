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
  void test(void*){}
};

class CEventTestHandler
{
public:
  CcEventHandler oHandler;
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
    oHandler.oHandler.append(NewCcEvent(CEventTestObject, void, CEventTestObject::test, &oObject));
    if(oHandler.oHandler.size() > 0)
    {
      bSuccess = true;
    }
  }
  if(bSuccess)
  {
    if(oHandler.oHandler.size() > 0)
    {
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool CEventTest::testAutoRemoveHandler()
{
  bool bSuccess = false;
  CEventTestHandler* pHandler =  new CEventTestHandler();
  CEventTestObject oObject;
  pHandler->oHandler.append(NewCcEvent(CEventTestObject, void, CEventTestObject::test, &oObject));
  if (pHandler->oHandler.size() > 0)
  {
    if (oObject.getOnDeleteHandler().size() > 0)
    {
      delete pHandler;
      pHandler = nullptr;
      if (oObject.getOnDeleteHandler().size() == 0)
      {
        bSuccess = true;
      }
    }
  }
  if(pHandler != nullptr)
  {
    delete pHandler;
  }
  return bSuccess;
}
