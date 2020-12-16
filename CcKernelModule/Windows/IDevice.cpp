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
  CCNEW(m_pContext, CContext);
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

  m_pContext->iStatus = IoCreateDevice(m_pContext->pDriver->getContext()->pDriverObject,
                                       0,
                                       nullptr,
                                       uiDriverType,
                                       0,
                                       FALSE,
                                       &m_pContext->pDevice
  );
}

IDevice::~IDevice()
{

}

CcStatus IDevice::open(CcRequest* pRequest)
{
  CcStatus oStatus = false;
  if (pRequest->getConnection())
  {
    oStatus = EStatus::AlreadyStarted;
  }
  else
  {
    CCNEWTYPE(pNewConnection, CcConnection, this);
    pRequest->setConnection(pNewConnection);
    m_pContext->oConnections.append(pNewConnection);
  }
  return oStatus;
}

CcStatus IDevice::close(CcRequest* pRequest)
{
  CcStatus oStatus = false;
  if (pRequest->getConnection())
  {
    CcConnection* pConnection = pRequest->getConnection();
    if (m_pContext->oConnections.removeItem(pConnection))
    {
      CCDELETE(pConnection);
    }
  }
  else
  {
    oStatus = EStatus::NotStarted;
  }
  return oStatus;
}

CcStatus IDevice::read(CcRequest* /*pRequest*/)
{
  CcStatus oStatus = EStatus::NotSupported;
  return oStatus;
}

CcStatus IDevice::write(CcRequest* /*pRequest*/)
{
  CcStatus oStatus = EStatus::NotSupported;
  return oStatus;
}

CcStatus IDevice::ioControl(CcRequest* /*pRequest*/)
{
  CcStatus oStatus = false;
  return oStatus;
}

}
