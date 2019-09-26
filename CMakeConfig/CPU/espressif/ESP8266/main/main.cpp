#include "CcBase.h"
#include "CcStatic.h"
#include "CcKernel.h"
#include "IThread.h"
#include "Devices/IGpioPort.h"

#ifdef CCOS_MAIN_REPLACED
  #undef main
#endif

CCEXTERNC int main(int argc, char** argv)
{
  CCUNUSED(argc);
  CCUNUSED(argv);
  CcKernel::initCLI();
  return -1;
}
