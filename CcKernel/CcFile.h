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
 * @subpage   CcFile
 *
 * @page      CcFile
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFile
 */
#ifndef CcFile_H_
#define CcFile_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcFileAbstract.h"

class CcCrc32;
class CcFile;
class CcString;
class CcByteArray;

/**
 * @brief File Handler.
 */
class CcKernelSHARED CcFile : public CcFileAbstract {
public:
  /**
   * @brief Constructor
   */
  CcFile(void);

  /**
   * @brief Constructor
   */
  CcFile( const CcString& path);


  /**
   * @brief Destructor
   */
  virtual ~CcFile( void );

  /**
   * @brief Get Size of File
   * @return Size of File or SIZE_MAX if any Error occured
   */
  virtual size_t size(void);

  /**
   * @brief Get Size of File as 64 bit unsigned integer
   * @return Size of File or UINT64_MAX if any Error occured
   */
  virtual uint64 size64(void);

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
   * @brief Check if stored Path is a File, and not for example a directory
   * @return true if stored Path is File
   */
  virtual bool isFile(void) const override;

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
  virtual bool isDir(void) const override;
  
  /**
   * @brief Check if stored Path is a Directory, and not for example file
   * @param sPath: Path to target directory.
   * @return true if stored Path is directory
   */
  static bool isDir(const CcString& sPath);

  /**
   * @brief Move Current File to new Location, and delete source
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus move(const CcString& sPath) override;
  
  /**
   * @brief Move Current File to new Location, and delete source
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  static CcStatus move(const CcString sFrom, const CcString& sTo);
  
  /**
   * @brief Copy Current File to new Location
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus copy(const CcString& sPath) override;
  
  /**
   * @brief Move Current File to new Location
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  static CcStatus copy(const CcString sFrom, const CcString& sTo);
  
  /**
   * @brief Compare two files if they are the same
   * @param sFile1: File 1 for compare
   * @param sFile2: File 2 for compare
   * @param bDoCrc: Compare Files by thier Crc value ( much faster )
   * @return true if Files are same.
   */
  static bool compare(const CcString sFile1, const CcString& sFile2, bool bDoCrc = true);

  virtual CcFileInfo getInfo() const override;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getModified(void) const;

  /**
   * @brief Get Filelist from current Filepath
   *          If current File is not a Directory, List will be empty
   * @param showFlags
   * @return List of Files in Directory
   * @todo replace showFlags with enum
   */
  virtual CcFileInfoList getFileList() const;
  virtual CcDateTime getCreated(void) const override;
  virtual CcStatus setCreated(const CcDateTime& oDateTime) override;
  virtual CcStatus setModified(const CcDateTime& oDateTime) override;
  virtual CcStatus setUserId(uint32 uiUserId) override;
  virtual CcStatus setGroupId(uint32 uiGroupId) override;
  virtual CcStatus setAttributes(EFileAttributes uiAttributes) override;

  static CcStatus setCreated(const CcString& sFilePath, const CcDateTime& oDateTime);
  static CcStatus setModified(const CcString& sFilePath, const CcDateTime& oDateTime);
  static CcStatus setUserId(const CcString& sFilePath, uint32 uiUserId);
  static CcStatus setGroupId(const CcString& sFilePath, uint32 uiGroupId);

  inline bool exists() const
    {return isFile() || isDir();}

  static bool exists(const CcString& sPathToFile);
  static CcStatus remove(const CcString& sPathToFile);

  CcCrc32 getCrc32();
  static CcCrc32 getCrc32(const CcString& sPathToFile);

protected: //Variables
  size_t m_filePointer; //!< Position of FilePointer in File
private:
  CcFilePointer m_SystemFile; //!< Pointer to SystemFile for interface-Class
};

#endif /* CcFile_H_ */
