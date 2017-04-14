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
 * @page      CcFileSystem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcFileSystem
 */
#ifndef CCFILESYSTEM_H_
#define CCFILESYSTEM_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcFile.h"
#include "CcFileSystemListItem.h"
#include "CcVector.h"

#ifdef WIN32
template class CcKernelSHARED CcVector<CcFileSystemListItem>;
#endif

/**
 * @brief Manage all access to Files on a Specific-System.
 *        This class is designed to have a defined Access to FileSystems.
 */
class CcKernelSHARED CcFileSystem {
public:
  /**
   * @brief Constructor
   */
  CcFileSystem(void);

  /**
   * @brief Destructor
   */
  virtual ~CcFileSystem(void);

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
  virtual bool mkdir(const CcString& Path) const = 0;

  /**
   * @brief Delete File/Directory
   * @param Path: Path to File
   * @return true if successfully deleted
   */
  virtual bool remove(const CcString& Path) const = 0;
};

#endif /* CcFileSystem_H_ */
