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
 * @brief     Implemtation of class CReferenceCountTest
 */
#include "CReferenceCountTest.h"
#include "CcCoReferenceCount.h"

CReferenceCountTest::CReferenceCountTest() :
  CcTest("CReferenceCountTest")
{
  appendTestMethod("Test constructor", &CReferenceCountTest::testConstructor);
  appendTestMethod("Test copy", &CReferenceCountTest::testCopyConstructor);
}

CReferenceCountTest::~CReferenceCountTest()
{
}

bool CReferenceCountTest::testConstructor()
{
  bool bRet = false;
  CcCoReferenceCount oReferenceCount;
  CcCoReferenceCount oReferenceCount1(1);
  CcCoReferenceCount oReferenceCount2(2);
  oReferenceCount2.referenceCountDecrement();
  if(oReferenceCount.referenceCount() == oReferenceCount1.referenceCount() &&
     oReferenceCount.referenceCount() == oReferenceCount2.referenceCount())
  {
    bRet = true;
  }
  return bRet;
}

bool CReferenceCountTest::testCopyConstructor()
{
  bool bRet = false;
  CcCoReferenceCount oReferenceCount;
  CcCoReferenceCount oReferenceCount1(2);
  // Copy constructor
  CcCoReferenceCount oReferenceCount2(oReferenceCount1);
  // Asignment operator
  CcCoReferenceCount oReferenceCount3;
  oReferenceCount3 = oReferenceCount2;
  oReferenceCount1.referenceCountDecrement();
  oReferenceCount2.referenceCountDecrement();
  if(oReferenceCount.referenceCount() == oReferenceCount1.referenceCount() &&
     oReferenceCount.referenceCount() == oReferenceCount2.referenceCount() &&
     oReferenceCount.referenceCount() == oReferenceCount3.referenceCount())
  {
    bRet = true;
  }
  return bRet;
}
