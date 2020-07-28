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
 * @subpage   IKernel
 *
 * @page      IKernel
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IKernel
 */
#ifndef H_IKernel_H_
#define H_IKernel_H_

#include "CcBase.h"
#include "IDevice.h"

/**
 * @brief This interface can share Kernel Resources with Modules and other
 *        parts which does not have direct access to it.
 *        An Instance of this module can be genareted from CcKernel::getInterface
 */
class CcKernelSHARED IKernel
{
public:
  IKernel() = default;
  IKernel(
    void(*addDevice)(CcDeviceHandle),
    void(*removeDevice)(CcDeviceHandle),
    void* (*opNew)(size_t),
    void(*opDel)(void*),
    void(*opDelMemory)(const void*)
  ) : 
    addDevice(addDevice), 
    removeDevice(removeDevice),
    opNew(opNew), 
    opDel(opDel),
    opDelMemory(opDelMemory)
  { }
  void (*addDevice)(CcDeviceHandle Device) = nullptr;     //!< Pointer to CcKernel::addDevice
  void (*removeDevice)(CcDeviceHandle Device) = nullptr;  //!< Pointer to CcKernel::removeDevice
  void* (*opNew)(size_t uiSize) = nullptr;                //!< Pointer to new operator in Kernel space
  void (*opDel)(void*) = nullptr;                         //!< Pointer to delete operator in Kernel space
  void (*opDelMemory)(const void*) = nullptr;             //!< Pointer to delete memory form MemoryManager 
};

#endif // H_IKernel_H_
