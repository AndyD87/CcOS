/*
 *
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
 * @page      Linux
 * @subpage   LinuxFile
 *
 * @pag       LinuxFile
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class LinuxFile
 */
#ifndef LinuxFile_H_
#define LinuxFile_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcFileAbstract.h"
#include "CcFile.h"
#include "CcByteArray.h"
#include <stdio.h>

class CcFileInfoList;

/**
 * @brief Button for GUI Applications
 */
class LinuxFile : public CcFileAbstract {
public:
  /**
   * @brief Constructor
   */
  LinuxFile(const CcString& path = "");

  /**
   * @brief Destructor
   */
  virtual ~LinuxFile( void );

  virtual size_t size(void) override;
  virtual size_t read(char* buffer, size_t size) override;
  virtual size_t write(const char* buffer, size_t size) override;
  virtual bool open(EOpenFlags flags) override;
  virtual bool close(void) override;
  virtual bool isFile(void) const override;
  virtual bool isDir(void) const override;
  virtual bool move(const CcString& Path) override;
  virtual bool copy(const CcString& Path) override;
  virtual CcDateTime getModified(void) const override;
  virtual CcDateTime getCreated(void) const override;
  virtual bool setCreated(const CcDateTime& oDateTime) override;
  virtual bool setModified(const CcDateTime& oDateTime) override;
  virtual bool setUserId(uint32 uiUserId) override;
  virtual bool setGroupId(uint32 uiUserId) override;
  virtual bool setAttributes(EFileAttributes uiAttributes) override;
  EFileAttributes getAttributes();

  virtual bool ioControl(uint32 cmd, const void *argument) override;

  virtual bool setFilePointer(size_t pos) override;

  bool createFile();

  virtual CcFileInfo getInfo() const;

  virtual CcFileInfoList getFileList() const override;
  virtual bool cancel()
    {}

private:
  FILE* m_hFile = nullptr;
  CcString m_Path;
};

#endif /* LinuxFile_H_ */
