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
 * @brief     Class CcDebug
 */
#pragma once

//! Forward Declaration
#include "CcBase.h"
class CcString;

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
class CcKernelSHARED CcDebug
{
public:
  //! @brief Level of debug output
  enum class ELevel : uint8
  {
    None = 0,
    Error,
    Warning,
    Info,
    Debug,
    Verbose,
  };

  /**
   * @brief Write formated to output
   * @param sMsg
   */
  static void write(const CcString& sMsg);

  /**
   * @brief Write [dbg ] message to output
   * @param sMsg: message to write
   */
  static void writeDebug(const CcString& sMsg);

  /**
   * @brief Write [vbs ] message to output
   * @param sMsg: message to write
   */
  static void writeVerbose(const CcString& sMsg);
  /**
   * @brief Write [info] message to output
   * @param sMsg: message to write
   */
  static void writeInfo(const CcString& sMsg);

  /**
   * @brief Write [warn] message to output
   * @param sMsg: message to write
   */
  static void writeWarning(const CcString& sMsg);

  /**
   * @brief Write [err ] message to output
   * @param sMsg: message to write
   */
  static void writeError(const CcString& sMsg);

  //! @param eLevel: Set this as fileter level
  static void setLevel(ELevel eLevel);

  //! @param bEnable: Enable verbose output with true.
  static void setVerbose(bool bEnable);
private:
  CcDebug() = delete;
  ~CcDebug() = delete;

  static ELevel c_eLevel;
  static bool c_bVerboseEnabled;
};
