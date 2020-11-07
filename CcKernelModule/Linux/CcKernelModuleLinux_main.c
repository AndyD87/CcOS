#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "../CcKernelModule.h"

static int __init lkm_example_init(void)
{
  CcKernelModule_load();
  return 0;
}

static void __exit lkm_example_exit(void)
{
  CcKernelModule_unload();
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);

MODULE_LICENSE    ("Dual BSD/GPL");
MODULE_AUTHOR     ("Andreas Dirmeier");
MODULE_DESCRIPTION("Basic kernel module");
MODULE_VERSION    ("0.01");
