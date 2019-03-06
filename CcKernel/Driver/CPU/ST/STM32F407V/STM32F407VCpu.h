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
 * @page      STM32F407V
 * @subpage   STM32F407VCpu
 * 
 * @page      STM32F407VCpu
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F407VCpu
 */
#ifndef _CCLIB_STM32F407VCpu_H_
#define _CCLIB_STM32F407VCpu_H_

#include "CcBase.h"
#include "Devices/ICpu.h"

class STM32F407VCpu : public ICpu
{
public: // methods
  STM32F407VCpu();
  virtual ~STM32F407VCpu();

  virtual size_t coreNumber() override;
  virtual void* captureRegisters(size_t uiCoreNr) override;
  virtual void  restoreRegisters(size_t uiCoreNr, void* pState) override;
  virtual void  delteRegisters(void* pState) override;

private: // types
  class STM32F407VCpuPrivate;
  class STM32F407VCpuRegisters;
private: // member
  STM32F407VCpuPrivate* m_pPrivate;
};

#endif /* _CCLIB_STM32F407VCpu_H_ */
