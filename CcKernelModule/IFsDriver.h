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
 * @page      CcUtil
 * @subpage   IFsDriver
 *
 * @page      IFsDriver
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IFsDriver
 **/
#ifndef H_IFsDriver_H_
#define H_IFsDriver_H_

#include "CcBase.h"
#include "IDriver.h"

namespace NKernelModule
{

class IFsDevice;
class IDeviceInterface;

/**
 * @brief Class impelmentation
 */
class IFsDriver : public NKernelModule::IDriver
{
public:
  /**
   * @brief Constructor
   */
  IFsDriver(CcKernelModuleContext* pContext);

  /**
   * @brief Destructor
   */
  virtual ~IFsDriver();

  //! Check if device it maches with our filesystem requirements
  virtual bool checkDevice(IDeviceInterface& oTargetDevice) = 0;
  //! Create device
  virtual NKernelModule::IFsDevice* createDevice(IDeviceInterface& oTargetDevice) = 0;
  virtual NKernelModule::IDevice*   createDevice() override final;
};

}

#endif // H_IFsDriver_H_
