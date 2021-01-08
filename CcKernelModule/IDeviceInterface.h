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
 * @page      CcKernelModule
 * @subpage   IDeviceInterface
 *
 * @page      IDeviceInterface
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Interface IDeviceInterface
 */

#ifndef H_IDeviceInterface_H_
#define H_IDeviceInterface_H_

#include "CcBase.h"
#include "CcKernelModule.h"
#include "CcList.h"

namespace NKernelModule
{

class CcRequest;

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelModuleSHARED IDeviceInterface
{
public:
  class CContext;

  IDeviceInterface() = default;
  IDeviceInterface(CContext* pContext);

  CcRequest createRequest(uint32 IoControlCode,
                          void* pInputBuffer,
                          uint32 uiInputBufferSize,
                          void* pOutputBuffer,
                          uint32 uiOutputBufferSize);
  CcStatus sendRequest(CcRequest& oRequest);
  void removeRequest(CcRequest& oRequest);

protected:
  CContext* getContext()
  { return m_pContext; }
private:
  CContext* m_pContext = nullptr;
};

}

#endif // _IDeviceInterface_H_
