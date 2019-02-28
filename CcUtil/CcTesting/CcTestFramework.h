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
 * @page      CcUtil
 * @subpage   CcTestFramework
 *
 * @page      CcTestFramework
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTestFramework
 **/
#ifndef _CcTestFramework_H_
#define _CcTestFramework_H_

#include "CcBase.h"
#include "CcTesting.h"
#include "CcSharedPointer.h"
#include "ITest.h"

class CcTestFrameworkPrivate;

#define CcTestFramework_addTest(TestClass) CcTestFramework::addTest(TestClass::create)

/**
 * @brief CcTestFramework is used to setup a working environment for all tests.
 *        It can setup test directories, and can be used for a common output format for messages.
 */
class CcTestingSHARED CcTestFramework
{
public:
  typedef ITest*(*FTestCreate)();

  static CcTestFrameworkPrivate& getPrivate();
  static bool init(int iArgc, char** ppArgv);
  static int deinit();

  static void writeDebug(const CcString& sMessage);
  static void writeInfo(const CcString& sMessage);
  static void writeError(const CcString& sMessage);

  static bool enableLog(const CcString& sPathToLogFile);

  static CcString getBinaryDir();
  static const CcString& getTemporaryDir();
  static void removeTemporaryDir();
  static const CcString& regenerateTemporaryDir();

  static void addTest(FTestCreate fTestCreate);
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

private: // Member
  static CcTestFrameworkPrivate* s_pPrivate;
  static bool m_bSuccess;
};

#endif /* _CcTestFramework_H_ */
