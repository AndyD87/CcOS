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
 * @brief     Class CcDirectory
 */
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "CcFile.h"
#include "CcFileInfoList.h"

class CcDateTime;

/**
 * @brief File Handler.
 */
class CcKernelSHARED CcDirectory
{
public:
  /**
   * @brief Constructor
   */
  CcDirectory( const CcString& sPath = "") : m_sPath(sPath)
    {}

  /**
   * @brief Destructor
   */
  ~CcDirectory() = default;

  /**
   * @brief Check if directory exists
   * @return True if directory exists
   */
  bool exists();

  /**
   * @brief Check if directory exists
   * @param sPathToDirectory: Path to directory
   * @return True if directory exists
   */
  static bool exists(const CcString& sPathToDirectory);

  /**
   * @brief Create directory if not already existing.
   * @param sPathToDirectory: Path to dir to create
   * @param bRecursive:       On true create parent dirs if not existing
   * @param bFaileOnExists:   Return false if directory was already available
   * @return True on success otherwise false.
   */
  static bool create(const CcString& sPathToDirectory, bool bRecursive = false, bool bFaileOnExists = false);
  //! @copydoc CcDirectory::create()
  static bool create(const char* sPathToDirectory, bool bRecursive = false, bool bFaileOnExists = false)
  { return create(CcString(sPathToDirectory), bRecursive, bFaileOnExists); }

  /**
   * @brief Create this directory if not already existing.
   * @param bRecursive:       On true create parent dirs if not existing
   * @param bFaileOnExists:   Return false if directory was already available
   * @return True on success otherwise false.
   */
  bool create(bool bRecursive = false, bool bFaileOnExists = false);

  /**
   * @brief Move a directory from one location to another
   * @param sPathToDirectoryFrom: Path to directory to move
   * @param sPathToDirectoryTo:   Target directory to move to
   * @return True if move was succeeded
   */
  static bool move(const CcString& sPathToDirectoryFrom, const CcString& sPathToDirectoryTo);

  /**
   * @brief Move this directory to another location
   * @param sPathToDirectoryTo:   Target directory to move to
   * @return True if move was succeeded
   */
  bool move( const CcString& sPathToDirectoryTo);

  /**
   * @brief Copy a directory from one location to another
   * @param sPathToDirectoryFrom: Path to directory to copy
   * @param sPathToDirectoryTo:   Target directory to copy to
   * @return True if copy was succeeded
   */
  static bool copy(const CcString& sPathToDirectoryFrom, const CcString& sPathToDirectoryTo);

  /**
   * @brief Copy this directory to another location
   * @param sPathToDirectoryTo:   Target directory to copy to
   * @return True if copy was succeeded
   */
  bool copy(const CcString& sPathToDirectoryTo);

  /**
   * @brief Remove a directory from system. It has to be empty.
   * @param sPathToDirectory: Directory to remove
   * @return True if directory was successfully removed
   */
  static bool remove(const CcString& sPathToDirectory);
  //! @copydoc CcDirectory::remove()
  static bool remove(const char* sPathToDirectory)
  { return remove(CcString(sPathToDirectory)); }

  /**
   * @brief Remove a directory from system. It has to be empty, or recursive is enabled.
   * @param sPathToDirectory: Directory to remove
   * @param bRecursive:       If true delete containing files and folders too.
   * @return True if directory was successfully removed
   */
  static bool remove(const CcString& sPathToDirectory, bool bRecursive);

  /**
   * @brief Remove this directory from system. It has to be empty, or recursive is enabled.
   * @param bRecursive:       If true delete containing files and folders too.
   * @return True if directory was successfully removed
   */
  bool remove(bool bRecursive = false);

  /**
   * @brief Remove all files and, if Recursive is set, all folders within this directory
   * @param bRecursive: remove directories too
   * @return true if succeeded
   */
  bool clear(bool bRecursive = false);
  
  /**
   * @brief Remove all files and, if recursive is set, all folders within directory
   * @param sPath:      Target file to clear
   * @param bRecursive: remove directories too
   * @return true if succeeded
   */
  static bool clear(const CcString& sPath, bool bRecursive = false);

  /**
   * @brief Set created timestamp of file.
   * @param oCreated: Time to set
   * @return True if time was changed successfully
   */
  bool setCreated(const CcDateTime& oCreated);

  /**
   * @brief Set modified timestamp of file.
   * @param oModified: Time to set
   * @return True if time was changed successfully
   */
  bool setModified(const CcDateTime& oModified);

  /**
   * @brief Set user id of file.
   * @param uiUserId: User id to set
   * @return True if user was changed successfully
   */
  bool setUserId(uint32 uiUserId);

  /**
   * @brief Set group id of file.
   * @param uiGroupId: Group id to set
   * @return True if group was changed successfully
   */
  bool setGroupId(uint32 uiGroupId);

  /**
   * @brief Get file list from current Directory
   * @return List of file informations
   */
  CcFileInfoList getFileList();

  const CcString& getFilePath()
  { return m_sPath; }
  void setFilePath(const CcString& sPath);

  /**
   * @brief Get file list from a Directory
   * @param sPathToDirectory: Patht to file for list
   * @return List of file informations
   */
  static CcFileInfoList getFileList(const CcString& sPathToDirectory);

protected: //Variables
  CcString m_sPath;      //!< Path to File as String
};
