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
 * @brief     Implemtation of class CcKernelModule
 */

#include "CcKernelModule.h"
#include "CcMalloc.h"
#include "CcString.h"

CcKernelModule* pModule = nullptr;

void CcKernelModule::testLoad()
{
  CcString sMessage("CcKernelModule is loading\n");
  CcMalloc_print(sMessage.getCharString());
  pModule = new CcKernelModule();
}

void CcKernelModule::testUnload()
{
  delete pModule;
  CcMalloc_print("CcKernelModule is unloading\n");
}

CCEXTERNC_BEGIN
void CcKernelModule_load()
{
  CcKernelModule::testLoad();
}

void CcKernelModule_unload()
{
  CcKernelModule::testUnload();
}
CCEXTERNC_END

void* WINCEXPORT operator new(size_t sz) _GLIBCXX_THROW(std::bad_alloc)
{
    return CcMalloc_malloc(sz);
}

void* WINCEXPORT operator new[](size_t sz) _GLIBCXX_THROW(std::bad_alloc)
{
    return CcMalloc_malloc(sz);
}

void WINCEXPORT operator delete(void *p) NOEXCEPT_IMPLICIT
{
    CcMalloc_free(p);
}

void WINCEXPORT operator delete[](void *p) NOEXCEPT_IMPLICIT
{
    CcMalloc_free(p);
}

void terminate()
{
    //kern_log("terminate requested\n");
}

extern "C" void __cxa_pure_virtual()
{
    //kern_log("cxa_pure_virtual error handler\n");
}

extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{
  CCUNUSED(destructor);
  CCUNUSED(arg);
  CCUNUSED(dso);
  return 0;
}

extern "C" void __cxa_finalize(void *f)
{
  CCUNUSED(f);
}

void* __dso_handle = nullptr;
