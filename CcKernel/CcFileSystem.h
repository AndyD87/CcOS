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
 * @subpage   CcFileSystem
 *
 * @page      CcFileSystem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFileSystem
 */
#ifndef _CcFileSystem_H_
#define _CcFileSystem_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcFileSystemAbstract.h"
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
class CcKernelSHARED CcFileSystem
{
public:
  static void init();

  /**
   * @brief Get File by Path
   * @param path: Path to File
   * @return Pointer to File or NULL if file not found
   */
  static CcFilePointer getFile(const CcString& path);

  /**
   * @brief Add a Mounting Point to currentFileSystem
   * @param Path: Path to new FileSystem in Current FileSystem
   * @param Filesystem: FileSystem to get linked to
   */
  static void addMountPoint(const CcString& Path, CcFileSystemHandle Filesystem);

  /**
   * @brief Create a Directory
   * @param Path: Path to new directory
   * @return true if successfully created, or already available
   */
  static bool mkdir(const CcString& Path);

  /**
   * @brief Delete File/Directory
   * @param Path: Path to File
   * @return true if successfully deleted
   */
  static bool remove(const CcString& Path);

  static CcFileSystemHandle getFileSystemByPath(const CcString& sPath);
private:
  static CcString *m_WorkingDir;                 //!< Current Working Directory
  static CcList<CcFileSystemListItem> *m_FSList; //!< List of Mounted FileSystems
};

#endif /* _CcFileSystem_H_ */
