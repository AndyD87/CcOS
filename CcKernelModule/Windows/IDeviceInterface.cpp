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
 * @brief     Class IDeviceInterface
 */

#include "IDeviceInterface.h"
#include "CcRequest.h"
#include "CcConnection.h"
#include "IDeviceInterfaceContext.h"
#include "IDriverContext.h"
#include "CcRequestContext.h"

namespace NKernelModule
{
CcRequest IDeviceInterface::createRequest(uint32 uiIoControlCode,
                                          void* pInputBuffer, 
                                          uint32 uiInputBufferSize, 
                                          void* pOutputBuffer, 
                                          uint32 uiOutputBufferSize)
{
  CCNEWTYPE(pEvent, KEVENT);
  PIRP pIrp = IoBuildDeviceIoControlRequest(uiIoControlCode,
                                            m_pContext,
                                            pInputBuffer,
                                            uiInputBufferSize,
                                            pOutputBuffer,
                                            uiOutputBufferSize,
                                            TRUE,
                                            pEvent,
                                            nullptr
  );
  if (pIrp)
  {
    SetFlag(IoGetNextIrpStackLocation(pIrp)->Flags, SL_OVERRIDE_VERIFY_VOLUME);
  }
  CcRequest oRequest(pIrp);
  oRequest.getContext()->pEvent = pEvent;
  return oRequest;
}

CcStatus IDeviceInterface::sendRequest(CcRequest& oRequest)
{
  CcStatus oStatus;
  NTSTATUS iStatus = IoCallDriver(m_pContext, oRequest.getContext()->pIrp);
  if (iStatus == STATUS_PENDING)
  {
    (VOID)KeWaitForSingleObject(oRequest.getContext()->pEvent,
                                Executive,
                                KernelMode,
                                FALSE,
                                (PLARGE_INTEGER)NULL);
  }
  else if (!NT_SUCCESS(iStatus))
  {
    oStatus.setSystemError(iStatus);
  }
  return oStatus;
}

void IDeviceInterface::removeRequest(CcRequest& oRequest)
{
  CCDELETE(oRequest.getContext()->pEvent);
}

}
