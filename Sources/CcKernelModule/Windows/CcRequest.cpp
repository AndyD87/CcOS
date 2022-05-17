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
 * @brief     Implemtation of class CcRequest
 */
#include "CcRequest.h"
#include "CcRequestContext.h"
#include <cstring>

namespace NKernelModule
{

CcRequest::CcRequest(void* pSystemContext)
{
  CCNEW(m_pContext, CContext);
  if (pSystemContext)
  {
    m_pContext->pIrp = static_cast<PIRP>(pSystemContext);
    if (m_pContext->pIrp)
    {
      m_pContext->pStackLocation = IoGetCurrentIrpStackLocation(m_pContext->pIrp);
    }
  }
}

CcRequest::CcRequest(const CcRequest& oToCopy)
{
  CCNEW(m_pContext, CContext);
  m_pContext->pIrp            = oToCopy.m_pContext->pIrp;
  m_pContext->pStackLocation  = oToCopy.m_pContext->pStackLocation;
}

CcRequest::~CcRequest()
{
  CCDELETE(m_pContext);
}

CcRequest& CcRequest::operator=(const CcRequest& oToCopy)
{
  m_pContext->pIrp            = oToCopy.m_pContext->pIrp;
  m_pContext->pStackLocation  = oToCopy.m_pContext->pStackLocation;
  return *this;
}

void CcRequest::finish()
{
  if (m_pContext->pIrp != nullptr)
  {
    if (m_pContext->pIrp->IoStatus.Status != STATUS_PENDING)
    {
      IoCompleteRequest(m_pContext->pIrp, IO_NO_INCREMENT);
      m_pContext->pIrp = nullptr;
    }
  }
}

CcConnection* CcRequest::getConnection() const
{
  CcConnection* pConnection = nullptr;
  if (m_pContext->pStackLocation)
  {
    pConnection = static_cast<CcConnection*>(m_pContext->pStackLocation->FileObject->FsContext2);
  }
  return pConnection;
}

int32 CcRequest::getStatus()
{
  int32 iStatus = 0;
  if (m_pContext->pIrp)
  {
    iStatus = m_pContext->pIrp->IoStatus.Status;
  }
  return iStatus;
}

size_t CcRequest::getSize()
{
  size_t uiSize = 0;
  if (m_pContext->pIrp)
  {
    uiSize = m_pContext->pIrp->IoStatus.Information;
  }
  return uiSize;
}

uint32 CcRequest::getIoCode()
{
  uint32 uiCode = 0;
  return uiCode;
}

void* CcRequest::getInputBuffer(size_t uiMinSize)
{
  void* pBuffer = NULL;
  if (m_pContext->pStackLocation != NULL &&
      m_pContext->pStackLocation->Parameters.DeviceIoControl.InputBufferLength >= uiMinSize)
  {
    if (m_pContext->pIrp->AssociatedIrp.SystemBuffer != NULL)
    {
      pBuffer = m_pContext->pIrp->AssociatedIrp.SystemBuffer;
    }
    else
    {
      if (m_pContext->pIrp->UserBuffer != NULL)
      {
        pBuffer = m_pContext->pIrp->UserBuffer;
      }
      else
      {
        if (m_pContext->pIrp->MdlAddress != NULL)
        {
          pBuffer = MmGetMdlVirtualAddress(m_pContext->pIrp->MdlAddress);
          if (pBuffer == NULL)
          {
            pBuffer = MmGetSystemAddressForMdlSafe(m_pContext->pIrp->MdlAddress, NormalPagePriority);
          }
        }
      }
    }
  }
  return pBuffer;
}

size_t CcRequest::getInputBufferSize()
{
  size_t uiSize = 0;
  if (m_pContext->pStackLocation != NULL)
  {
    uiSize = m_pContext->pStackLocation->Parameters.DeviceIoControl.InputBufferLength;
  }
  return uiSize;
}

void* CcRequest::getOutputBuffer(size_t uiMinSize)
{
  void* pBuffer = NULL;
  if (m_pContext->pStackLocation != NULL &&
      m_pContext->pStackLocation->Parameters.DeviceIoControl.OutputBufferLength >= uiMinSize)
  {
    if (m_pContext->pIrp->AssociatedIrp.SystemBuffer != NULL)
    {
      pBuffer = m_pContext->pIrp->AssociatedIrp.SystemBuffer;
    }
    else
    {
      if (m_pContext->pIrp->UserBuffer != NULL)
      {
        pBuffer = m_pContext->pIrp->UserBuffer;
      }
      else
      {
        if (m_pContext->pIrp->MdlAddress != NULL)
        {
          pBuffer = MmGetMdlVirtualAddress(m_pContext->pIrp->MdlAddress);
          if (pBuffer == NULL)
          {
            pBuffer = MmGetSystemAddressForMdlSafe(m_pContext->pIrp->MdlAddress, NormalPagePriority);
          }
        }
      }
    }
  }
  return pBuffer;
}

size_t CcRequest::getOutputBufferSize()
{
  size_t uiSize = 0;
  if (m_pContext->pStackLocation != NULL)
  {
    uiSize = m_pContext->pStackLocation->Parameters.DeviceIoControl.OutputBufferLength;
  }
  return uiSize;
}

uint64 CcRequest::getOffset()
{
  uint64 uiOffset = 0;
  return uiOffset;
}

bool CcRequest::isPending()
{
  bool bRet = false;
  if (getStatus() == STATUS_PENDING)
  {
    bRet = true;
  }
  return bRet;
}

void CcRequest::setConnection(CcConnection* pNewConnection)
{
  if (getConnection() != nullptr)
  {
    // Cleanup Connection Storage
  }
  if (m_pContext->pStackLocation)
  {
    m_pContext->pStackLocation->FileObject->FsContext2 = pNewConnection;
  }
}

void CcRequest::setStatus(int32 iStatus, size_t uiSize)
{
  if (m_pContext->pIrp)
  {
    m_pContext->pIrp->IoStatus.Status = iStatus;
    m_pContext->pIrp->IoStatus.Information = uiSize;
  }
}

void CcRequest::setStatus(const CcStatus& eStatus)
{
  switch (eStatus.getError())
  {
    case EStatus::AllOk:
      setStatus(STATUS_SUCCESS, getSize());
      break;
    case EStatus::NotSupported:
      setStatus(STATUS_NOT_SUPPORTED, getSize());
      break;
    case EStatus::TimeoutReached:
      setStatus(STATUS_IO_OPERATION_TIMEOUT, getSize());
      break;
    case EStatus::MoreProcessingRequired:
      setStatus(STATUS_PENDING, getSize());
      break;
    default:
      // Unkown or not convertable error code
      setStatus(STATUS_NOT_SUPPORTED, getSize());
  }
}

void CcRequest::setPending()
{
  if (m_pContext->pIrp)
  {
    setStatus(STATUS_PENDING, 0);
    IoMarkIrpPending(m_pContext->pIrp);
  }
}


}
