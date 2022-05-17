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
 * @brief     Class CcRequest
 **/
#pragma once

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

  /**
   * @brief Copy request info from another request
   * @param oToCopy: Request to copy from
   */
  CcRequest(const CcRequest& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcRequest();

  /**
   * @brief Copy request info from another request
   * @param oToCopy: Request to copy from
   * @return Handle to this
   */
  CcRequest& operator=(const CcRequest& oToCopy);

  /**
   * @brief Finish request and cleanup
   */
  void finish();

  //! @return Get parent connection of request
  CcConnection* getConnection() const;
  //! @return Get status of request
  int32 getStatus();
  //! @return Get size of request
  size_t getSize();
  //! @return Get io code from request
  uint32 getIoCode();
  //! @param uiMinSize: Minimum size of buffer
  //! @return Get input transfer buffer if it's size is lower than
  //!         uiMinSize, otherwise nullptr.
  void* getInputBuffer(size_t uiMinSize = SIZE_MAX);
  //! @return Get size of input buffer
  size_t getInputBufferSize();
  //! @param uiMinSize: Minimum size of buffer
  //! @return Get output transfer buffer if it's size is lower than
  //!         uiMinSize, otherwise nullptr.
  void* getOutputBuffer(size_t uiMinSize = SIZE_MAX);
  //! @return Get size of output buffer
  size_t getOutputBufferSize();
  //! @return Get offset of transfer
  uint64 getOffset();

  //! @return True if request is marked as pending, it will not be finished
  //!         until this flag is set
  bool isPending();

  //! @brief Set status to pending and prevent finish request
  void setPending();
  //! @param pNewConnection: Change parent connection
  void setConnection(CcConnection* pNewConnection);
  //! @param iStatus: Status of request
  //! @param uiSize:  Size of output buffer
  void setStatus(int32 iStatus, size_t uiSize = 0);
  //! @param eStatus: Set known status and translate to system code
  void setStatus(const CcStatus& eStatus);

  class CContext;
  //! @return Get context of this request.
  CContext* getContext()
  { return m_pContext;  }
private:
  CContext*     m_pContext = nullptr;
};

}
