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
 * @brief     Class CcMessageFormat
 **/
#pragma once

#include "CcBase.h"
#include "CcString.h"

/**
 * @brief Class for parsing and working with arguments from a commandline
 */
class CcKernelSHARED CcMessageFormat
{
public:
  CcMessageFormat() = delete;

  /**
   * @brief Format a debug message for log
   * @param sMsg: Messag to write
   */
  static CcString formatDebugMessage(const CcString& sMsg);

  /**
   * @brief Format a verbose message for log
   * @param sMsg: Messag to write
   */
  static CcString formatVerboseMessage(const CcString& sMsg);

  /**
   * @brief Format a info message for log
   * @param sMsg: Messag to write
   */
  static CcString formatInfoMessage(const CcString& sMsg);

  /**
   * @brief Format a warning message for log
   * @param sMsg: Messag to write
   */
  static CcString formatWarningMessage(const CcString& sMsg);

  /**
   * @brief Format a error message for log
   * @param sMsg: Messag to write
   */
  static CcString formatErrorMessage(const CcString& sMsg);

};
