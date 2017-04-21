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
* @page      CcFileAbstract
* @copyright Andreas Dirmeier (C) 2017
* @author    Andreas Dirmeier
* @par       Web: http://adirmeier.de/CcOS
* @version   0.01
* @date      2016-04
* @par       Language   C++ ANSI V3
* @brief     Class CcFileAbstract
*/
#ifndef CcFileAbstract_H_
#define CcFileAbstract_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "CcSharedPointer.h"

class CcFileAbstract;
class CcFileInfo;
class CcFileInfoList;
class CcString;
class CcDateTime;

#if WIN32
template class CcKernelSHARED CcSharedPointer<CcFileAbstract>;
#endif
typedef CcSharedPointer<CcFileAbstract> CcFilePointer;

/**
* @brief File Handler.
*/
class CcKernelSHARED CcFileAbstract : public CcIODevice {
public:
  /**
  * @brief Get Size of File
  * @return Size of File or SIZE_MAX if any Error occured
  */
  virtual size_t size(void) = 0;

  /**
  * @brief Read an amount of bytes from File
  * @param buffer: buffer where read data gets stored
  * @param size:   size of buffer, to maximum read
  * @return Number of bytes read from file or SIZE_MAX if any Error occured
  */
  virtual size_t read(char* buffer, size_t size) = 0;

  /**
  * @brief Write an amount of bytes to file
  * @param buffer: buffer with bytes to write to file
  * @param size:   number of bytes to write to file
  * @return Number of bytes written to file or SIZE_MAX if any Error occured
  */
  virtual size_t write(const char* buffer, size_t size) = 0;

  /**
  * @brief Open File with flags for type of access to be created
  * @param flags:
  * @return true if file was successfully opend
  * @todo make flags as enum for type savety
  */
  virtual bool open(EOpenFlags flags) = 0;

  /**
  * @brief Close File
  * @return true if no error occured
  */
  virtual bool close() = 0;

  /**
  * @brief Set File Pointer to a specific Position in File
  * @param pos: Position to set to
  * @return true if File Pointer is set successfully
  */
  virtual bool setFilePointer(size_t pos) = 0;

  /**
  * @brief Check if stored Path is a File, and not for example a directory
  * @return true if stored Path is File
  */
  virtual bool isFile(void) const = 0;

  /**
  * @brief Check if stored Path is a Directory, and not for example file
  * @return true if stored Path is Directory
  */
  virtual bool isDir(void) const = 0;

  /**
  * @brief Move Current File to new Location
  * @param Path: Target Location
  * @return true if File was successfully moved.
  */
  virtual bool move(const CcString& sPath) = 0;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getModified(void) const = 0;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getCreated(void) const = 0;
  
  virtual bool setCreated(const CcDateTime& oDateTime) = 0;
  virtual bool setModified(const CcDateTime& oDateTime) = 0;
  virtual bool setUserId(uint16 uiUserId) = 0;
  virtual bool setGroupId(uint16 uiGroupId) = 0;

  virtual CcFileInfo getInfo() const = 0;

  /**
   * @brief Get Filelist from current Filepath
   *          If current File is not a Directory, List will be empty
   * @param showFlags
   * @return List of Files in Directory
   * @todo replace showFlags with enum
   */
  virtual CcFileInfoList getFileList() const = 0;
};

#endif /* CcFileAbstract_H_ */
