#include "../../CcKernel/CcBase.h"

#ifndef KERN_INFO
  #define KERN_INFO "6"
#endif

CCEXTERNC_BEGIN
extern void* CcMalloc_malloc(size_t uiSize);
extern void CcMalloc_free(void* pBuffer);
extern void CcMalloc_print(const char* pFormat, ...);
CCEXTERNC_END
