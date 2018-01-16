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
 * @subpage   CHashTest
 *
 * @page      CHashTest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CHashTest
 **/
#ifndef _CHashTest_H_
#define _CHashTest_H_

#include "CcBase.h"

/**
 * @brief Class impelmentation
 */
class CHashTest 
{
public:
  /**
   * @brief Constructor
   */
  CHashTest( void );

  /**
   * @brief Destructor
   */
  ~CHashTest( void );

  bool test();

private:
  bool testSha256();
  bool testMd5();
  bool testMd5Append();
  bool testCcHash();
};

#endif /* _CHashTest_H_ */
