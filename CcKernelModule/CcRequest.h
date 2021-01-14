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
 *
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRequest
 **/
#ifndef H_CcRequest_H_
#define H_CcRequest_H_

#include "CcBase.h"

namespace NKernelModule
{

class CcConnection;

/**
 * @brief Class impelmentation
 */
class CcRequest
{
public:
  /**
   * @brief Constructor
   * @param pSystemContext: For example on windows, it will be an PIRP
   */
  CcRequest(void* pSystemContext);
  CcRequest(const CcRequest& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcRequest();

  CcRequest& operator=(const CcRequest& oToCopy);

  void finish();

  CcConnection* getConnection() const;
  int32 getStatus();
  size_t getSize();
  uint32 getIoCode();
  void* getInputBuffer(size_t uiMinSize = SIZE_MAX);
  size_t getInputBufferSize();
  void* getOutputBuffer(size_t uiMinSize = SIZE_MAX);
  size_t getOutputBufferSize();
  uint64 getOffset();

  bool isPending();

  void setPending();
  void setConnection(CcConnection* pNewConnection);
  void setStatus(int32 iStatus, size_t uiSize = 0);
  void setStatus(const CcStatus& eStatus);

  class CContext;
  CContext* getContext()
  { return m_pContext;  }
private:
  CContext*     m_pContext = nullptr;
};

}

#endif // H_CcRequest_H_
