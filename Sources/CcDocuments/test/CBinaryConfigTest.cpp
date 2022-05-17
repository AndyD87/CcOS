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
 * @brief     Implemtation of class CBinaryConfigTest
 */
#include "CBinaryConfigTest.h"
#include "CcKernel.h"
#include "Config/CcConfigBinary.h"

CBinaryConfigTest::CBinaryConfigTest() :
  CcTest<CBinaryConfigTest>("CBinaryConfigTest")
{
  appendTestMethod("Test if positions and ids are matching", &CBinaryConfigTest::testIdsAndPostions);
}

CBinaryConfigTest::~CBinaryConfigTest()
{
}

bool CBinaryConfigTest::testIdsAndPostions()
{
  bool bSuccess = true;
  for(size_t uiI = 0; uiI < CcConfigBinary::CItem::knownListGetSize(); uiI++)
  {
    bSuccess &= static_cast<CcConfigBinary::EType>(uiI) == CcConfigBinary::CItem::knownListGetType(uiI);
    if(!bSuccess) CcTestFramework::writeError("Failed on checking type at: " + CcString::fromSize(uiI));
  }
  return bSuccess;
}
