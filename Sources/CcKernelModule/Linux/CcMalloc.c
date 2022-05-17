#include <linux/kernel.h>
#include <linux/slab.h>

#include "CcMalloc.h"

void* CcMalloc_malloc(size_t uiSize)
{
  return kmalloc(uiSize, GFP_ATOMIC);
}

void CcMalloc_free(void* pBuffer)
{
  kfree(pBuffer);
}

void CcMalloc_print(const char* pFormat, ...)
{
  va_list oArgs;
  va_start(oArgs, pFormat);
  vprintk(pFormat, oArgs);
  va_end(oArgs);
}
