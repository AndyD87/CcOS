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
 * @subpage   CcFileSystemManager
 *
 * @page      CcFileSystemManager
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcFileSystemManager
 */
#ifndef CcFileSystemManager_H_
#define CcFileSystemManager_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcFileSystem.h"
#include "CcFileSystemListItem.h"
#include "CcList.h"
#include "CcFileAbstract.h"

#ifdef WIN32
template class CcKernelSHARED CcList<CcFileSystemListItem>;
#endif

/**
 * @brief Manage all access to Files on a Specific-System.
 *        This class is designed to have a defined Access to FileSystems.
 */
class CcKernelSHARED CcFileSystemManager : private CcFileSystem
{
public:
  /**
   * @brief Constructor
   */
  CcFileSystemManager(void);

  /**
   * @brief Destructor
   */
  ~CcFileSystemManager(void);

  /**
   * @brief Get File by Path
   * @param path: Path to File
   * @return Pointer to File or NULL if file not found
   */
  CcFilePointer getFile(const CcString& path) const;

  /**
   * @brief Add a Mounting Point to currentFileSystem
   * @param Path: Path to new FileSystem in Current FileSystem
   * @param Filesystem: FileSystem to get linked to
   */
  void addMountPoint(const CcString& Path, CcHandle<CcFileSystem> Filesystem);

  /**
   * @brief Get Current Working dir of this FileSystem
   * @return Path as String
   */
  const CcString& getWorkingDir(void) const;

  /**
   * @brief Create a Directory
   * @param Path: Path to new directory
   * @return true if successfully created, or already available
   */
  bool mkdir(const CcString& Path) const;

  /**
   * @brief Delete File/Directory
   * @param Path: Path to File
   * @return true if successfully deleted
   */
  bool remove(const CcString& Path) const;

  /**
   * @brief Set Current Working Dir to new directory
   * @param Path: Path to new directory
   */
  void setWorkingDir( const CcString& Path);

private:
  CcString m_WorkingDir;    //!< Current Working Directory
  CcHandle<CcFileSystem> getFileSystemManagerByPath(const CcString& sPath) const;
  CcList<CcFileSystemListItem> m_FSList; //!< List of Mounted FileSystems
};

#endif /* CcFileSystemManager_H_ */
