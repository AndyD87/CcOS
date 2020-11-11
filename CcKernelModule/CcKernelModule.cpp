#include "CcKernelModule.h"
#include "CcMalloc.h"

CcKernelModule* pModule = nullptr;

void CcKernelModule::testLoad()
{
  CcMalloc_print("CcKernelModule is loading\n");
  pModule = new CcKernelModule();
}

void CcKernelModule::testUnload()
{
  delete pModule;
  CcMalloc_print("CcKernelModule is unloading\n");
}

void CcKernelModule_load()
{
  CcKernelModule::testLoad();
}

void CcKernelModule_unload()
{
  CcKernelModule::testUnload();
}

#ifndef _GLIBCXX_THROW
#define _GLIBCXX_THROW(BLAH)
#endif
#ifndef _GLIBCXX_USE_NOEXCEPT
#define _GLIBCXX_USE_NOEXCEPT
#endif

void *operator new(size_t sz) _GLIBCXX_THROW(std::bad_alloc)
{
    return CcMalloc_malloc(sz);
}

void *operator new[](size_t sz) _GLIBCXX_THROW(std::bad_alloc)
{
    return CcMalloc_malloc(sz);
}

void operator delete(void *p) _GLIBCXX_USE_NOEXCEPT
{
    CcMalloc_free(p);
}

void operator delete[](void *p) _GLIBCXX_USE_NOEXCEPT
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
