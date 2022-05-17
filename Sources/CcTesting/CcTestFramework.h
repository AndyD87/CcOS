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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTestFramework
 **/
#pragma once

#include "CcBase.h"
#include "CcTesting.h"
#include "CcSharedPointer.h"
#include "ITest.h"

/**
 * @brief Create test makro for an CcTest inheriting class.
 *        It will add to Test to CcTestFramework
 */
#define CcTestFramework_addTest(TestClass) CcTestFramework::addTest(TestClass::create)

/**
 * @brief CcTestFramework is used to setup a working environment for all tests.
 *        It can setup test directories, and can be used for a common output format for messages.
 */
class CcTestingSHARED CcTestFramework
{
public:
  //! Function type to call for testing
  typedef ITest*(*FTestCreate)(void*);

  /**
   * @brief Init TestFramework with arguments from command line
   * @param iArgc:  Number of arguments in ppArgv
   * @param ppArgv: Arguments
   * @return True if init succeeded
   */
  static bool init(int iArgc, char** ppArgv);

  /**
   * @brief Deinit TestFramework and cleanup test
   * @param bStopKernel: Stop CcKernel too
   * @return Exit code for Applications with 0 on success.
   */
  static int deinit(bool bStopKernel = false);

  /**
   * @brief If this method is called, all addTest() will
   *        be executed instantly
   */
  static void setInstantStart();

  /**
   * @brief Write debug message to test framework.
   *        Depending on settings, it will be printed to stdout, logged, ignored, etc.
   * @param sMessage: Messag for TestFramework
   */
  static void writeDebug(const CcString& sMessage);

  /**
   * @brief Write info message to test framework.
   *        Depending on settings, it will be printed to stdout, logged, ignored, etc.
   * @param sMessage: Messag for TestFramework
   */
  static void writeInfo(const CcString& sMessage);

  /**
   * @brief Write error message to test framework.
   *        Depending on settings, it will be printed to stdout, logged, ignored, etc.
   * @param sMessage: Messag for TestFramework
   */
  static void writeError(const CcString& sMessage);

  /**
   * @brief Enable log file
   * @param sPathToLogFile: Path to log to write on execution
   * @return True if log was created and opend
   */
  static bool enableLog(const CcString& sPathToLogFile);

  //! @return Get binary dir for testing other executables
  static CcString getBinaryDir();
  //! @return Generate a temporary directory for testing
  static const CcString& getTemporaryDir();
  //! @brief Remove current set temporary directory and cleanup
  static void removeTemporaryDir();
  //! @brief Cleanup current temporary directory
  static const CcString& regenerateTemporaryDir();

  //! @param fTestCreate: Add CcTest\<\> class to framework
  static void addTest(FTestCreate fTestCreate);
  //! @return Start all test and return true if all tests was running successfully.
  //!         If false, tests where stopped on first error.
  static bool runTests();

private: // Methods
  /**
   * @brief Constructor
   */
  CcTestFramework() = default;

  /**
   * @brief Destructor
   */
  virtual ~CcTestFramework() = default;

  static void writeLine(const CcString& sMessage);
  static bool runTest(FTestCreate fTestCreate);

public: // Types
  class CPrivate;
private: // Methods
  static CPrivate& getPrivate();
private: // Member
  static CPrivate* s_pPrivate;
  static bool s_bSuccess;
  static bool s_bInstantStart;
};
