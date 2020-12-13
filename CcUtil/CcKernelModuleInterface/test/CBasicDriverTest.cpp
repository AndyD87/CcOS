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
 * @brief     Implemtation of class CBasicDriverTest
 */
#include "CBasicDriverTest.h"
#include "CcString.h"

CBasicDriverTest::CBasicDriverTest() :
  CcTest("CBasicDriverTest")
{
  appendTestMethod("Test basics", &CBasicDriverTest::testBasic);
}

CBasicDriverTest::~CBasicDriverTest()
{
}

bool CBasicDriverTest::testBasic()
{
  bool bRet = true;
  //sc stop TestDriver
  //sc delete TestDriver
  //sc create TestDriver type = kernel start = demand binpath = CcBasicDriver.sys
  //sc start TestDriver
  return bRet;
}
