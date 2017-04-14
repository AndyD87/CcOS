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
 * @page      CJsonTest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CJsonTest
 **/
#ifndef CJsonTest_H_
#define CJsonTest_H_

#include "CcBase.h"

/**
 * @brief Class impelmentation
 */
class CJsonTest 
{
public:
  /**
   * @brief Constructor
   */
  CJsonTest( void );

  /**
   * @brief Destructor
   */
  virtual ~CJsonTest( void );

  bool test();

private:
  bool JsonToCompact();
  bool JsonAppendMove();
  bool JsonBugNr1();
};

#endif /* CJsonTest_H_ */
