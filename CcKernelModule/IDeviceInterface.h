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
 * @brief     Interface IDeviceInterface
 */
#pragma once

#include "CcBase.h"
#include "CcKernelModule.h"
#include "CcList.h"

namespace NKernelModule
{

class CcRequest;

/**
 * @brief Interface to devices
 */
class CcKernelModuleSHARED IDeviceInterface
{
public:
  class CContext;

  IDeviceInterface() = default;

  /**
   * @brief Create device with existing interface context from system.
   * @param pContext: Context generated from subsystem
   */
  IDeviceInterface(CContext* pContext);

  /**
   * @brief Create request for sending
   * @param IoControlCode:        Target io code to send
   * @param pInputBuffer:         Input buffer along with request
   * @param uiInputBufferSize:    Size of @p pInputBuffer
   * @param pOutputBuffer:        Output buffer along with request
   * @param uiOutputBufferSize:   Size of @p pOutputBuffer
   * @return Generated request to transfer with interface
   */
  CcRequest createRequest(uint32 IoControlCode,
                          void* pInputBuffer,
                          uint32 uiInputBufferSize,
                          void* pOutputBuffer,
                          uint32 uiOutputBufferSize);

  /**
   * @brief Send request, generated with @ref createRequest
   * @param oRequest: Request to send
   * @return Status of opeartion.
   */
  CcStatus sendRequest(CcRequest& oRequest);

  /**
   * @brief Remove generated request with @ref createRequest
   * @param oRequest: Request to cleanup
   */
  void removeRequest(CcRequest& oRequest);

protected:
  //! @return Get interface context from subsystem
  CContext* getContext()
  { return m_pContext; }
private:
  CContext* m_pContext = nullptr;
};

}
