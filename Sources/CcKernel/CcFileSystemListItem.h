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
 * @brief     Class CcFileSystemListItem
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "IFileSystem.h"

/**
 * @brief Manage all access to Files on a Specific-System.
 *        This class is designed to have a defined Access to FileSystems.
 */
class CcKernelSHARED CcFileSystemListItem
{
public:
  /**
   * @brief Constructor
   */
  CcFileSystemListItem() = default;
  /**
   * @brief Constructor with initializing members.
   * @param sPath: Initialize with this path
   * @param oFileSystem: FileSystem Handle to be associated with sPath
   */
  CcFileSystemListItem(const CcString& sPath, const CcFileSystemHandle& oFileSystem):
    m_sPath(sPath),
    m_oFS(oFileSystem)
    {}

  ~CcFileSystemListItem() = default;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcFileSystemListItem &oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcFileSystemListItem &oToCompare) const
    { return !operator==(oToCompare); }

  //! @return Get path to this filesystem
  const CcString& getPath() const
  { return m_sPath;}
  //! @return Get handle to target filesystem
  const CcFileSystemHandle& getFileSystem() const
  { return m_oFS;}

  //! @param sPath: Set new path for this filesystem
  void setPath(const CcString& sPath)
  { m_sPath = sPath;}
  //! @param oFileSystem: Set new handle to filesystem
  void setFileSystem(const CcFileSystemHandle& oFileSystem)
  { m_oFS = oFileSystem;}
private:
  CcString                m_sPath;
  CcFileSystemHandle      m_oFS;
};
