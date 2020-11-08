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

void *operator new(size_t sz) throw ()
{
    return CcMalloc_malloc(sz);
}

void *operator new[](size_t sz) throw ()
{
    return CcMalloc_malloc(sz);
}

void operator delete(void *p)
{
    CcMalloc_free(p);
}

void operator delete[](void *p)
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
