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
 * @brief     Class CcIniFile
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcMap.h"

class IIo;

/**
 * @brief Ini file format reader and writer
 */
class CcKernelSHARED  CcIniFile
{
public:
  class CcKernelSHARED CIniLine
  {
  public:
    CcString sPreKey;
    CcString sKey;
    CcString sPostKey;
    CcString sPreValue;
    CcString sValue;
    CcString sPostValue;
    char     cCommentSign;
    CcString sComment;
  };

  typedef CcList<CIniLine> CSection;
  #ifdef _MSC_VER
    class CcKernelSHARED CSection;
  #endif
  typedef CcMap<CIniLine,CSection> CSectionMap;
  #ifdef _MSC_VER
    class CcKernelSHARED CSectionMap;
  #endif

  /**
   * @brief Constructor
   */
  CcIniFile() = default;

  /**
   * @brief Destructor
   */
  ~CcIniFile() = default;

  CcStatus readFile(const CcString& sPath);

  CcStatus readStream(IIo& oStream);

private:
  CcStatus addSection(const CcString& sLine);
  CcStatus addLine(const CcString& sLine);

private:
  CSectionMap m_oSections;
};
