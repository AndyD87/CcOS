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
 * @brief     Implemtation of class CBitcoinAddressTest
 */
#include "CBitcoinAddressTest.h"
#include "CcKernel.h"
#include "CcString.h"
#include "CcBitcoinAddress.h"

CBitcoinAddressTest::CBitcoinAddressTest(void )
{
  appendTestMethod("check generation of bitcoin address", &CBitcoinAddressTest::testAdressGeneration);
}

CBitcoinAddressTest::~CBitcoinAddressTest(void )
{
}

bool CBitcoinAddressTest::testAdressGeneration()
{
  bool bSuccess = true;
  CcBitcoinAddress oAddress1("0000000000000000000000000000000000000000");
  CcBitcoinAddress oAddress2("1111111111111111111114oLvT2");
  if (oAddress1.getAddress() != oAddress2.getAddress())
  {
    bSuccess = false;
  }
  return bSuccess;
}
