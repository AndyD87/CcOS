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
 * @brief     Class CcTestUtility
 **/
#pragma once

#include "CcBase.h"
#include "CcTest.h"

class CcStatus;
class CcString;
class IProgressReceiver;

/**
 * @brief CcTestUtility is used to used to test if loading libraries is working.
 */
class CcTestingSHARED CcTestUtility
{
public:
  /**
   * @brief Create file by path and write a test pattern to file.
   *        Pattern will be increased by every write step.
   *        File will be closed and opened again and patterns are read back to verify the content.
   * @param sPath:      Path to File to test
   * @param uiSize:     Size of target file
   * @param uiPattern:  Pattern start value
   * @param pProgress:  Optional progress receiver, or null if not wanted.
   * @return Status of operation.
   */
  static CcStatus generateAndVerifyFile(const CcString& sPath, uint64 uiSize, uint64 uiPattern = 0, IProgressReceiver *pProgress = nullptr);

private:
  CcTestUtility() = delete;
  virtual ~CcTestUtility() = delete;
};
