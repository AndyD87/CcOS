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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSharedMemory
 *            Abstract Class to define an interface to System own Shared Memory
 */
#ifndef CcSharedMemory_H_
#define CcSharedMemory_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcSharedMemoryAbstract.h"

class CcKernelSHARED CcSharedMemory : public CcSharedMemoryAbstract
{
public:
  CcSharedMemory(const CcString& sName, size_t uiSize = c_uiSize);
  CcSharedMemory(const CcSharedMemory&) = delete; //<! no copy allowed
  CcSharedMemory(CcSharedMemory&& oToMove)
    { operator=(std::move(oToMove)); }
  virtual ~CcSharedMemory();

  CcSharedMemory& operator=(const CcSharedMemory&) = delete; //<! no copy allowed
  CcSharedMemory& operator=(CcSharedMemory&&);

  virtual CcStatus open(EOpenFlags eOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  virtual CcStatus claim(EOpenFlags eOpenFlags) override;
  virtual bool exists() override;

  virtual size_t read(void* buffer, size_t size) override;
  virtual size_t write(const void* buffer, size_t size) override;

  inline static size_t getDefaultSize()
    { return c_uiSize; }

private:
  CcSharedMemoryAbstract* m_pSystem = nullptr;
  static const size_t c_uiSize = 1024; //<! Default size is set to 1024byte
};

#endif // CcSharedMemory_H_
