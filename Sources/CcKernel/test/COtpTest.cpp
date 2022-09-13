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
 * @brief     Implemtation of class COtpTest
 */
#include "COtpTest.h"
#include "Hash/CcOtp.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"

//! Define a test url to be parsed successfully
#define TESTURL "https://username:password@hostname.dom:80/Path/to"
//! Create Test url string
const CcString sTestUrl(TESTURL);

COtpTest::COtpTest() :
  CcTest("COtpTest")
{
  appendTestMethod("Test basic", &COtpTest::testBasic);
}

COtpTest::~COtpTest()
{
}

bool COtpTest::testBasic()
{
  bool bSuccess = false;
  CcOtp oOtpTest;
  CcString oTestString("TestKeyGenerate");
  uint64 uiCounter = 12345678;
  CcTestFramework::writeInfo("KeyStrValue: " + oTestString);
  CcTestFramework::writeInfo("KeyHexValue: " + oTestString.getByteArray().getHexString());
  CcTestFramework::writeInfo("Counter:     " + CcString::fromNumber(uiCounter));
  oOtpTest.setKey(oTestString.getCharString(), oTestString.length());
  oOtpTest.finalizeCounter(uiCounter);
  CcString sResult6 = oOtpTest.getDigits(6);
  CcString sResult8 = oOtpTest.getDigits(8);
  CcTestFramework::writeInfo("Result6dig:  " + sResult6);
  CcTestFramework::writeInfo("Result8dig:  " + sResult8);
  if (sResult6 == "627585" && sResult8 == "61627585")
  {
    bSuccess = true;
  }
  else
  {
    CcTestFramework::writeError("Target6dig:  627585");
    CcTestFramework::writeError("Target8dig:  61627585");
  }
  return bSuccess;
}
