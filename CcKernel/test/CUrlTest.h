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
 * @subpage   CUrlTest
 *
 * @page      CUrlTest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CUrlTest
 **/
#ifndef _CUrlTest_H_
#define _CUrlTest_H_

#include "CcBase.h"

/**
 * @brief Class impelmentation
 */
class CUrlTest {
public:
  /**
   * @brief Constructor
   */
  CUrlTest();

  /**
   * @brief Destructor
   */
  ~CUrlTest();

  bool test();

private:
  bool testConstructors();
  bool testExampleUrl();
};

#endif /* _CUrlTest_H_ */
