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
 * @brief     Implemtation of class CBitcoinClientTest
 */
#include "CBitcoinClientTest.h"
#include "CcKernel.h"
#include "CcString.h"
#include "CcBitcoinAddress.h"
#include "CcBitcoinClient.h"
#include "Network/CcCommonPorts.h"
#include "CcFile.h"

CBitcoinClientTest::CBitcoinClientTest() :
  CcTest<CBitcoinClientTest>("CBitcoinClientTest")
{
  if(CcFile::exists("bitcoin-cli"))
  {
    appendTestMethod("Test if CLI access is available", &CBitcoinClientTest::testCli);
  }
  appendTestMethod("Test client can be executed", &CBitcoinClientTest::testClient);
}

CBitcoinClientTest::~CBitcoinClientTest()
{
  CCDEBUG("Remove CBitcoinClientTest");
}

bool CBitcoinClientTest::testCli()
{
  bool bSuccess = true;
  return bSuccess;
}

bool CBitcoinClientTest::testClient()
{
  bool bSuccess = true;
  CcBitcoinClient oClient;
  oClient.setHost("http://localhost:" + CcString::fromNumber(CcCommonPorts::BITCOIN_RPC));
  CCDEBUG(oClient.getWork());
  return bSuccess;
}
