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
 * @brief     Struct _CcKernelModuleContext
 **/
#pragma once 

#include <ntifs.h>

#include <CcWString.h>
#include "CcKernelModule.h"

struct CcKernelModuleSHARED _CcKernelModuleContext
{
public:
  PDRIVER_OBJECT  pDriverObject;  //!< Object from driver entry
  PUNICODE_STRING pRegistryPath;  //!< Register path from driver entry
  NTSTATUS        oDriverStatus;  //!< Global state of driver.
};

//! Required for windows driver framework
CCEXTERNC float _fltused;

//! NTDevice InitPath
#define NT_DEVICE_INIT                  L"\\Device\\"
