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
 * @subpage   CcLinuxFile
 *
 * @pag       CcLinuxFile
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class CcLinuxFile
 */
#ifndef CcLinuxFile_H_
#define CcLinuxFile_H_

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
class CcLinuxFile : public CcFileAbstract {
public:
  /**
   * @brief Constructor
   */
  CcLinuxFile(const CcString& path = "");

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxFile( void );

  virtual size_t size(void) override;
  virtual uint64 size64(void) override;
  virtual size_t read(void* buffer, size_t size) override;
  virtual size_t write(const void* buffer, size_t size) override;
  virtual CcStatus open(EOpenFlags flags) override;
  virtual CcStatus close(void) override;
  virtual bool isFile(void) const override;
  virtual bool isDir(void) const override;
  virtual CcStatus move(const CcString& Path) override;
  virtual CcStatus copy(const CcString& Path) override;
  virtual CcDateTime getModified(void) const override;
  virtual CcDateTime getCreated(void) const override;
  virtual CcStatus setCreated(const CcDateTime& oDateTime) override;
  virtual CcStatus setModified(const CcDateTime& oDateTime) override;
  virtual CcStatus setUserId(uint32 uiUserId) override;
  virtual CcStatus setGroupId(uint32 uiUserId) override;
  virtual CcStatus setAttributes(EFileAttributes uiAttributes) override;
  EFileAttributes getAttributes();

  virtual CcStatus ioControl(uint32 cmd, const void *argument) override;

  virtual CcStatus setFilePointer(size_t pos) override;

  bool createFile();

  virtual CcFileInfo getInfo() const;

  virtual CcFileInfoList getFileList() const override;
  virtual CcStatus cancel()
    {}

private:
  FILE* m_hFile = nullptr;
  CcString m_Path;
};

#endif /* CcLinuxFile_H_ */
