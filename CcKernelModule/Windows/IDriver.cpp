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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IIo
 */

#include "IDriver.h"
#include "CcKernelModuleContext.h"
#include "CcRequest.h"
#include "CcMalloc.h"
#include "IDevice.h"
#include "IDeviceContext.h"
#include "IDriverContext.h"
#include "CcConnection.h"
#include <ntddk.h>
#include <wdf.h>

namespace NKernelModule
{
  
_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_CREATE)
NTSTATUS Driver_Create(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->open(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_CLEANUP)
NTSTATUS Driver_Cleanup(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->cleanup(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_CLOSE)
NTSTATUS Driver_Close(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->close(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_READ)
NTSTATUS Driver_Read(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->read(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_WRITE)
NTSTATUS Driver_Write(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->write(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
NTSTATUS Driver_DeviceControl(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->ioControl(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_SHUTDOWN)
NTSTATUS Driver_Shutdown(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->shutdown(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_PNP)
NTSTATUS Driver_Pnp(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->powerControl(oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_QUERY_INFORMATION)
NTSTATUS Driver_QueryInformation(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::QueryInformation, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_SET_INFORMATION)
NTSTATUS Driver_SetInformation(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::SetInformation, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_QUERY_EA)
NTSTATUS Driver_QueryEa(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::QueryEa, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_SET_EA)
NTSTATUS Driver_SetEa(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::SetEa, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_FLUSH_BUFFERS)
NTSTATUS Driver_FlushBuffers(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::FlushBuffers, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_QUERY_VOLUME_INFORMATION)
NTSTATUS Driver_QueryVolumeInformation(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::QueryVolumeInformation, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_SET_VOLUME_INFORMATION)
NTSTATUS Driver_SetVolumeInformation(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::SetVolumeInformation, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_DIRECTORY_CONTROL)
NTSTATUS Driver_DirectoryControl(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::DirectoryControl, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_FILE_SYSTEM_CONTROL)
NTSTATUS Driver_FileSystemControl(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::FileystemControl, oRequest);
  }
  return oRequest.getStatus();
}

_Function_class_(DRIVER_DISPATCH)
_Dispatch_type_(IRP_MJ_LOCK_CONTROL)
NTSTATUS Driver_LockControl(IDevice::CContext* pDeviceObject, PIRP pIrp)
{
  CcRequest oRequest(pIrp);
  if (pDeviceObject->pDevice)
  {
    pDeviceObject->pDevice->specificControl(IDevice::ESpecificRequests::LockControl, oRequest);
  }
  return oRequest.getStatus();
}

IDriver::IDriver(CcKernelModuleContext* pContext)
{
  CCNEW(m_pContext, CContext);
  m_pContext->pDriver = this;
  m_pContext->pDriverObject = pContext->pDriverObject;
  m_pContext->sRegistryPath.set(pContext->pRegistryPath->Buffer, pContext->pRegistryPath->Length);

  PDRIVER_OBJECT pDriverObject = m_pContext->pDriverObject;
  pDriverObject->MajorFunction[IRP_MJ_CREATE]   = (PDRIVER_DISPATCH)Driver_Create;
  pDriverObject->MajorFunction[IRP_MJ_CLEANUP]  = (PDRIVER_DISPATCH)Driver_Cleanup;
  pDriverObject->MajorFunction[IRP_MJ_CLOSE]    = (PDRIVER_DISPATCH)Driver_Close;
  pDriverObject->MajorFunction[IRP_MJ_READ]     = (PDRIVER_DISPATCH)Driver_Read;
  pDriverObject->MajorFunction[IRP_MJ_WRITE]    = (PDRIVER_DISPATCH)Driver_Write;
  pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)Driver_DeviceControl;
  pDriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = (PDRIVER_DISPATCH)Driver_Shutdown;
  pDriverObject->MajorFunction[IRP_MJ_PNP] = (PDRIVER_DISPATCH)Driver_Pnp;
  pDriverObject->MajorFunction[IRP_MJ_QUERY_INFORMATION]        = (PDRIVER_DISPATCH)Driver_QueryInformation;
  pDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION]          = (PDRIVER_DISPATCH)Driver_SetInformation;
  pDriverObject->MajorFunction[IRP_MJ_QUERY_EA]                 = (PDRIVER_DISPATCH)Driver_QueryEa;
  pDriverObject->MajorFunction[IRP_MJ_SET_EA]                   = (PDRIVER_DISPATCH)Driver_SetEa;
  pDriverObject->MajorFunction[IRP_MJ_FLUSH_BUFFERS]            = (PDRIVER_DISPATCH)Driver_FlushBuffers;
  pDriverObject->MajorFunction[IRP_MJ_QUERY_VOLUME_INFORMATION] = (PDRIVER_DISPATCH)Driver_QueryVolumeInformation;
  pDriverObject->MajorFunction[IRP_MJ_SET_VOLUME_INFORMATION]   = (PDRIVER_DISPATCH)Driver_SetVolumeInformation;
  pDriverObject->MajorFunction[IRP_MJ_DIRECTORY_CONTROL]        = (PDRIVER_DISPATCH)Driver_DirectoryControl;
  pDriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL]      = (PDRIVER_DISPATCH)Driver_FileSystemControl;
  pDriverObject->MajorFunction[IRP_MJ_LOCK_CONTROL]             = (PDRIVER_DISPATCH)Driver_LockControl;
}

IDriver::~IDriver()
{
  CCDELETE(m_pContext);
}

IDevice* IDriver::createDevice()
{
  CCNEWTYPE(pDevice, IDevice, this, IDevice::EType::Basic);
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