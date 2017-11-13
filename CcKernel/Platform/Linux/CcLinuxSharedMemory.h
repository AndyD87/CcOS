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
 * @subpage   CcIODevice
 *
 * @page      CcIODevice
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcLinuxSharedMemory
 *            Abstract Class to define an interface to System own Shared Memory
 */
#ifndef CcLinuxSharedMemory_H_
#define CcLinuxSharedMemory_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcSharedMemoryAbstract.h"

class CcLinuxSharedMemoryPrivate;

class CcLinuxSharedMemory : public CcSharedMemoryAbstract
{
public:
  CcLinuxSharedMemory(const CcString& sName, uint32 uiSize);
  virtual ~CcLinuxSharedMemory();

  virtual CcStatus open(EOpenFlags eOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  virtual CcStatus claim(EOpenFlags eOpenFlags) override;
  virtual bool exists() override;

  virtual size_t read(void* buffer, size_t size) override;
  virtual size_t write(const void* buffer, size_t size) override;
  
private:
  CcString m_sName;
  size_t m_uiSize = 0;
  CcLinuxSharedMemoryPrivate* m_pPrivate;
};

#endif // CcLinuxSharedMemory_H_
