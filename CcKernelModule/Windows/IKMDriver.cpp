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
 * @brief     Class IIo
 */

#include "IKMDriver.h"
#include "CcKernelModuleContext.h"
#include "CcMalloc.h"
#include <ntddk.h>
#include <wdf.h>

typedef struct
{
  IKMDriver* pKMDriver;
} IKMDriver_Context;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(IKMDriver_Context, IKMDriver_GetContext);

class IKMDriver::CContext
{
public:
  CContext()
  {}

  static NTSTATUS EvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
  )
  {
    NTSTATUS uStatus = STATUS_SUCCESS;
    IKMDriver_Context* pDriverContext = IKMDriver_GetContext(Driver);
    CCUNUSED(DeviceInit);
    CCUNUSED(pDriverContext);
    return uStatus;
  }

  WDFDRIVER  oDriver;
};

IKMDriver::IKMDriver(CcKernelModuleContext* pContext)
{
  // Allocate the driver configuration object
  WDF_DRIVER_CONFIG oConfig;

  // Initialize the driver configuration object to register the
  // entry point for the EvtDeviceAdd callback, KmdfHelloWorldEvtDeviceAdd
  WDF_DRIVER_CONFIG_INIT(
    &oConfig,
    IKMDriver::CContext::EvtDeviceAdd
  );

  WDF_OBJECT_ATTRIBUTES oAttributes;
  WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&oAttributes, IKMDriver_Context);

  pContext->oDriverStatus = WdfDriverCreate(pContext->pDriverObject,
                                    pContext->pRegistryPath,
                                    &oAttributes,
                                    &oConfig,
                                    &m_pContext->oDriver
  );
}

IKMDriver::~IKMDriver()
{
  CCDELETE(m_pContext);
}
