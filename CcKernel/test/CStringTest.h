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
 * @page      Test
 * @subpage   CStringTest
 *
 * @page      CStringTest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CStringTest
 **/
#ifndef H_CStringTest_H_
#define H_CStringTest_H_

#include "CcBase.h"
#include "CcTest.h"

/**
 * @brief Class implementation
 */
class CStringTest : public CcTest<CStringTest>
{
public:
  /**
   * @brief Constructor
   */
  CStringTest();

  /**
   * @brief Destructor
   */
  virtual ~CStringTest();

private:
  bool test1();
  bool baseEncodings();
  bool base64Known();
  bool testStringConversions();
  bool testInteger();
  bool testUnsignedInteger();
  bool testFloatAndDouble();
  bool testPaths();
  bool testOsPath();
  bool testEndsWith();
  bool testStartsWith();
  bool testPathNormalizing();
  /**
   * @brief Testing a bug where CcString("uu") == "u" was true
   * @return
   */
  bool testCompareShort();
};

#endif // H_CStringTest_H_
