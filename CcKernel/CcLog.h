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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcLog
 */
#ifndef CcLog_H_
#define CcLog_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcFile.h"

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
class CcKernelSHARED CcLog {
public:
  CcLog(const CcString& sOutputFile);
  virtual ~CcLog();

  void write(const CcString& sMsg);
  void writeLine(const CcString& sMsg);
  void writeDebug(const CcString& sMsg);
  void writeVerbose(const CcString& sMsg);
  void writeInfo(const CcString& sMsg);
  void writeWarning(const CcString& sMsg);
  void writeError(const CcString& sMsg);

private:
  bool  m_bFileValid = false;
  CcFile m_oOutputFile;
};

#endif /* CcLog_H_ */
