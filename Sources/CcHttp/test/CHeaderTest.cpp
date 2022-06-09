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
 * @brief     Implemtation of class CHeaderTest
 */
#include "CHeaderTest.h"
#include "CcKernel.h"
#include "CcHttpRequest.h"

CHeaderTest::CHeaderTest() :
  CcTest("CHeaderTest")
{
  appendTestMethod("Start chunked test", &CHeaderTest::testChunked);
}

CHeaderTest::~CHeaderTest()
{
}

bool CHeaderTest::testChunked()
{
  bool bSuccess = true;
  // Parse and generate header
  return bSuccess;
}