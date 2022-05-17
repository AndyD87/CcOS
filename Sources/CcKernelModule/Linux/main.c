
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "CcKernelModuleContext.h"
#include "../CcKernelModule.h"
#include "CcMalloc.h"

CcKernelModuleContext* pContext;

int __init lkm_example_init(void)
{
  CcMalloc_print("CcKernelModule init\n");
  pContext = CcMalloc_malloc(sizeof(CcKernelModuleContext));
  CcKernelModule_load(pContext);
  return 0;
}

void __exit lkm_example_exit(void)
{
  CcKernelModule_unload(pContext);
  CcMalloc_free(pContext);
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);

MODULE_LICENSE    ("Dual BSD/GPL");
MODULE_AUTHOR     ("Andreas Dirmeier");
MODULE_DESCRIPTION("Basic kernel module");
MODULE_VERSION    ("0.01");
