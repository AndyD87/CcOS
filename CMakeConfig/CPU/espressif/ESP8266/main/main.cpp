#include "CcBase.h"
#include "CcStatic.h"
#include "CcRemoteDeviceServer.h"
#include "CcKernel.h"
#include "IThread.h"
#include "Devices/IGpioPort.h"

#ifdef CCOS_MAIN_REPLACED
  #undef main
#endif

CCEXTERNC int main(int argc, char** argv)
{
  //CcRemoteDeviceServer oServer;
  return oServer.exec().getErrorInt();
}
