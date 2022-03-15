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
 * @brief     Implemtation of class CSslHmacTest
 */
#include "CSslHmacTest.h"
#include "CcKernel.h"
#include "CcString.h"
#include "Hash/CcSslHmac.h"

CSslHmacTest::CSslHmacTest() :
  CcTest("CSslHmacTest")
{
  appendTestMethod("Test HMAC with SHA1", &CSslHmacTest::testHmac);
}

CSslHmacTest::~CSslHmacTest()
{
}

const CcString c_sTestString1("Hallo");                                           //!< Test data from SslHmac
const CcString c_sTestSecret1("0");                                               //!< Test data from SslHmac
const CcString c_sTestResult1("927d1f23f16ae8ee5258a26e653f825632758ea7");        //!< Test data from SslHmac
const ESslHashAlgoritm c_eTestAlgorithm1(ESslHashAlgoritm::SHA1);                 //!< Test data from SslHmac

bool CSslHmacTest::testHmac()
{
  bool bSuccess = true;
  CcSslHmac oHmac;
  oHmac.setSecretKey(c_sTestSecret1);
  oHmac.setHashAlgorithm(c_eTestAlgorithm1);
  oHmac.generateByteArray(c_sTestString1);
  if (oHmac.getHexString() != c_sTestResult1)
  {
    bSuccess = false;
  }
  return bSuccess;
}
