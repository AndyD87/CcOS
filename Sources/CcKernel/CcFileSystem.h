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
 * @brief     Class CcFileSystem
 */
#pragma once

#include "CcBase.h"
#include "IFileSystem.h"
#include "CcFileSystemListItem.h"
#include "CcVector.h"
#include "IFile.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<CcFileSystemListItem>;
#endif

/**
 * @brief Manage all access to Files on a Specific-System.
 *        This class is designed to have a defined Access to FileSystems.
 */
class CcKernelSHARED CcFileSystem
{
public:
  /**
   * @brief Initialize global filesystem managment
   */
  static void init(CcVector<CcFileSystemListItem>*& pItem);

  /**
   * @brief Deinitialize and cleanup global filesystem managment
   */
  static void deinit(CcVector<CcFileSystemListItem>*& pItem);

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
   * @brief Remove a Mounting Point to currentFileSystem
   * @param Path: Path to FileSystem to remove
   */
  static void removeMountPoint(const CcString& Path);

  /**
   * @brief Create a Directory
   * @param Path: Path to new directory
   * @return true if successfully created, or already available
   */
  static CcStatus mkdir(const CcString& Path);

  /**
   * @brief Delete File/Directory
   * @param Path: Path to File
   * @return true if successfully deleted
   */
  static CcStatus remove(const CcString& Path);

  /**
   * @brief Get next free autoincremented filename in specified directory.
   * @param sPath: sPath to directory for file:
   * @param sName: sName without incremental number
   * @param sAppend: String which is following after number
   * @return New name
   */
  static CcString getNextFreeFilename(const CcString& sPath, const CcString& sName, const CcString& sAppend);

  /**
   * @brief Find Executable in all binary PATH dirs
   * @param sName: Name of Executable to find.
   * @return Absolute path or empty string if not found.
   */
  static CcString findExecutable(const CcString& sName);

  /**
   * @brief Find all executables in all binary PATH dirs
   * @param sName: Name of Executable to find.
   * @param uiNr:  Maximum number of executables to find. Default = 0 for no limit.
   * @return Absolute path or empty string if not found.
   */
  static CcStringList findExecutables(const CcString& sName, size_t uiNr = 0);

  /**
   * @brief Find all executables in all binary PATH dirs
   * @param oDirs: List of all directories to search in
   * @param sName: Name of File to find.
   * @param uiNr:  Maximum number of executables to find. Default = 0 for no limit.
   * @return Absolute path or empty string if not found.
   */
  static CcStringList findFileInDirectories(const CcStringList oDirs, const CcString& sName, size_t uiNr = 0);

  /**
   * @brief Get a filesystem by path
   * @param sPath: Path to search for basic filsystem
   * @param[out] sInnerPath: Path to file within filesystem.
   * @return Filesystem handle of path, or invalid handle if path not found
   */
  static CcFileSystemHandle getFileSystemByPath(const CcString& sPath, CcString& sInnerPath);

  /**
   * @brief Get all Filesystems wich are mapped wit this given path and higher
   * @param sPath: Target mount dir
   * @return List of all Filesystems
   */
  static CcFileInfoList getFileSystemsByPath(const CcString& sPath);
  
private:
  static CcVector<CcFileSystemListItem> *s_pFSList; //!< List of Mounted FileSystems
};
