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
#include "CcList.h"
#include "CcConsole.h"
#include "CcString.h"

/**
 * @brief Class impelmentation
 */
template <class C>
class CcTest : public ITest
{
public:
  typedef bool (C::*FTestMethod)(void);
  typedef struct
  {
    CcString sTestName;
    FTestMethod oTestMethod;
  } STestMethod;

  /**
   * @brief Constructor
   */
  CcTest( void )
    {}

  /**
   * @brief Destructor
   */
  virtual ~CcTest( void )
    {}

  virtual bool test() override
  {
    bool bSuccess = true;
    for (STestMethod& rMethod : m_oTestList)
    {
      if(rMethod.sTestName.length() > 0)
        CcConsole::writeLine("Start Test: " + rMethod.sTestName);
      bSuccess &= testMethod(rMethod.oTestMethod);
      if (bSuccess)
      {
        if (rMethod.sTestName.length() > 0)
          CcConsole::writeLine("Test succeeded: " + rMethod.sTestName);
      }
      else
      {
        if (rMethod.sTestName.length() > 0)
          CcConsole::writeLine("Test failed: " + rMethod.sTestName);
        break;
      }
    }
    return bSuccess;
  }

  void appendTestMethod(const CcString& sName, FTestMethod oMethod)
  {
    STestMethod oTestMethod = { sName, oMethod };
    m_oTestList.append(oTestMethod);
  }

  bool testMethod(FTestMethod oTestMethod)
  {
    if(isOk()) if(!(((C*)this)->*oTestMethod)()) this->setFailed();
    return isOk();
  }

  bool isOk()
    {return m_bCurrentState;}
  void setFailed()
    {m_bCurrentState = false;}

private:
  bool                 m_bCurrentState = true;
  CcList<STestMethod> m_oTestList;
  
};

#endif /* _CcTest_H_ */
