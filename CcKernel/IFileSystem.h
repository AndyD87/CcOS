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
#ifndef H_CCFILESYSTEM_H_
#define H_CCFILESYSTEM_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcHandle.h"
#include "CcFile.h"

class IFileSystem;

#ifdef _MSC_VER
  template class CcKernelSHARED CcHandle<IFileSystem>;
#endif

typedef CcHandle<IFileSystem> CcFileSystemHandle;

/**
 * @brief Manage all access to Files on a Specific-System.
 *        This class is designed to have a defined Access to FileSystems.
 */
class CcKernelSHARED IFileSystem
{
public:
  /**
   * @brief Constructor
   */
  IFileSystem() = default;

  /**
   * @brief Destructor
   */
  virtual ~IFileSystem() = default;

  /**
   * @brief Get File by Path
   * @param path: Path to File
   * @return Pointer to File or NULL if file not found
   */
  virtual CcFilePointer getFile(const CcString& path) const = 0;

  /**
   * @brief Create a Directory
   * @param Path: Path to new directory
   * @return true if successfully created, or already available
   */
  virtual CcStatus mkdir(const CcString& Path) const = 0;

  /**
   * @brief Delete File/Directory
   * @param Path: Path to File
   * @return true if successfully deleted
   */
  virtual CcStatus remove(const CcString& Path) const = 0;
};

#endif // H_CcFileSystem_H_
