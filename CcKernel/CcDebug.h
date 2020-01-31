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
 * @subpage   CcDebug
 *
 * @page      CcDebug
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDebug
 */
#ifndef H_CcDebug_H_
#define H_CcDebug_H_

//! Forward Declaration
#include "CcBase.h"
class CcString;

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */
class CcKernelSHARED CcDebug
{
public:
  enum class ELevel : uint8
  {
    None = 0,
    Error,
    Warning,
    Info,
    Debug,
    Verbose,
  };

  static void write(const CcString& sMsg);
  static void writeDebug(const CcString& sMsg);
  static void writeVerbose(const CcString& sMsg);
  static void writeInfo(const CcString& sMsg);
  static void writeWarning(const CcString& sMsg);
  static void writeError(const CcString& sMsg);

  static void setLevel(ELevel eLevel);
  static void setVerbose(bool bEnable);
private:
  CcDebug();
  ~CcDebug();

  static ELevel c_eLevel;
  static bool c_bVerboseEnabled;
};

#endif // H_CcDebug_H_
