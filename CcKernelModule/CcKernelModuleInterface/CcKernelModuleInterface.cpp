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

void WINCEXPORT operator delete(void *p) _GLIBCXX_USE_NOEXCEPT
{
    CcMalloc_free(p);
}

void WINCEXPORT operator delete[](void *p) _GLIBCXX_USE_NOEXCEPT
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
