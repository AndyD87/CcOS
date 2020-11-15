#include "CcBase.h"

#ifdef __cplusplus
class CcKernelModule
{
public:
  static void testLoad();
  static void testUnload();
};
#endif

CCEXTERNC_BEGIN
extern void CcKernelModule_load(void);
extern void CcKernelModule_unload(void);
CCEXTERNC_END
