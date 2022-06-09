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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CListTest
 */
#include "CListTest.h"
#include "CcString.h"
#include "CcConsole.h"
#include "CcKernel.h"
#include "CcFileSystem.h"
#include "CcGlobalStrings.h"

CListTest::CListTest() : 
  CcTest("CListTest")
{
  appendTestMethod("Test inserts to list", &CListTest::testInsert);
  appendTestMethod("Test reserve list", &CListTest::testReserve);
  appendTestMethod("Test remove from list", &CListTest::testRemove);
  appendTestMethod("Test imported data from braces list", &CListTest::testInitializerList);
}

CListTest::~CListTest()
{
}

bool CListTest::testInsert()
{
  bool bRet = false;
  CcList<int> oIntList;
  oIntList.prepend(100);
  oIntList.append(102);
  oIntList.insert(0, 99);
  oIntList.insert(2, 101);
  oIntList.prepend(98);
  if (oIntList[0] == 98 &&
    oIntList[1] == 99 &&
    oIntList[2] == 100 &&
    oIntList[3] == 101 &&
    oIntList[4] == 102)
  {
    // Test dequeing and enquing
    CcList<int>::iterator oEnqueuedFirst = oIntList.dequeueFirst();
    CcList<int>::iterator oEnqueued = oIntList.dequeue(1);
    CcList<int>::iterator oEnqueuedLast = oIntList.dequeueLast();
    oIntList.insert(0, oEnqueuedLast);
    oIntList.insert(2, oEnqueuedFirst);
    oIntList.insert(4, oEnqueued);
    if (oIntList[0] == 102 &&
      oIntList[1] == 99 &&
      oIntList[2] == 98 &&
      oIntList[3] == 101 &&
      oIntList[4] == 100)
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CListTest::testReserve()
{
  bool bSuccess = false;
  CcList<int> oList(static_cast<size_t>(256));
  if(oList.size() == 256)
  {
    int i=0;
    // Test for possible crash
    for(int& iItem : oList)
    {
      iItem = i;
      i++;
    }
    if(oList.at(255) == 255) bSuccess = true;
  }
  return bSuccess;
}

bool CListTest::testRemove()
{
  bool bSuccess = false;
  CcList<int> oIntList;
  oIntList.prepend(100);
  oIntList.append(102);
  oIntList.insert(0, 99);
  oIntList.insert(2, 101);
  oIntList.prepend(98);
  if(oIntList.size() == 5)
  {
    oIntList.remove(1);
    if(oIntList.size() == 4 &&
       oIntList[0] == 98 &&
       oIntList[1] == 100
       )
    {
      oIntList.remove(1,2);
      if(oIntList.size() == 2 &&
         oIntList[0] == 98 &&
         oIntList[1] == 102
         )
      {
        oIntList.remove(0);
        if(oIntList.size() == 1 &&
           oIntList[0] == 102
           )
        {
          bSuccess = true;
        }
      }
    }
  }
  return bSuccess;
}

bool CListTest::testInitializerList()
{
  bool bSuccess = false;
  if(m_oTestCc[0][0] == "Hallo" &&
     m_oTestCc[0][1] == "du"    &&
     m_oTestCc[1][0] == "da")
  {
    bSuccess = true;
  }
  return bSuccess;
}