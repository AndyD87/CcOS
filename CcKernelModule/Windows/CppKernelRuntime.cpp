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
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtations for initializing and working with cpp on Windows Kernels
 *            The kernel module has to be build with CppKernelRuntime_Init as init function
 *
 *            The basic informations are from http://www.zer0mem.sk/?p=517
 */

// Infos from http://www.zer0mem.sk/?p=517
#include <ntddk.h>

#include "CcBase.h"
#include "CcMalloc.h"
#include "CcKernelModule.h"
#include "CcKernelModuleContext.h"

extern "C" DRIVER_INITIALIZE FxDriverEntry;           //!< Wdf Driver Entry function
extern "C" DRIVER_INITIALIZE CppKernelRuntime_Init;
extern "C" DRIVER_UNLOAD     CppKernelRuntime;

static CcKernelModuleContext CppKernelRuntime_Context;

typedef void(WINCEXPORT *CppKernelRuntime_CrtFunction)();

#pragma data_seg()
#if defined(_IA64_) || defined(_AMD64_)
#pragma section(".CRT$XCA",long,read)
__declspec(allocate(".CRT$XCA")) void(*CppKernelRuntime_InitFunctionsBegin[1])(void) = {0};
#pragma section(".CRT$XCZ",long,read)
__declspec(allocate(".CRT$XCZ")) void(*CppKernelRuntime_InitFunctionsEnd[1])(void) = {0};
#pragma data_seg()
#else
#pragma data_seg(".CRT$XCA")
CppKernelRuntime_CrtFunction CppKernelRuntime_InitFunctionsBegin[1];
#pragma data_seg(".CRT$XCZ")
CppKernelRuntime_CrtFunction CppKernelRuntime_InitFunctionsEnd[1];
#pragma data_seg()
#endif

CCEXTERNC_BEGIN

typedef struct
{
  LIST_ENTRY            link;       // linking fields
  CppKernelRuntime_CrtFunction  f;  // function to call during exit processing
} CppKernelRuntime_SExitListItem, *PCppKernelRuntime_SExitListItem;

static KSPIN_LOCK     CppKernelRuntime_oExitLock; // spin lock to protect atexit list
static LIST_ENTRY     CppKernelRuntime_oExitList; // anchor of atexit list
static DRIVER_UNLOAD* CppKernelRuntime_pOldUnload = nullptr;

CCEXTERNC_END

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING psRegistryPath)
{
  CcMalloc_print("Kernel print");
  DbgPrintEx(DPFLTR_CLASSPNP_ID, DPFLTR_ERROR_LEVEL, "Kernel print2");

  // Initialize static destructors
  InitializeListHead(&CppKernelRuntime_oExitList);
  KeInitializeSpinLock(&CppKernelRuntime_oExitLock);

  // For Debug store begin and end in Variables
  CppKernelRuntime_CrtFunction* pBegin = CppKernelRuntime_InitFunctionsBegin + 1;
  CppKernelRuntime_CrtFunction* pEnd = CppKernelRuntime_InitFunctionsEnd;
  // Initialize all static variables
  for (; pBegin < pEnd; ++pBegin)
  {
    (*pBegin)();
  }

  CppKernelRuntime_Context.pDriverObject = DriverObject;
  CppKernelRuntime_Context.pRegistryPath = psRegistryPath;

  // Call Module load
  CcKernelModule_load(&CppKernelRuntime_Context);
  
  // @TODO Start Driver: NTSTATUS uStatus = main(0, nullptr);
  NTSTATUS uStatus = 0;

  // Backup WDF Driver Unload
  CppKernelRuntime_pOldUnload = DriverObject->DriverUnload;
  // Set own Driver Unload to unload static variables and free thier entries.
  DriverObject->DriverUnload = CppKernelRuntime;

  return uStatus;
}

extern "C" VOID CppKernelRuntime(DRIVER_OBJECT *DriverObject)
{
  // Call Module unload
  CcKernelModule_unload(&CppKernelRuntime_Context);
  
  PCppKernelRuntime_SExitListItem p;
  while ( (p = (PCppKernelRuntime_SExitListItem)ExInterlockedRemoveHeadList(&CppKernelRuntime_oExitList, &CppKernelRuntime_oExitLock)) != nullptr)
  {
    (*p->f)();
    CcMalloc_free(p);
  }
  DriverObject->DriverUnload = CppKernelRuntime_pOldUnload;
  if (CppKernelRuntime_pOldUnload != nullptr)
    (*CppKernelRuntime_pOldUnload)(DriverObject);
}

extern "C" int WINCEXPORT atexit(CppKernelRuntime_CrtFunction f)
{
  PCppKernelRuntime_SExitListItem p = (PCppKernelRuntime_SExitListItem)CcMalloc_malloc(sizeof(CppKernelRuntime_SExitListItem));
  if (!p)
    return 0;

  p->f = f;
  ExInterlockedInsertTailList(&CppKernelRuntime_oExitList, &p->link, &CppKernelRuntime_oExitLock);

  return 1;
}
