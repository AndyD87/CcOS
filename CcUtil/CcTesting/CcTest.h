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
#include "CcTestFramework.h"
#include "CcList.h"
#include "CcString.h"
#include "CcSharedPointer.h"
#include "CcStringList.h"

/**
 * @brief Class impelmentation
 */
template <class C>
class CcTest : public ITest
{
public:
  typedef bool (C::*FTestMethod)(void);

  /**
   * @brief Constructor
   * @param sName: Set name of test wich will increase the readability of output
   */
  CcTest( const CcString& sName = "" ) :
    m_sName(sName)
    {}

  /**
   * @brief Destructor
   */
  virtual ~CcTest( void )
    {}

  virtual bool test() override
  {
    bool bSuccess = true;
    size_t i = 0;
    for (CcString& sTestName : m_oNameList)
    {
      if(sTestName.length() > 0)
        CcTestFramework::writeInfo("  Start Method: " + sTestName);
      bSuccess &= testMethod((FTestMethod) m_oMethodList[i]);
      if (bSuccess)
      {
        if (sTestName.length() > 0)
          CcTestFramework::writeInfo("  Method succeeded: " + sTestName);
      }
      else
      {
        if (sTestName.length() > 0)
          CcTestFramework::writeInfo("  Method failed: " + sTestName);
        break;
      }
      i++;
    }
    return bSuccess;
  }

  static ITest* create()
  {
    ITest* iTest = static_cast<ITest*>(new C());
    CCMONITORNEW(iTest);
    return iTest;
  }

  const CcString& getName()
    { return m_sName; }

  void appendTestMethod(const CcString& sName, FTestMethod oMethod)
  {
    m_oNameList.append(sName);
    m_oMethodList.append(oMethod);
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
  bool                m_bCurrentState = true;
  CcStringList        m_oNameList;
  CcList<FTestMethod> m_oMethodList;
  CcString            m_sName;
};

#endif /* _CcTest_H_ */
