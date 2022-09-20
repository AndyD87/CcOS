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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IIo
 */

#include "IDriver.h"
#include "CcKernelModuleContext.h"
#include "CcRequest.h"
#include "CcMalloc.h"
#include "IDevice.h"
#include "IDeviceContext.h"
#include "IDriverContext.h"
#include "CcConnection.h"

namespace NKernelModule
{

IDriver::IDriver(CcKernelModuleContext* CCUNUSED_PARAM(pContext))
{
  CCNEW(m_pContext, CContext);
  m_pContext->pDriver = this;
}

IDriver::~IDriver()
{
  CCDELETE(m_pContext);
}

IDevice* IDriver::createDevice()
{
  CCNEWTYPE(pDevice, IDevice, this, IDevice::EType::Basic);
  if (pDevice->start())
  {
    CCDEBUG("Device created");
  }
  else
  {
    CCDELETE(pDevice);
  }
  return pDevice;
}

}
