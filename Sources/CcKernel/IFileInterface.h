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
 * @brief     Class IFileInterface
 */
#pragma once

#include "CcBase.h"
#include "IFile.h"
#include "CcFileInfoList.h"
#include "CcDateTime.h"

/**
 * @brief File Interface with all definition for implemnting file access.
 *        Some basic methods for all files are implemented too.
 */
class CcKernelSHARED IFileInterface : public IFile
{
public:
  IFileInterface(IFile* pBase) : m_pBase(pBase)
  {}
  
  virtual size_t size()
  { return m_pBase->size(); }
  virtual uint64 size64()
  { return m_pBase->size64(); }
  virtual size_t read(void* pBuffer, size_t uSize)
  { return m_pBase->read(pBuffer, uSize); }
  virtual size_t write(const void* pBuffer, size_t uSize)
  { return m_pBase->write(pBuffer, uSize); }
  virtual CcStatus open(EOpenFlags flags)
  { return m_pBase->open(flags); }
  virtual CcStatus close()
  { return m_pBase->close(); }
  virtual CcStatus cancel()
  { return m_pBase->cancel(); }
  virtual CcStatus setFilePointer(uint64 pos)
  { return m_pBase->setFilePointer(pos); }
  virtual bool isFile() const
  { return m_pBase->isFile(); }
  virtual bool isDir() const
  { return m_pBase->isDir(); }
  virtual CcStatus move(const CcString& sPath)
  { return m_pBase->move(sPath); }
  virtual CcStatus copy(const CcString& sPath)
  { return m_pBase->copy(sPath); }
  virtual CcDateTime getModified() const
  { return m_pBase->getModified(); }
  virtual CcDateTime getCreated() const
  { return m_pBase->getCreated(); }
  virtual CcStatus setCreated(const CcDateTime& oDateTime)
  { return m_pBase->setCreated(oDateTime); }
  virtual CcStatus setModified(const CcDateTime& oDateTime)
  { return m_pBase->setModified(oDateTime); }
  virtual CcStatus setUserId(uint32 uiUserId)
  { return m_pBase->setUserId(uiUserId); }
  virtual CcStatus setGroupId(uint32 uiGroupId)
  { return m_pBase->setGroupId(uiGroupId); }
  virtual CcStatus setAttributes(EFileAttributes uiAttributes)
  { return m_pBase->setAttributes(uiAttributes); }
  virtual uint64 getFilePointer() const
  { return m_pBase->getFilePointer(); }
  virtual CcFileInfo getInfo() const
  { return m_pBase->getInfo(); }
  virtual CcFileInfoList getFileList() const
  { return m_pBase->getFileList(); }
private:
  IFile* m_pBase;
};
