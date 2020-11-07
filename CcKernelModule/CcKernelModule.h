

#ifdef __cplusplus
class CcKernelModule
{
public:
  static void testLoad();
  static void testUnload();
};
#endif

#ifdef __cplusplus
  extern "C" {
#endif

extern void CcKernelModule_load(void);
extern void CcKernelModule_unload(void);

#ifdef __cplusplus
  }
#endif
