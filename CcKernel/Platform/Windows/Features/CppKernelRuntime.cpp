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

#pragma comment(linker, "/ENTRY:\"CppKernelRuntime_Init\"")

#include <ntddk.h>

extern "C" DRIVER_INITIALIZE FxDriverEntry;           //!< Wdf Driver Entry function
extern "C" DRIVER_INITIALIZE CppKernelRuntime_Init;
extern "C" DRIVER_UNLOAD     CppKernelRuntime;

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, CppKernelRuntime_Init)
#endif

typedef void(__cdecl *CppKernelRuntime_CrtFunction)();

#define CppKernelRuntime__NewTag 'CcOs'

#pragma data_seg()
#if defined(_IA64_) || defined(_AMD64_)
#pragma section(".CRT$XCA",long,read)
__declspec(allocate(".CRT$XCA")) void(*CppKernelRuntime_InitFunctionsBegin[1])(void) = {0};
#pragma section(".CRT$XCZ",long,read)
__declspec(allocate(".CRT$XCZ")) void(*CppKernelRuntime_InitFunctionsEnd[1])(void) = {0};
#pragma data_seg()
#else
#pragma data_seg(".CRT$XCA")
void(*CppKernelRuntime_InitFunctionsBegin[1])(void) = {0};
#pragma data_seg(".CRT$XCZ")
void(*CppKernelRuntime_InitFunctionsEnd[1])(void) = {0};
#pragma data_seg()
#endif

typedef struct
{
  LIST_ENTRY            link;       // linking fields
  CppKernelRuntime_CrtFunction  f;  // function to call during exit processing
} CppKernelRuntime_SExitListItem, *PCppKernelRuntime_SExitListItem;

static float          CppKernelRuntime_fltused = 0.0;
static KSPIN_LOCK     CppKernelRuntime_oExitLock; // spin lock to protect atexit list
static LIST_ENTRY     CppKernelRuntime_oExitList; // anchor of atexit list
static DRIVER_UNLOAD* CppKernelRuntime_pOldUnload = nullptr;

extern "C" NTSTATUS CppKernelRuntime_Init(PDRIVER_OBJECT DriverObject, PUNICODE_STRING psRegistryPath)
{
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

  // Call WDF Driver Entry
  NTSTATUS uStatus = FxDriverEntry(DriverObject, psRegistryPath);

  // Backup WDF Driver Unload
  CppKernelRuntime_pOldUnload = DriverObject->DriverUnload;
  // Set own Driver Unload to unload static variables and free thier entries.
  DriverObject->DriverUnload = CppKernelRuntime;

  return uStatus;
}

extern "C" VOID CppKernelRuntime(DRIVER_OBJECT *DriverObject)
{
  PCppKernelRuntime_SExitListItem p;
  while ( (p = (PCppKernelRuntime_SExitListItem)ExInterlockedRemoveHeadList(&CppKernelRuntime_oExitList, &CppKernelRuntime_oExitLock)) != nullptr)
  {
    (*p->f)();
    ExFreePoolWithTag(p, CppKernelRuntime__NewTag);
  }
  if (CppKernelRuntime_pOldUnload != nullptr)
    (*CppKernelRuntime_pOldUnload)(DriverObject);
}

extern "C" int __cdecl atexit(CppKernelRuntime_CrtFunction f)
{
  PCppKernelRuntime_SExitListItem p = (PCppKernelRuntime_SExitListItem)
    ExAllocatePoolWithTag(NonPagedPoolNx, sizeof(CppKernelRuntime_SExitListItem), CppKernelRuntime__NewTag);
  if (!p)
    return 0;

  p->f = f;
  ExInterlockedInsertTailList(&CppKernelRuntime_oExitList, &p->link, &CppKernelRuntime_oExitLock);

  return 1;
}

void*__cdecl operator new(size_t uiSize)
{
  return ExAllocatePoolWithTag(NonPagedPoolNx, uiSize, CppKernelRuntime__NewTag);
}

void __cdecl operator delete(void* pBuffer)
{
  return ExFreePoolWithTag(pBuffer, CppKernelRuntime__NewTag);
}

void __cdecl operator delete(void* pBuffer, size_t uiSize)
{
  UNREFERENCED_PARAMETER(uiSize);
  return ExFreePoolWithTag(pBuffer, CppKernelRuntime__NewTag);
}

void* __cdecl operator new[](size_t uiSize)
{
  return ExAllocatePoolWithTag(NonPagedPoolNx, uiSize, CppKernelRuntime__NewTag);
}

void __cdecl operator delete[](void* pBuffer)
{
  return ExFreePoolWithTag(pBuffer, CppKernelRuntime__NewTag);
}

void __cdecl operator delete[](void* pBuffer, size_t uiSize)
{
  UNREFERENCED_PARAMETER(uiSize);
  ExFreePoolWithTag(pBuffer, CppKernelRuntime__NewTag);
}
