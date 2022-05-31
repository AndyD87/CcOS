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
 * @brief     Implemtation of class IFsDriver
 */
#include "IFsDriver.h"
#include "CcString.h"
#include "IDevice.h"
#include "CcRequest.h"
#include "CcConnection.h"
#include "IDeviceContext.h"
#include "CcRequestContext.h"
#include "IDeviceInterface.h"
#include "IDeviceInterfaceContext.h"

namespace NKernelModule
{

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class IFsDriverDevice : public IDevice
{
public:
  IFsDriverDevice(IFsDriver* pDriver);
  virtual ~IFsDriverDevice();

  virtual CcStatus start() override;
  
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
  IFsDriver* m_pFsDriver;
};

IFsDriverDevice::IFsDriverDevice(IFsDriver* pDriver) :
  IDevice(pDriver, EType::FileSystem),
  m_pFsDriver(pDriver)
{
}

IFsDriverDevice::~IFsDriverDevice()
{
}

CcStatus IFsDriverDevice::start()
{
  CcStatus oStatus = IDevice::start();
  return oStatus;
}

void IFsDriverDevice::open(CcRequest& oRequest)
{
  // Is File?
  if (1)
  {
    IDevice::open(oRequest);
  }
  else
  {
    oRequest.setStatus(EStatus::NotStarted);
  }
}

void IFsDriverDevice::shutdown(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDriverDevice::cleanup(CcRequest& oRequest)
{
  // Is File?
  if (1)
  {
    IDevice::close(oRequest);
  }
  else
  {
    oRequest.setStatus(EStatus::NotStarted);
  }
}

void IFsDriverDevice::close(CcRequest& oRequest)
{
  // Is File?
  if (1)
  {
    IDevice::close(oRequest);
  }
  else
  {
    oRequest.setStatus(EStatus::NotStarted);
  }
}

void IFsDriverDevice::read(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDriverDevice::write(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDriverDevice::powerControl(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDriverDevice::ioControl(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDriverDevice::specificControl(ESpecificRequests eRequestType, CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDriverDevice::checkMountVolume(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

IFsDriver::IFsDriver(CcKernelModuleContext* pContext) :
  IDriver(pContext)
{
  // Filesystem driver do net have hardware, initialize them here
  createDevice();
}

IFsDriver::~IFsDriver()
{
}

NKernelModule::IDevice* IFsDriver::createDevice()
{
  CCNEWTYPE(pDevice, IFsDriverDevice, this);
  if (pDevice->start())
  {
    CCDEBUG("Device created");
  }
  else
  {
    CCDELETE(pDevice);
  }
  return pDevice;
}

}
