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
 * @brief     Implemtation of class CHmacTest
 */
#include "CHmacTest.h"
#include "Hash/CcHmac.h"
#include "CcStringUtil.h"
#include "CcByteArray.h"

//! Define a test url to be parsed successfully
#define TESTURL "https://username:password@hostname.dom:80/Path/to"
//! Create Test url string
const CcString sTestUrl(TESTURL);

CHmacTest::CHmacTest() :
  CcTest("CHmacTest")
{
  appendTestMethod("Test sha256 algorithm", &CHmacTest::testSha256);
}

CHmacTest::~CHmacTest()
{
}

const CcString c_sTestString1("Hallo");                                           //!< Test data from SslHmac
const CcString c_sTestSecret1("0");                                               //!< Test data from SslHmac
const CcString c_sTestResult1("927d1f23f16ae8ee5258a26e653f825632758ea7");        //!< Test data from SslHmac

bool CHmacTest::testSha256()
{
  bool bSuccess = false;
  CcHmac oSha256(EHashType::Sha256);
  oSha256.setSecretKey(c_sTestSecret1);
  oSha256.generateByteArray(c_sTestString1);
  if (oSha256.getHexString() != c_sTestResult1)
  {
    bSuccess = false;
  }
  return bSuccess;
}
