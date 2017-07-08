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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
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
   * @brief Read an amount of bytes from File
   * @param buffer: buffer where read data gets stored
   * @param size:   size of buffer, to maximum read
   * @return Number of bytes read from file or SIZE_MAX if any Error occured
   */
  virtual size_t read(char* buffer, size_t size) override;

  /**
   * @brief Write an amount of bytes to file
   * @param buffer: buffer with bytes to write to file
   * @param size:   number of bytes to write to file
   * @return Number of bytes written to file or SIZE_MAX if any Error occured
   */
  virtual size_t write(const char* buffer, size_t size) override;

  /**
   * @brief Open File with flags for type of access to be created
   * @param flags:
   * @return true if file was successfully opend
   * @todo make flags as enum for type savety
   */
  virtual bool open(EOpenFlags flags) override;

  /**
   * @brief Close File
   * @return true if no error occured
   */
  virtual bool close() override;

  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  bool cancel()override {return false;}

  /**
   * @brief Set File Pointer to a specific Position in File
   * @param pos: Position to set to
   * @return true if File Pointer is set successfully
   */
  virtual bool setFilePointer(size_t pos);

  /**
   * @brief Check if stored Path is a File, and not for example a directory
   * @return true if stored Path is File
   */
  virtual bool isFile(void) const override;

  /**
   * @brief Check if stored Path is a Directory, and not for example file
   * @return true if stored Path is Directory
   */
  virtual bool isDir(void) const override;

  /**
   * @brief Move Current File to new Location
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  virtual bool move(const CcString& sPath) override;
  
  /**
   * @brief Move Current File to new Location
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  static bool move(const CcString sFrom, const CcString& sTo);

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
  virtual bool setCreated(const CcDateTime& oDateTime) override;
  virtual bool setModified(const CcDateTime& oDateTime) override;
  virtual bool setUserId(uint16 uiUserId) override;
  virtual bool setGroupId(uint16 uiGroupId) override;

  static bool setCreated(CcString sFilePath, const CcDateTime& oDateTime);
  static bool setModified(CcString sFilePath, const CcDateTime& oDateTime);
  static bool setUserId(CcString sFilePath, uint16 uiUserId);
  static bool setGroupId(CcString sFilePath, uint16 uiGroupId);

  inline bool exists() const
    {return isFile() || isDir();}

  static bool exists(const CcString& sPathToFile);
  static bool remove(const CcString& sPathToFile);

  CcCrc32 getCrc32();
  static CcCrc32 getCrc32(const CcString& sPathToFile);

protected: //Variables
  size_t m_filePointer; //!< Position of FilePointer in File
private:
  CcFilePointer m_SystemFile; //!< Pointer to SystemFile for interface-Class
};

#endif /* CcFile_H_ */
