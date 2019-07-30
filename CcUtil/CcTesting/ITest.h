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
 * @page      CcTesting
 * @subpage   ITest
 *
 * @page      ITest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ITest
 **/
#ifndef _ITest_H_
#define _ITest_H_

#include "CcTesting.h"
#include "CcSharedPointer.h"

class CcString;

/**
 * @brief Class implementation
 */
class CcTestingSHARED ITest 
{
public:
  /**
   * @brief Constructor
   */
  ITest() = default;

  /**
   * @brief Destructor
   */
  virtual ~ITest() = default;

  virtual bool test() = 0;
  virtual const CcString& getName() = 0;
};

#endif /* _ITest_H_ */
