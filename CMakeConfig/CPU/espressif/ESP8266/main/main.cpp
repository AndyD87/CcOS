#include "CcBase.h"
#include "CcStatic.h"
#include "CcKernel.h"
#include "IThread.h"
#include "Devices/IEeprom.h"

#ifdef CCOS_MAIN_REPLACED
  #undef main
#endif

CCEXTERNC int main(int argc, char** argv)
{
  CCUNUSED(argc);
  CCUNUSED(argv);
  CcKernel::initCLI();

  CcHandle<IEeprom> pEeprom = CcKernel::getDevice(EDeviceType::Eeprom).cast<IEeprom>();
  if(pEeprom.isValid())
  {
    char pData[12] = {0};
    if(pEeprom->read(pData, 10))
    {
      CCDEBUG("EEPROM read done");
      CcString sData(pData);
      CCDEBUG("  " + sData);
    }
  }
  else
  {
    CCDEBUG("EEPROM not found");
  }
  CCDEBUG("Here we are");
  return -1;
}
