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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CStringTest
 */
#include "CStringTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"
#include "CcConsole.h"

CStringTest::CStringTest( void )
{
}

CStringTest::~CStringTest( void )
{
}

bool CStringTest::test()
{
  bool bSuccess = true;
  bSuccess &= test1();
  bSuccess &= baseEncodings();
  bSuccess &= testStringConversions();
  if(!bSuccess)
  {
    CcConsole::writeLine("CStringTest failed");
  }
  return bSuccess;
}

bool CStringTest::test1()
{
  bool bRet = true;
  return bRet;
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
