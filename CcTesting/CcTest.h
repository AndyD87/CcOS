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
 * @brief     Class CcTest
 **/
#pragma once

#include "CcTesting.h"
#include "ITest.h"
#include "CcTestFramework.h"
#include "CcList.h"
#include "CcString.h"
#include "CcSharedPointer.h"
#include "CcStringList.h"

/**
 * @brief Class implementation
 */
template <class C>
class CcTest : public ITest
{
public:
  //! Function type of test method
  typedef bool (C::*FTestMethod)();

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
  virtual ~CcTest() = default;

  /**
   * @brief Overwrite test of interface to execute all stored test in m_oMethodList
   * @return True if all tests succeded, or False on first failed test.
   */
  virtual bool test() override final
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

  /**
   * @brief Create or Remove test instance from inheriting class
   * @param pCreate: If nullptr, new Test will be generated, otherwise the Test will be deleted
   * @return Pointer to new test if created, or nullptr
   */
  static ITest* create(void* pCreate)
  {
    if (pCreate)
    {
      ITest* pTest = static_cast<ITest*>(pCreate);
      CCDELETE(pTest);
      return nullptr;
    }
    else
    {
      CCNEWTYPE(iTest, C);
      return iTest;
    }
  }

  //! @return Get initialized name
  const CcString& getName() override
  { return m_sName; }

  /**
   * @brief Append an method to test
   * @param sName:    Name of method for log
   * @param oMethod:  Pointer to method
   */
  void appendTestMethod(const CcString& sName, FTestMethod oMethod)
  {
    m_oNameList.append(sName);
    m_oMethodList.append(oMethod);
  }

  /**
   * @brief Call a test method and writ result back.
   * @param oTestMethod: Method to call
   * @return True if test succeded.
   */
  bool testMethod(FTestMethod oTestMethod)
  {
    if(isOk()) if(!(((C*)this)->*oTestMethod)()) this->setFailed();
    return isOk();
  }

  //! @return True if current state of all tests is true
  bool isOk()
  { return m_bCurrentState;}
  //! @brief Set state of curren test to false
  void setFailed()
  { m_bCurrentState = false;}

private:
  bool                m_bCurrentState = true;
  CcStringList        m_oNameList;
  CcList<FTestMethod> m_oMethodList;
  CcString            m_sName;
};
