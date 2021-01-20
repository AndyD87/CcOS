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
 * @brief     Class CcFileInfoList
 */
#pragma once

#include "CcBase.h"
#include "CcFileInfo.h"
#include "CcList.h"

enum class EFileInfoListFormats : uint8
{
  NamesOnly   = 0,
  Hidden      = 1,
  ExtendedLs  = 2
};

/**
 * @brief Handles all devices and Interfaces connected to Kernel
 */
class CcKernelSHARED CcFileInfoList : public CcList<CcFileInfo>
{
public:
  /**
   * @brief Constructor
   */
  CcFileInfoList() = default;

  /**
   * @brief Destructor
   */
  ~CcFileInfoList() = default;

  bool containsFile(const CcString& sName) const;
  inline bool containsDirectory(const CcString& sDirectoryName) const
    { return containsFile(sDirectoryName);}
  CcFileInfo& getFile(const CcString& sFilename);
  const CcFileInfo& getFile(const CcString& sFilename) const;
  inline const CcFileInfo& getDirectory(const CcString& sDirectoryName) const
    { return getFile(sDirectoryName); }
  inline CcFileInfo& getDirectory(const CcString& sDirectoryName)
    { return getFile(sDirectoryName); }
  bool removeFile(const CcString& sFilename);

  CcStringList getFormatedList(EFileInfoListFormats uiShowFlags) const;
};
