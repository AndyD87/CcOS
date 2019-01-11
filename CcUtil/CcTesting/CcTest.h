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
 * @subpage   CcTest
 *
 * @page      CcTest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTest
 **/
#ifndef _CcTest_H_
#define _CcTest_H_

#include "CcTesting.h"
#include "ITest.h"

/**
 * @brief Class impelmentation
 */
template <class C>
class CcTestingSHARED CcTest : public ITest
{
public:
  /**
   * @brief Constructor
   */
  CcTest( void )
  {
  }

  /**
   * @brief Destructor
   */
  virtual ~CcTest( void )
  {
  }

  bool testMethod(bool (C::*oTestMEthod)(void))
  {
    if(isOk()) if(!(((C*)this)->*oTestMEthod)()) this->setFailed();
    return isOk();
  }

  bool isOk()
    {return m_bCurrentState;}
  void setFailed()
    {m_bCurrentState = false;}

private:
  bool m_bCurrentState = true;
};

#endif /* _CcTest_H_ */
