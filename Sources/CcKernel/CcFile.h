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
 * @brief     Class CcFile
 */
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "IFile.h"

class CcCrc32;
class CcFile;
class CcString;
class CcByteArray;

/**
 * @brief File Handler.
 */
class CcKernelSHARED CcFile : public IFile
{
public:
  /**
   * @brief Constructor
   */
  CcFile() = default;

  /**
   * @brief Constructor
   */
  CcFile( const CcString& sPath);

  /**
   * @brief Destructor
   */
  virtual ~CcFile() = default;

  /**
   * @brief Get Size of File
   * @return Size of File or SIZE_MAX if any Error occured
   */
  virtual size_t size() override;

  /**
   * @brief Get Size of File as 64 bit unsigned integer
   * @return Size of File or UINT64_MAX if any Error occured
   */
  virtual uint64 size64() override;

  /**
   * @brief Read an amount of bytes from File
   * @param pBuffer: buffer where read data gets stored
   * @param uSize:   size of buffer, to maximum read
   * @return Number of bytes read from file or SIZE_MAX if any Error occured
   */
  virtual size_t read(void* pBuffer, size_t uSize) override;

  /**
   * @brief Write an amount of bytes to file
   * @param pBuffer: buffer with bytes to write to file
   * @param uSize:   number of bytes to write to file
   * @return Number of bytes written to file or SIZE_MAX if any Error occured
   */
  virtual size_t write(const void* pBuffer, size_t uSize) override;

  virtual CcStatus flush() override;
  /**
   * @brief Open File with flags for type of access to be created
   * @param flags:
   * @return true if file was successfully opend
   * @todo make flags as enum for type savety
   */
  virtual CcStatus open(EOpenFlags flags) override;

  /**
   * @brief Close File
   * @return true if no error occured
   */
  virtual CcStatus close() override;

  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  virtual CcStatus cancel() override {return false;}

  /**
   * @brief Set File Pointer to a specific Position in File
   * @param pos: Position to set to
   * @return true if File Pointer is set successfully
   */
  virtual CcStatus setFilePointer(uint64 pos) override;

  /**
   * @brief Change current stored file path
   *        close will be called before filepath will be changed.
   * @param sPath: New target file path
   * @return void
   */
  virtual void setFilePath(const CcString& sPath);

  /**
   * @brief Check if stored Path is a File, and not for example a directory
   * @return true if stored Path is File
   */
  virtual bool isFile() const override;

  /**
   * @brief Check if stored Path is a File, and not for example a directory
   * @param sPath: Path to target file.
   * @return true if stored Path is File
   */
  static bool isFile(const CcString& sPath);

  /**
   * @brief Check if stored Path is a Directory, and not for example file
   * @return true if stored Path is Directory
   */
  virtual bool isDir() const override;
  
  /**
   * @brief Check if stored Path is a Directory, and not for example file
   * @param sPath: Path to target directory.
   * @return true if stored Path is directory
   */
  static bool isDir(const CcString& sPath);

  /**
   * @brief Move Current File to new Location, and delete source
   * @param sPath: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus move(const CcString& sPath) override;
  
  /**
   * @brief Move File to new Location, and delete source
   * @param sFrom: Source location
   * @param sTo: Target Location
   * @return true if File was successfully moved.
   */
  static CcStatus move(const CcString& sFrom, const CcString& sTo);
  
  /**
   * @brief Copy current file to Location
   * @param sPath: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus copy(const CcString& sPath) override;

  /**
   * @brief Copy file to target location
   * @param sFrom: source location
   * @param sTo: target Location
   * @return true if file was successfully moved.
   */
  static CcStatus copy(const CcString& sFrom, const CcString& sTo);
  
  /**
   * @brief Compare two files if they are the same
   * @param sFile1: File 1 for compare
   * @param sFile2: File 2 for compare
   * @param bDoCrc: Compare Files by thier Crc value ( much faster )
   * @return true if Files are same.
   */
  static bool compare(const CcString& sFile1, const CcString& sFile2, bool bDoCrc = true);

  virtual CcFileInfo getInfo() const override;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getModified() const override;

  /**
   * @brief Get info of specific file
   * @param sFilePath: Path to file to retreive informations from
   * @return Read informations from file
   */
  static CcFileInfo getInfo(const CcString& sFilePath);

  /**
   * @brief Get modified time of specific file
   * @param sFilePath: Path to file to retreive modified time from
   * @return Read modified time from file
   */
  static CcDateTime getModified(const CcString& sFilePath);

  /**
   * @brief Get Filelist from current Filepath
   *          If current File is not a Directory, List will be empty
   * @return List of Files in Directory
   */
  virtual CcFileInfoList getFileList() const override;

  /**
   * @brief Get current position of pointer in File
   * @return true if File Pointer is set successfully
   */
  virtual uint64 getFilePointer() const override;
  virtual CcDateTime getCreated() const override;

  virtual CcStatus setCreated(const CcDateTime& oDateTime) override;
  virtual CcStatus setModified(const CcDateTime& oDateTime) override;
  virtual CcStatus setUserId(uint32 uiUserId) override;
  virtual CcStatus setGroupId(uint32 uiGroupId) override;
  virtual CcStatus setAttributes(EFileAttributes uiAttributes) override;

  /**
   * @brief Set created file time to specific file
   * @param sFilePath:  File to update time
   * @param oDateTime:  Time to set to file
   * @return Status of operation
   */
  static CcStatus setCreated(const CcString& sFilePath, const CcDateTime& oDateTime);

  /**
   * @brief Set modified file time to specific file
   * @param sFilePath:  File to update time
   * @param oDateTime:  Time to set to file
   * @return Status of operation
   */
  static CcStatus setModified(const CcString& sFilePath, const CcDateTime& oDateTime);

  /**
   * @brief Set user id of specific file
   * @param sFilePath:  File to update time
   * @param uiUserId:   New user id
   * @return Status of operation
   */
  static CcStatus setUserId(const CcString& sFilePath, uint32 uiUserId);

  /**
   * @brief Set group id of specific file
   * @param sFilePath:  File to update time
   * @param uiGroupId:   New group id
   * @return Status of operation
   */
  static CcStatus setGroupId(const CcString& sFilePath, uint32 uiGroupId);

  //! @return True if file or directory is available
  inline bool exists() const
  {return isFile() || isDir();}

  /**
   * @brief Check if specific file or directory is availabe.
   * @param sPathToFile: Path to target file
   * @return True if file or directory is available
   */
  static bool exists(const CcString& sPathToFile);

  /**
   * @brief Remove specific file from disk
   * @param sPathToFile: Path to file to remove
   * @return Status of operation
   */
  static CcStatus remove(const CcString& sPathToFile);

  /**
   * @brief Resolve path, cleanup and get it's final absolute path to file.
   * @param sPathToFile: Path to cleanup
   * @return Clean absolute path to file
   */
  static CcString getAbsolutePath(const CcString& sPathToFile);
  
  /**
   * @brief Resolve path, cleanup and get it's final absolute path to file.
   * @param sPathToFile: Path to cleanup
   * @param sWorkingDir: Path to prefix if sPathToFile is relative
   * @return Clean absolute path to file
   */
  static CcString getAbsolutePath(const CcString& sPathToFile, const CcString& sWorkingDir);

  /**
   * @brief Get Crc32 value of current file
   * @return Generated value of file
   */
  CcCrc32 getCrc32();

  /**
   * @brief Get Crc32 value of specific file
   * @param sPathToFile: Path to file to get hash for
   * @return Generated value of file
   */
  static CcCrc32 getCrc32(const CcString& sPathToFile);

  virtual void* getStdFile() override;

protected: //Variables
  size_t m_filePointer = 0; //!< Position of FilePointer in File
private:
  CcFilePointer m_SystemFile; //!< Pointer to SystemFile for interface-Class
};
