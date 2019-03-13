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
 * @brief     Implemtation of class CHashTest
 */
#include "CHashTest.h"
#include "Hash/CcMd5.h"
#include "Hash/CcSha256.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcConsole.h"

const CcString g_sMidstate = "24e39e501efebbc8fb545b91db1ff3caa66f356d7482c0f3acc0caa800f10dad";
const CcString g_sResData = "00000002b15704f4ecae05d077e54f6ec36da7f20189ef73b77603225ae56d2b00000000b052cbbdeed2489ccb13a526b77fadceef4caf7d3bb82a9eb0b69ebb90f9f5a7510c27fd1c0e8a37fa531338";

CHashTest::CHashTest()
{
  appendTestMethod("Test sha256", &CHashTest::testSha256);
  appendTestMethod("Test md5", &CHashTest::testMd5);
  appendTestMethod("Test md5 append bug", &CHashTest::testMd5Append);
  appendTestMethod("Test CcHash", &CHashTest::testIHash);
}

CHashTest::~CHashTest()
{
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
  CcByteArray oTestArray1(oTestString1, CcStringUtil::strlen(oTestString1));
  CcByteArray oTestArrayResult1((char*)oTestStringResult1, 16);
  CcMd5 oMd5;
  oMd5.generate(oTestArray1);
  if (oMd5.getValue() == oTestArrayResult1)
  {
    CcByteArray oTestArray2(oTestString2, CcStringUtil::strlen(oTestString2));
    CcByteArray oTestArrayResult2((char*) oTestStringResult2, 16);
    // Reuse oMd5 
    oMd5.generate(oTestArray2);
    if (oMd5.getValue() == oTestArrayResult2)
    {
      CcByteArray oTestArray3(oTestString3, CcStringUtil::strlen(oTestString3));
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
  CcByteArray oByteArrayConcat       (oTestString1, CcStringUtil::strlen(oTestString1));
              oByteArrayConcat.append(oTestString2, CcStringUtil::strlen(oTestString2));
  CcMd5 oMd5Concat;
  oMd5Concat.generate(oByteArrayConcat);
  CcMd5 oMd5Appended;
  oMd5Appended.append(oTestString1, CcStringUtil::strlen(oTestString1));
  oMd5Appended.append(oTestString2, CcStringUtil::strlen(oTestString2));
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
  CcSha256 oHash1;
  oHash1.generate(CcByteArray("test", 4));
  CcString sHash1 = oHash1.getValue().getHexString();
  if (sHash1 != "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08")
  {
    bRet = false;
  }
  CcSha256 oHash2;
  oHash2.generate("123456781", 9);
  CcString sHash2 = oHash2.getValue().getHexString();
  if (sHash2 != "154c4c511cbb166a317c247a839e46cac6d9208af5b015e1867a84cd9a56007b")
  {
    bRet = false;
  }
  CcSha256 oHash3;
  oHash3.generate(oTestString3, CcStringUtil::strlen(oTestString3));
  CcString sHash3 = oHash3.getValue().getHexString();
  if (sHash3 != "ed76c7686c7f412e7235bacbd88f79f1f40e1523b52b40e81952c8c2955a152b")
  {
    bRet = false;
  }
  // Test bitcoin mining block without midstate
  {
    CcByteArray oData;
    oData.setHexString(g_sResData);
    oData.swapEndian32();
    CcSha256 oFirstHash;
    oFirstHash.generate(oData);
    CcSha256 oSecondHash;
    oSecondHash.generate(oFirstHash.getValue());
    CcByteArray oResult = oSecondHash.getValue();
    if (oResult[7 * 4] != 0 ||
      oResult[7 * 4 + 1] != 0 ||
      oResult[7 * 4 + 2] != 0 ||
      oResult[7 * 4 + 3] != 0)
    {
      bRet = false;
    }
  }
  // Midstate test from Bitcoin block mining
  {
    CcByteArray oMidstate;
    oMidstate.setHexString(g_sMidstate);
    CcByteArray oData;
    oData.setHexString(g_sResData);
    oData.swapEndian32();
    CcSha256 oFirstHash;
    oFirstHash.setMidstate(oMidstate, 0x40);
    oFirstHash.finalize(oData.getArray(0x40), 0x10);
    CcSha256 oSecondHash;
    oSecondHash.generate(oFirstHash.getValue());
    CcByteArray oResult = oSecondHash.getValue();
    if (oResult[7 * 4] != 0 ||
        oResult[7 * 4 + 1] != 0 ||
        oResult[7 * 4 + 2] != 0 ||
        oResult[7 * 4 + 3] != 0)
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CHashTest::testIHash()
{
  CcHash oHash; // currently just create it to avoid compiler to build non implemented abstract class.
  return true;
}
