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
 * @subpage   CImagePpmTest
 *
 * @page      CImagePpmTest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CImagePpmTest
 **/
#ifndef H_CImagePpmTest_H_
#define H_CImagePpmTest_H_

#include "CcBase.h"
#include "CcTest.h"

/**
 * @brief Class implementation
 */
class CImagePpmTest : public CcTest<CImagePpmTest>
{
public:
  /**
   * @brief Constructor
   */
  CImagePpmTest();

  /**
   * @brief Destructor
   */
  virtual ~CImagePpmTest();
private:
  bool testBasic();
  bool testConvertP1();
  bool testConvertP2();
  bool testConvertP3();
  bool testConvertP5();
  bool testConvertP6();
  bool testConvertP6GrayFail();
};

#endif // H_CImagePpmTest_H_
