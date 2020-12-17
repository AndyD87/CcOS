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

#include <wdm.h>

namespace NKernelModule
{

IDevice::IDevice(IDriver* pDriver)
{
  m_pContext->pDriver = pDriver;
  // Create the device object for disks.  To avoid problems with filters who
  // know this name, we must keep it.
  uint32 uiDriverType = FILE_DEVICE_UNKNOWN;
  switch (getType())
  {
    case EType::Basic:
      uiDriverType = FILE_DEVICE_UNKNOWN;
      break;
    case EType::FileSystem:
      uiDriverType = FILE_DEVICE_DISK_FILE_SYSTEM;
      break;
  }
  NTSTATUS iStatus = IoCreateDevice(pDriver->getContext()->pDriverObject,
                                       sizeof(CContext) - sizeof(DEVICE_OBJECT),
                                       nullptr,
                                       uiDriverType,
                                       0,
                                       FALSE,
                                       reinterpret_cast<DEVICE_OBJECT**>(&m_pContext)
  );
  if (NT_SUCCESS(iStatus))
  {
    m_pContext->iStatus = iStatus;
    m_pContext->pDevice = this;
    m_pContext->pDriver = pDriver;
  }
}

IDevice::~IDevice()
{

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

void IDevice::ioControl(CcRequest& oRequest)
{
  oRequest.setStatus(EStatus::NotSupported);
}

}
