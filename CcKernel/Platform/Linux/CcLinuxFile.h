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
 * @file
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxFile
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "IFile.h"
#include "CcFile.h"
#include "CcByteArray.h"
#include <stdio.h>

class CcFileInfoList;

/**
 * @brief Button for GUI Applications
 */
class CcLinuxFile : public IFile {
public:
  /**
   * @brief Constructor
   */
  CcLinuxFile(const CcString& path = "");

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxFile();

  virtual size_t size() override;
  virtual uint64 size64() override;
  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus flush() override;
  virtual CcStatus open(EOpenFlags flags) override;
  virtual CcStatus close() override;
  virtual bool isFile() const override;
  virtual bool isDir() const override;
  virtual CcStatus move(const CcString& Path) override;
  virtual CcStatus copy(const CcString& Path) override;
  virtual CcDateTime getModified() const override;
  virtual CcDateTime getCreated() const override;
  virtual CcStatus setCreated(const CcDateTime& oDateTime) override;
  virtual uint64 getFilePointer() const override;
  virtual CcStatus setModified(const CcDateTime& oDateTime) override;
  virtual CcStatus setUserId(uint32 uiUserId) override;
  virtual CcStatus setGroupId(uint32 uiUserId) override;
  virtual CcStatus setAttributes(EFileAttributes uiAttributes) override;

  /**
   * @brief Get attributes from file
   * @return File attributes
   */
  EFileAttributes getAttributes();

  virtual CcStatus ioControl(uint32 cmd, const void *pInArg = nullptr, size_t uiInSize = 0, void *pOutArg = nullptr, size_t uiOutSize = 0, size_t* puiWritten = nullptr) override;

  virtual CcStatus setFilePointer(uint64 pos) override;

  /**
   * @brief Create empty file.
   * @return True if succeeded.
   */
  bool createFile();

  virtual CcFileInfo getInfo() const override;
  virtual void* getStdFile() override
    {return static_cast<void*>(m_hFile);}

  virtual CcFileInfoList getFileList() const override;
  virtual CcStatus cancel() override
    { return false; }

private:
  FILE* m_hFile = nullptr;
  CcString m_sPath;
};
