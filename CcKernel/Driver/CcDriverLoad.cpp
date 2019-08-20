/* Generated File */
#include "Driver/CcDriverLoad.h"
#include "Driver/CPU/espressif/ESP8266/ESP8266Driver.h"
int CcDriverLoad::s_iState(-1);
void CcDriverLoad::load0()
{
  // Load Boot Level 0
  IDriver* pESP8266Driver = new ESP8266Driver();
  pESP8266Driver->entry();
  m_DriverList.append(pESP8266Driver);
}
void CcDriverLoad::load1()
{
  // Load Boot Level 1
}
void CcDriverLoad::load2()
{
  // Load Boot Level 2
}
void CcDriverLoad::load3()
{
  // Load Boot Level 3
}
