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
 * @subpage   CcWindowsFile
 *
 * @page      CcWindowsFile
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindowsFile
 */
#ifndef CcWindowsFile_H_
#define CcWindowsFile_H_

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "CcWString.h"
#include "CcFile.h"
#include "CcFileInfoList.h"

/**
 * @brief Button for GUI Applications
 */
class CcWindowsFile : public CcFileAbstract {
public:
  /**
   * @brief Constructor
   */
  CcWindowsFile(const CcString& path = "");

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsFile( void );

  size_t size(void) override;
  uint64 size64(void) override;
  virtual size_t read(void* buffer, size_t size) override;
  virtual size_t write(const void* buffer, size_t size) override;
  virtual CcStatus open(EOpenFlags flags) override;
  virtual CcStatus close(void) override;
  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  virtual CcStatus cancel() override {return false;}
  virtual bool isFile(void) const override;
  virtual bool isDir(void) const override;
  virtual CcStatus move(const CcString& Path) override;
  virtual CcStatus copy(const CcString& Path) override;
  virtual CcFileInfo getInfo(void) const override;
  virtual CcDateTime getModified(void) const override;
  virtual CcDateTime getCreated(void) const override;
  
  virtual CcStatus setCreated(const CcDateTime& oDateTime) override;
  virtual CcStatus setModified(const CcDateTime& oDateTime) override;
  virtual CcStatus setUserId(uint32 uiUserId) override;
  virtual CcStatus setGroupId(uint32 uiUserId) override;
  virtual CcStatus setAttributes(EFileAttributes eAttributes) override;


  virtual CcStatus setFilePointer(uint64 pos) override;

  virtual CcFileInfoList getFileList() const override;

private:
  HANDLE m_hFile;
  CcWString m_sPath;
};

#endif /* CcWindowsFile_H_ */
