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
 * @brief     Class CcSharedMemoryAbstract
 *            Abstract Class to define an interface to System own Shared Memory
 */
#ifndef CcSharedMemoryAbstract_H_
#define CcSharedMemoryAbstract_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"

class CcKernelSHARED CcSharedMemoryAbstract : public CcIODevice
{
public:
  virtual ~CcSharedMemoryAbstract()
    {}

  virtual CcStatus open(EOpenFlags eOpenFlags) = 0;
  virtual CcStatus close() = 0;
  virtual CcStatus cancel() = 0;
  virtual CcStatus claim(EOpenFlags eOpenFlags) = 0;
  virtual bool exists() = 0;

  virtual size_t read(void* buffer, size_t size) = 0;
  virtual size_t write(const void* buffer, size_t size) = 0;
};

#endif // CcSharedMemoryAbstract_H_
