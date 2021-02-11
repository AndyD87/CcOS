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
 * @brief     Class CcLog
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcFile.h"
#include "IIo.h"

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
class CcKernelSHARED CcLog : public IIo
{
public:
  CcLog() = default;
  /**
   * @brief Initialize log with output file
   * @param sOutputFile
   */
  CcLog(const CcString& sOutputFile);

  /**
   * @brief Cleanup and close file
   */
  ~CcLog();

  virtual size_t read(void* pBuffer, size_t uSize) override
  {CCUNUSED(pBuffer); CCUNUSED(uSize); return SIZE_MAX; }
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags) override
  { return EStatus::NotSupported; }
  virtual CcStatus close() override
  { return EStatus::NotSupported; }
  virtual CcStatus cancel() override
  { return EStatus::NotSupported; }

  /**
   * @brief Set file path and open file
   * @param sOutputFile: Path to file for log
   * @return True if path changed and file is opened
   */
  bool setFilePath(const CcString& sOutputFile);

  /**
   * @brief Write a message to log without format
   * @param sMsg: Messag to write
   */
  void write(const CcString& sMsg);

  /**
   * @brief Write a message to log and go to next line
   * @param sMsg: Messag to write
   */
  void writeLine(const CcString& sMsg);

  /**
   * @brief Write a debug message to log and go to next line
   * @param sMsg: Messag to write
   */
  void writeDebug(const CcString& sMsg);

  /**
   * @brief Write a verbos message to log and go to next line
   * @param sMsg: Messag to write
   */
  void writeVerbose(const CcString& sMsg);

  /**
   * @brief Write a info message to log and go to next line
   * @param sMsg: Messag to write
   */
  void writeInfo(const CcString& sMsg);

  /**
   * @brief Write a warning message to log and go to next line
   * @param sMsg: Messag to write
   */
  void writeWarning(const CcString& sMsg);

  /**
   * @brief Write a error message to log and go to next line
   * @param sMsg: Messag to write
   */
  void writeError(const CcString& sMsg);

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

  /**
   * @brief Enable or disable console output. So every log message can be displayed
   *        on output too.
   * @param bEnable: True for enable output to console
   */
  void setConsoleOutput(bool bEnable)
  { m_bWriteToConsole = bEnable; }
private:
  bool  m_bFileValid = false;
  bool m_bWriteToConsole = false;
  CcFile m_oOutputFile;
};
