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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IFsDevice
 */

#include "IFsDevice.h"
#include "CcRequest.h"
#include "CcConnection.h"
#include "IDeviceContext.h"
#include "CcRequestContext.h"
#include "IDeviceInterface.h"
#include "IDeviceInterfaceContext.h"
#include <ntdddisk.h>

namespace NKernelModule
{

IFsDevice::IFsDevice(IDriver* pDriver) :
  IDevice(pDriver, EType::FileSystem)
{
  if (getStatus())
  {
    IoRegisterFileSystem(getContext());
    ObReferenceObject(getContext());
    CCDEBUG("IFsDevice created");
  }
}

IFsDevice::~IFsDevice()
{
  ObDereferenceObject(getContext());
}

void IFsDevice::open(CcRequest& oRequest)
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

void IFsDevice::shutdown(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDevice::cleanup(CcRequest& oRequest)
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

void IFsDevice::close(CcRequest& oRequest)
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

void IFsDevice::read(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDevice::write(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDevice::powerControl(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDevice::ioControl(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IFsDevice::specificControl(ESpecificRequests eRequestType, CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);

  switch (eRequestType)
  {
    case ESpecificRequests::FileystemControl:
      switch (oRequest.getContext()->pStackLocation->MinorFunction)
      {
        case IRP_MN_VERIFY_VOLUME:
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

}
