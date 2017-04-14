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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CHashTest
 */
#include "CHashTest.h"
#include "Hash/CcMd5.h"
#include <cstring>

CHashTest::CHashTest( void )
{
}

CHashTest::~CHashTest( void )
{
}

bool CHashTest::test()
{
  bool bSuccess = true;
  bSuccess &= testSha256();
  bSuccess &= testMd5();
  bSuccess &= testMd5Append();
  return bSuccess;
}

static const unsigned char oTestStringResult1[] = {0xb1, 0x0a, 0x8d, 0xb1, 0x64, 0xe0, 0x75, 0x41, 0x05, 0xb7, 0xa9, 0x9b, 0xe7, 0x2e, 0x3f, 0xe5};
static const unsigned char oTestStringResult2[] = {0x83, 0xb8, 0xdb, 0x31, 0xdc, 0x1e, 0x69, 0x41, 0x9b, 0x28, 0x47, 0xd0, 0xce, 0xeb, 0x1c, 0xf6};
static const unsigned char oTestStringResult3[] = {0x45, 0xf3, 0xbd, 0x47, 0x0d, 0x24, 0xc6, 0xda, 0xaf, 0x01, 0x19, 0xa4, 0xdc, 0xf4, 0x26, 0x0d};
const char* oTestString1 = "Hello World";
const char* oTestString2 = "This test - string is longer than 128bit.";
const char* oTestString3 = "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized. "\
                           "This test-string is oversized.";

bool CHashTest::testMd5()
{
  bool bRet = false;
  CcByteArray oTestArray1(oTestString1, strlen(oTestString1));
  CcByteArray oTestArrayResult1((char*)oTestStringResult1, 16);
  CcMd5 oMd5;
  oMd5.generate(oTestArray1);
  if (oMd5.getValue() == oTestArrayResult1)
  {
    CcByteArray oTestArray2(oTestString2, strlen(oTestString2));
    CcByteArray oTestArrayResult2((char*) oTestStringResult2, 16);
    // Reuse oMd5 
    oMd5.generate(oTestArray2);
    if (oMd5.getValue() == oTestArrayResult2)
    {
      CcByteArray oTestArray3(oTestString3, strlen(oTestString3));
      CcByteArray oTestArrayResult3((char*) oTestStringResult3, 16);
      // Reuse oMd5 
      oMd5.generate(oTestArray3);
      if (oMd5.getValue() == oTestArrayResult3)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CHashTest::testMd5Append()
{
  bool bRet = false;
  CcByteArray oByteArrayConcat       (oTestString1, strlen(oTestString1));
              oByteArrayConcat.append(oTestString2, strlen(oTestString2));
  CcMd5 oMd5Concat;
  oMd5Concat.generate(oByteArrayConcat);
  CcMd5 oMd5Appended;
  oMd5Appended.append(oTestString1, strlen(oTestString1));
  oMd5Appended.append(oTestString2, strlen(oTestString2));
  oMd5Appended.finalize();
  if (oMd5Concat.getValue() == oMd5Appended.getValue())
  {
    bRet = true;
  }
  return bRet;
}

bool CHashTest::testSha256()
{
  bool bRet = true;
  // @TODO to be done!
  return bRet;
}
