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
 * @brief     Implemtation of class CStringTest
 */
#include "CStringTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"
#include "CcConsole.h"

CStringTest::CStringTest() :
  CcTest("CStringTest")
{
  appendTestMethod("Basic test", &CStringTest::test1);
  appendTestMethod("Test encodings", &CStringTest::baseEncodings);
  appendTestMethod("Test conversion Bytes<>String", &CStringTest::testStringConversions);
  appendTestMethod("Test numbers", &CStringTest::testInteger);
  appendTestMethod("Test unsigned", &CStringTest::testUnsignedInteger);
  appendTestMethod("Test float and double conversion", &CStringTest::testFloatAndDouble);
  appendTestMethod("Test path manipulations", &CStringTest::testPaths);
}

CStringTest::~CStringTest()
{
}

bool CStringTest::test1()
{
  CcString oTest1("Test");
  CcString oTest2;
  oTest2 = "Test";
  return oTest2 == oTest1 && oTest1 == "Test";
}

bool CStringTest::baseEncodings()
{
  bool bSuccess = true;
  for (char i = 0; i < 100; i++)
  {
    CcByteArray oTestArray;
    CcString sBase64;
    CcByteArray oDecodedArray;
    for (char c = 0; c < i+1; c++) oTestArray.append(&c, 1);
    sBase64 = CcStringUtil::encodeBase58(oTestArray);
    oDecodedArray = CcStringUtil::decodeBase58(sBase64);
    if (oDecodedArray != oTestArray)
    {
      CCDEBUG("Base58 conversion failed:");
      CCDEBUG("Source: " + oTestArray.getHexString());
      CCDEBUG("Output: " + oDecodedArray.getHexString());
      bSuccess = false;
    }
  }
  for (char i = 0; i < 100; i++)
  {
    CcByteArray oTestArray("a");
    CcString sBase64;
    CcByteArray oDecodedArray;
    for (char c = 0; c < i; c++) oTestArray.append(&c, 1);
    sBase64 = CcStringUtil::encodeBase64(oTestArray);
    oDecodedArray = CcStringUtil::decodeBase64(sBase64);
    if (oDecodedArray != oTestArray)
    {
      CCDEBUG("Base64 conversion failed:");
      CCDEBUG("Source: " + oTestArray.getHexString());
      CCDEBUG("Output: " + oDecodedArray.getHexString());
      bSuccess = false;
    }
  }

  /**
   * Test Encodings by converting from one to other
   */
  for (char i = 30; i < 127; i++)
  {
    CcByteArray oTestArray;
    for (char c = 0; c < i; c++) oTestArray.append(&c, 1);
    CcByteArray oBase58Encoded = CcStringUtil::encodeBase58(oTestArray);
    CcByteArray oBase64Encoded = CcStringUtil::encodeBase64(oBase58Encoded);
    CcByteArray oBase64Decoded = CcStringUtil::decodeBase64(oBase64Encoded);
    CcByteArray oBase58Decoded = CcStringUtil::decodeBase58(oBase64Decoded);
    if (oBase58Decoded != oTestArray)
    {
      CCDEBUG("Base64 conversion failed:");
      CCDEBUG("Source: " + oTestArray.getHexString());
      CCDEBUG("Output: " + oBase58Encoded.getHexString());
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool CStringTest::testStringConversions()
{
  const char pcTestArray[] = ("\17\103\103\0\1\2\3\16\27\100\377");
  const char* pcTestArrayResult = ("\\17\\103\\103\\0\\1\\2\\3\\16\\27\\100\\377");
  bool bSuccess = true;
  CcByteArray oArray(pcTestArray, sizeof(pcTestArray)-1);
  CcString sTest = oArray.getOctalString();
  if (sTest != pcTestArrayResult)
  {
    bSuccess = false;
  }
  return bSuccess;
}

bool CStringTest::testInteger()
{ 
  bool bRet = false;
  if (CcString("-1").toInt8()  == static_cast<int8>(-1) &&
      CcString("-1").toInt16() == static_cast<int16>(-1) &&
      CcString("-1").toInt32() == static_cast<int32>(-1) &&
      CcString("-1").toInt64() == static_cast<int64>(-1) ) 
  {
    if (CcString("127").toInt8() == static_cast<int8>(127) &&
        CcString("255").toInt16() == static_cast<int16>(255) &&
        CcString("255").toInt32() == static_cast<int32>(255) &&
        CcString("255").toInt64() == static_cast<int64>(255))
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CStringTest::testUnsignedInteger()
{
  bool bRet = false;
  if (CcString("1").toUint8() == static_cast<uint8>(1) &&
      CcString("1").toUint16() == static_cast<uint16>(1) &&
      CcString("1").toUint32() == static_cast<uint32>(1) &&
      CcString("1").toUint64() == static_cast<uint64>(1))
  {
    if (CcString("128").toUint8() == static_cast<uint8>(128) &&
        CcString("255").toUint16() == static_cast<uint16>(255) &&
        CcString("255").toUint32() == static_cast<uint32>(255) &&
        CcString("255").toUint64() == static_cast<uint64>(255))
    {
      if (CcString("0x128").toUint16() == static_cast<uint16>(0x128) &&
          CcString("0xffffffffffffffff").toUint64() == static_cast<uint64>(0xffffffffffffffff))
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CStringTest::testFloatAndDouble()
{
  bool bSuccess = false;
  CcString sFloat1("10.1");
  float f1 = sFloat1.toFloat();
  float f1Test = 10.1f;
  if (f1 == f1Test)
  {
    CcString sFloat1Conv = CcString::fromNumber(f1);
    CcString sFloat2("22.442e2");
    float f2 = sFloat2.toFloat();
    float f2Test = 22.442e2f;
    if (f2 == f2Test)
    {
      CcString sFloat2Conv = CcString::fromNumber(f2);
      bSuccess = true;
    }
  }
  return bSuccess;
}

bool CStringTest::testPaths()
{
  bool bRet = false;
  CcString sBasePath("/temp/test");
  CcString sFirstPath(sBasePath);
  sFirstPath.appendPath("0");
  if (sFirstPath == "/temp/test/0")
  {
    CcString sPath2(sBasePath);
    sPath2.appendPath("/2");
    if (sPath2 == "/temp/test/2")
    {
      CcString sPath3(sBasePath);
      sPath3.appendPath("/3/");
      if (sPath3 == "/temp/test/3")
      {
        CcString sPath4(sFirstPath);
        sPath3.appendPath("/../");
        if (sPath3 == "/temp/test/3/..")
        {
          sPath3.normalizePath();
          if (sPath3 == "/temp/test")
          {
            bRet = true;
          }
        }
      }
    }
  }
  return bRet;
}
