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
 * @brief     Class IDevice
 */

#include "IDevice.h"
#include "CcRequest.h"
#include "CcConnection.h"
#include "IDeviceContext.h"
#include "IDriverContext.h"

namespace NKernelModule
{

CcStatus IDevice::start()
{
  CcStatus oStatus;
  // Create the device object for disks.  To avoid problems with filters who
  // know this name, we must keep it.
  uint32 uiDriverType = FILE_DEVICE_UNKNOWN;
  UNICODE_STRING oDeviceName = { 0 };
  switch (m_eType)
  {
    case EType::Basic:
      uiDriverType = FILE_DEVICE_UNKNOWN;
      break;
    case EType::FileSystem:
      uiDriverType = FILE_DEVICE_DISK_FILE_SYSTEM;
      break;
    case EType::Disk:
      uiDriverType = FILE_DEVICE_DISK;
      RtlInitUnicodeString(&oDeviceName, NT_DEVICE_INIT L"CcOSDiskDevice");
      break;
    default:
      oStatus.setError(EStatus::CommandUnknownParameter);
  }
  NTSTATUS iStatus = IoCreateDevice(m_pDriver->getContext()->pDriverObject,
                                    sizeof(CContext) - sizeof(DEVICE_OBJECT),
                                    &oDeviceName,
                                    uiDriverType,
                                    0,
                                    FALSE,
                                    reinterpret_cast<DEVICE_OBJECT**>(&m_pContext)
  );

  if (NT_SUCCESS(iStatus))
  {
    CCDEBUG("New device created");
    m_pContext->pDevice = this;
  }
  else
  {
    CCERROR("New device creation failed");
    oStatus.setSystemError(iStatus);
  }
  return oStatus; 
}

CcStatus IDevice::stop()
{
  CcStatus oStatus;
  return oStatus;
}

void IDevice::open(CcRequest& oRequest)
{
  if (oRequest.getConnection())
  {
    oRequest.setStatus(EStatus::AlreadyStarted);
  }
  else
  {
    CCNEWTYPE(pNewConnection, CcConnection, this);
    oRequest.setConnection(pNewConnection);
    m_pContext->getConnections()->append(pNewConnection);
  }
}

void IDevice::shutdown(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IDevice::cleanup(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotStarted);
}

void IDevice::close(CcRequest& oRequest)
{
  if (oRequest.getConnection())
  {
    CcConnection* pConnection = oRequest.getConnection();
    if (m_pContext->getConnections()->removeItem(pConnection))
    {
      CCDELETE(pConnection);
    }
  }
  else
  {
    oRequest.setStatus(EStatus::NotStarted);
  }
}

void IDevice::read(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IDevice::write(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IDevice::powerControl(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IDevice::ioControl(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IDevice::specificControl(ESpecificRequests CCUNUSED_PARAM(eRequestType), CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

void IDevice::dbgBreak()
{
  DbgBreakPoint();
}

}
