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

/**
 * @brief Enum for list details of file informations
 */
enum class EFileInfoListFormats : uint8
{
  NamesOnly   = 0,
  Hidden      = 1,
  ExtendedLs  = 2
};

/**
 * @brief List of file informations
 */
class CcKernelSHARED CcFileInfoList : public CcList<CcFileInfo>
{
public:
  CcFileInfoList() = default;
  ~CcFileInfoList() = default;

  /**
   * @brief Check if specific file is in list
   * @param sName: Name of file to search for
   * @return True if file was found
   */
  bool containsFile(const CcString& sName) const;

  /**
   * @brief Check if specific directory is in list.
   *        Directory is defined as file, so file would return True too.
   * @param sDirectoryName: Name of file to search for
   * @return True if file was found
   */
  inline bool containsDirectory(const CcString& sDirectoryName) const
  { return containsFile(sDirectoryName);}

  /**
   * @brief Get fileinfo from list by name.
   *        It should be verified if it is available with @ref containsFile
   * @param sFilename: Filename to get info
   * @return Handle to info
   */
  CcFileInfo& getFile(const CcString& sFilename);
  //! @copydoc getFile()
  const CcFileInfo& getFile(const CcString& sFilename) const;

  /**
   * @brief getDirectory info is same as @ref getFile. It is available for readability and compatibility reasons.
   *        It should be verified if it is available with @ref containsFile
   * @param sDirectoryName: Filename to get info
   * @return Handle to info
   */
  inline const CcFileInfo& getDirectory(const CcString& sDirectoryName) const
  { return getFile(sDirectoryName); }
  //! @copydoc getDirectory()
  inline CcFileInfo& getDirectory(const CcString& sDirectoryName)
  { return getFile(sDirectoryName); }

  /**
   * @brief Remove fileinfo from list by it's filename
   * @param sFilename: Filename to remov from list
   * @return True if file was found and removed
   */
  bool removeFile(const CcString& sFilename);

  /**
   * @brief Get formated string list with file informations.
   * @param uiShowFlags: Enums with display informations
   * @return Formated string list with file informations
   */
  CcStringList getFormatedList(EFileInfoListFormats uiShowFlags) const;
};
