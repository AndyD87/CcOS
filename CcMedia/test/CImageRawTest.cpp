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
 * @brief     Implemtation of class CImageRawTest
 */
#include "CImageRawTest.h"
#include "CcString.h"
#include "CcImageRaw.h"

CImageRawTest::CImageRawTest() :
  CcTest("CImageRawTest")
{
  appendTestMethod("Test basics", &CImageRawTest::testBasic);
}

CImageRawTest::~CImageRawTest()
{
}

bool CImageRawTest::testBasic()
{
  bool bRet = false;
  CcImageRaw oImage;
  CcImageRaw oImage2(10, 10);
  if(oImage.getBufferSize() == 0 &&
    oImage2.getBufferSize() == (10 * 10 * sizeof(uint32)))
  {
    bRet = true;
  }
  return bRet;
}
