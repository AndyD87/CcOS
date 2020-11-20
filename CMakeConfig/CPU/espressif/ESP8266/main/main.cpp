#include "CcBase.h"
#include "CcStatic.h"
#include "CcKernel.h"
#include "IThread.h"
#include "Devices/IEeprom.h"
#include "Devices/CcDeviceEeprom.h"

#ifdef CCOS_MAIN_REPLACED
  #undef main
#endif

CCEXTERNC int main(int argc, char** argv)
{
  CCUNUSED(argc);
  CCUNUSED(argv);
  CcKernel::initCLI();

  CcDeviceEeprom oEeprom = CcKernel::getDevice(EDeviceType::Eeprom);
  if(oEeprom.isValid())
  {
    char pData[12] = {0};
    if(oEeprom.read(pData, 10))
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
