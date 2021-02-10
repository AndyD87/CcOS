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
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Interface IFsDevice
 */
#pragma once

#include "CcBase.h"
#include "CcKernelModule.h"
#include "CcList.h"
#include "IDevice.h"

namespace NKernelModule
{

class IDriver;
class CcConnection;
class CcRequest;

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class IFsDevice : public IDevice
{
public:
  /**
   * @brief Initialice Filesystem device with parent driver handle
   * @param pDriver: Pointer to parent driver object
   */
  IFsDevice(IDriver* pDriver);
  virtual ~IFsDevice();

  virtual CcStatus start() override;
  virtual CcStatus stop() override;

  virtual void open(CcRequest& oRequest) override;
  virtual void shutdown(CcRequest& oRequest) override;
  virtual void cleanup(CcRequest& oRequest) override;
  virtual void close(CcRequest& oRequest) override;
  virtual void read(CcRequest& oRequest) override;
  virtual void write(CcRequest& oRequest) override;
  virtual void powerControl(CcRequest& oRequest) override;
  virtual void ioControl(CcRequest& oRequest) override;
  virtual void specificControl(IDevice::ESpecificRequests eRequestType, CcRequest& oRequest) override;

private:
  void checkMountVolume(CcRequest& oRequest);
};

}
