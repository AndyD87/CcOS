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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class IFsDriver
 */
#include "IFsDriver.h"
#include "IDevice.h"
#include "CcRequest.h"
#include "CcConnection.h"
#include "IDeviceContext.h"
#include "CcRequestContext.h"
#include "IDeviceInterface.h"
#include "IDeviceInterfaceContext.h"
#include <ntdddisk.h>

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
  
  virtual void open(CcRequest& oRequest);
  virtual void shutdown(CcRequest& oRequest);
  virtual void cleanup(CcRequest& oRequest);
  virtual void close(CcRequest& oRequest);
  virtual void read(CcRequest& oRequest);
  virtual void write(CcRequest& oRequest);
  virtual void powerControl(CcRequest& oRequest);
  virtual void ioControl(CcRequest& oRequest);
  virtual void specificControl(IDevice::ESpecificRequests eRequestType, CcRequest& oRequest);

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
  ObDereferenceObject(getContext());
}

CcStatus IFsDriverDevice::start()
{
  CcStatus oStatus = IDevice::start();
  if (oStatus)
  {
    IoRegisterFileSystem(getContext());
    ObReferenceObject(getContext());
    CCDEBUG("IFsDriverDevice created");
  }
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

  switch (eRequestType)
  {
    case ESpecificRequests::FileystemControl:
      switch (oRequest.getContext()->pStackLocation->MinorFunction)
      {
        case IRP_MN_USER_FS_REQUEST:
          dbgBreak();
          oRequest.setStatus(STATUS_INVALID_DEVICE_REQUEST);
          break;
        case IRP_MN_MOUNT_VOLUME:
          checkMountVolume(oRequest);
          break;
        case IRP_MN_VERIFY_VOLUME:
          dbgBreak();
          oRequest.setStatus(STATUS_INVALID_DEVICE_REQUEST);
          break;

        default:
          dbgBreak();
          oRequest.setStatus(STATUS_INVALID_DEVICE_REQUEST);
          break;
      }
      break;
    default:
      dbgBreak();
      oRequest.setStatus(STATUS_INVALID_DEVICE_REQUEST);
      break;
  }
}

void IFsDriverDevice::checkMountVolume(CcRequest& oRequest)
{
  PVPB pVpb = oRequest.getContext()->pStackLocation->Parameters.MountVolume.Vpb;
  PDEVICE_OBJECT pLowerDevice = oRequest.getContext()->pStackLocation->Parameters.MountVolume.DeviceObject;

  if (pVpb && pLowerDevice)
  {
    IDeviceInterface oInterface(static_cast<IDeviceInterface::CContext*>(pLowerDevice));
    ULONG uiChangeCount = 0;
    CcRequest oDeviceRequest = oInterface.createRequest(IOCTL_STORAGE_CHECK_VERIFY, NULL, 0, &uiChangeCount, sizeof(uiChangeCount));
    CcStatus oStatus = oInterface.sendRequest(oDeviceRequest);
    oInterface.removeRequest(oDeviceRequest);
    if (oStatus)
    {
      // We have a disk
      if (m_pFsDriver->checkDevice(oInterface))
      {
        m_pFsDriver->createDevice(oInterface);
      }
      else
      {
        oRequest.setStatus(STATUS_UNRECOGNIZED_VOLUME);
      }
    }
    else
    {
      oRequest.setStatus(STATUS_UNRECOGNIZED_VOLUME);
    }
  }
  else
  {
    oRequest.setStatus(STATUS_INVALID_PARAMETER);
  }
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