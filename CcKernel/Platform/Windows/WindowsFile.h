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
 * @page      Windows
 * @subpage   WindowsFile
 *
 * @page      WindowsFile
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsFile
 */
#ifndef WindowsFile_H_
#define WindowsFile_H_

#include "CcBase.h"
#include "WindowsGlobals.h"
#include "CcWString.h"
#include "CcFile.h"
#include "CcFileInfoList.h"

/**
 * @brief Button for GUI Applications
 */
class WindowsFile : public CcFileAbstract {
public:
  /**
   * @brief Constructor
   */
  WindowsFile(const CcString& path = "");

  /**
   * @brief Destructor
   */
  virtual ~WindowsFile( void );

  size_t size(void) override;
  size_t read(char* buffer, size_t size) override;
  size_t write(const char* buffer, size_t size) override;
  bool open(EOpenFlags flags) override;
  bool close(void) override;
  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  bool cancel() override {return false;}
  bool isFile(void) const override;
  bool isDir(void) const override;
  bool move(const CcString& Path) override;
  bool copy(const CcString& Path) override;
  CcFileInfo getInfo(void) const override;
  CcDateTime getModified(void) const override;
  CcDateTime getCreated(void) const override;
  
  bool setCreated(const CcDateTime& oDateTime) override;
  bool setModified(const CcDateTime& oDateTime) override;
  bool setUserId(uint16 uiUserId) override;
  bool setGroupId(uint16 uiUserId) override;


  virtual bool setFilePointer(size_t pos) override;

  CcFileInfoList getFileList() const override;

private:
  HANDLE m_hFile;
  CcWString m_sPath;
  uint64 m_uiFilePointer;
};

#endif /* WindowsFile_H_ */
