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

namespace NKernelModule
{

CcRequest::CcRequest(void* CCUNUSED_PARAM(pSystemContext))
{
  CCNEW(m_pContext, CContext);
}

CcRequest::CcRequest(const CcRequest& CCUNUSED_PARAM(oToCopy))
{
  CCNEW(m_pContext, CContext);
}

CcRequest::~CcRequest()
{
  CCDELETE(m_pContext);
}

CcRequest& CcRequest::operator=(const CcRequest& CCUNUSED_PARAM(oToCopy))
{
  return *this;
}

void CcRequest::finish()
{
}

CcConnection* CcRequest::getConnection() const
{
  CcConnection* pConnection = nullptr;
  return pConnection;
}

int32 CcRequest::getStatus()
{
  int32 iStatus = 0;
  return iStatus;
}

size_t CcRequest::getSize()
{
  size_t uiSize = 0;
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
  CCUNUSED(uiMinSize);
  return pBuffer;
}

size_t CcRequest::getInputBufferSize()
{
  size_t uiSize = 0;
  return uiSize;
}

void* CcRequest::getOutputBuffer(size_t uiMinSize)
{
  void* pBuffer = NULL;
  CCUNUSED(uiMinSize);
  return pBuffer;
}

size_t CcRequest::getOutputBufferSize()
{
  size_t uiSize = 0;
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
  return bRet;
}

void CcRequest::setConnection(CcConnection* CCUNUSED_PARAM(pNewConnection))
{
}

void CcRequest::setStatus(int32 CCUNUSED_PARAM(iStatus), size_t CCUNUSED_PARAM(uiSize))
{
}

void CcRequest::setStatus(const CcStatus& eStatus)
{
  switch (eStatus.getError())
  {
    case EStatus::AllOk:
      setStatus(0, getSize());
      break;
    case EStatus::NotSupported:
      setStatus(1, getSize());
      break;
    case EStatus::TimeoutReached:
      setStatus(2, getSize());
      break;
    case EStatus::MoreProcessingRequired:
      setStatus(3, getSize());
      break;
    default:
      // Unkown or not convertable error code
      setStatus(1, getSize());
  }
}

void CcRequest::setPending()
{
}

}
