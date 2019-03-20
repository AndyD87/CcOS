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
 * @subpage   CcGenericFile
 *
 * @pag       CcGenericFile
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcGenericFile
 */
#ifndef _CcGenericFile_H_
#define _CcGenericFile_H_

#include "CcBase.h"
#include "CcString.h"
#include "IFile.h"
#include "CcFileInfo.h"
#include "CcBufferList.h"

class CcFileInfoList;

/**
 * @brief Button for GUI Applications
 */
class CcGenericFile : public IFile
{
public:
  /**
   * @brief Constructor
   */
  CcGenericFile();

  /**
   * @brief Destructor
   */
  virtual ~CcGenericFile();

  virtual size_t size() override;
  virtual uint64 size64() override;
  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags flags) override;
  virtual CcStatus close() override;
  virtual bool isFile() const override;
  virtual bool isDir() const override;
  virtual CcStatus move(const CcString& Path) override;
  virtual CcStatus copy(const CcString& Path) override;
  virtual CcDateTime getModified() const override;
  virtual CcDateTime getCreated() const override;
  virtual CcStatus setCreated(const CcDateTime& oDateTime) override;
  virtual CcStatus setModified(const CcDateTime& oDateTime) override;
  virtual CcStatus setUserId(uint32 uiUserId) override;
  virtual CcStatus setGroupId(uint32 uiUserId) override;
  virtual CcStatus setAttributes(EFileAttributes uiAttributes) override;
  EFileAttributes getAttributes();

  virtual CcStatus ioControl(uint32 cmd, const void *argument) override;

  virtual CcStatus setFilePointer(uint64 pos) override;

  bool createFile();

  virtual CcFileInfo getInfo() const override;

  virtual CcFileInfoList getFileList() const override;
  virtual CcStatus cancel() override
    { return false; }

private:
  CcFileInfo m_oFileInfo;
  CcBufferList m_oData;
  EOpenFlags m_eOpenFlags;
  uint64_t   m_uiOffset;
};

#endif /* _CcGenericFile_H_ */
