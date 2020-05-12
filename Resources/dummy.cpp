#ifdef _MSC_VER
  // export dummy function to generate dll and lib
  void __declspec(dllexport) _CcOS_dummy(void);
#endif

void _CcOS_dummy(void){}

#include "CcStringList.h"

int main(int argc, char** argv)
{
  CcStringList oList;
  return static_cast<int>(oList.size());
}