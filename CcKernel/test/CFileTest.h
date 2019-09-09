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
 * @subpage   CFileTest
 *
 * @page      CFileTest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CFileTest
 **/
#ifndef H_CFileTest_H_
#define H_CFileTest_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcFile.h"
#include "CcTest.h"

/**
 * @brief Class implementation
 */
class CFileTest : public CcTest<CFileTest>
{
public:
  /**
   * @brief Constructor
   */
  CFileTest();

  /**
   * @brief Destructor
   */
  ~CFileTest();

private:
  bool createTestFile();
  bool removeTestFile();
  CcFile getTestFile(EOpenFlags eFlags);

  bool testBasics();
  bool crcTest();
  bool testCopyFile();
  bool testMoveFile();
  bool testAppendFile();
  bool testUserId();
  bool testGroupId();
  bool testAttributes();
  bool testDirectoryCreate();
  bool testDirectoryRemove();
  /**
   * @brief Once a app crashed because creating full dir path "TestDir" with Recursive
   *        flag on.
   * @return true if this bug is solved.
   */
  bool testDirectoryCreateBug();

#ifdef WINDOWS
  bool testWindows();
#endif

  const static CcString c_sTestFileName;
  static CcString s_sTestFilePath;
  static CcString s_sTestDirPath;
};

#endif // H_CFileTest_H_
