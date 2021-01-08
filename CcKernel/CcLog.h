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
 * @page      CcKernel
 * @subpage   CcLog
 *
 * @page      CcLog
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLog
 */
#ifndef H_CcLog_H_
#define H_CcLog_H_

#include "CcBase.h"
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
  CcLog(const CcString& sOutputFile);
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


  bool setFilePath(const CcString& sOutputFile);

  void write(const CcString& sMsg);
  void writeLine(const CcString& sMsg);
  void writeDebug(const CcString& sMsg);
  void writeVerbose(const CcString& sMsg);
  void writeInfo(const CcString& sMsg);
  void writeWarning(const CcString& sMsg);
  void writeError(const CcString& sMsg);

  static CcString formatDebugMessage(const CcString& sMsg);
  static CcString formatVerboseMessage(const CcString& sMsg);
  static CcString formatInfoMessage(const CcString& sMsg);
  static CcString formatWarningMessage(const CcString& sMsg);
  static CcString formatErrorMessage(const CcString& sMsg);

  void setConsoleOutput(bool bEnable)
  { m_bWriteToConsole = bEnable; }
private:
  bool  m_bFileValid = false;
  bool m_bWriteToConsole = false;
  CcFile m_oOutputFile;
};

#endif // H_CcLog_H_
