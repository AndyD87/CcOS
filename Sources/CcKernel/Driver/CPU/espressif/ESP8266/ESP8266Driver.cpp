/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class ESP8266Driver
 */

#include "ESP8266Driver.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "CcDevice.h"
#include "ESP8266Cpu.h"
#include "ESP8266GpioPort.h"
#include "ESP8266Wlan.h"
#include "ESP8266Eeprom.h"
#include "ESP8266Spi.h"
#include "ESP8266Clk.h"
#include "ESP8266Timer.h"
#include "Devices/IWlanAccessPoint.h"
#include "Devices/IWlanClient.h"
#include "nvs_flash.h"

CCEXTERNC_BEGIN
#include <esp_event_loop.h>
#include <esp_log.h>
CCEXTERNC_END

ESP8266Driver* ESP8266Driver::s_pInstance = nullptr;

#ifdef CCOS_MAIN_REPLACED
    #undef main
    int main(int argc, char** argv);
  #else
    #error "For esp main has to be replaced by define"
#endif

CCEXTERNC void app_main()
{
  ESP8266Driver::getInstance()->setupDrivers();
  ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Start Application");
  CcString sCcOS = CcGlobalStrings::CcOS;
  char *pCcOS = sCcOS.getCharString();
  main(1, &pCcOS);
}

/**
 * @brief Event handler signaled from ESP SDK
 * @param ctx:    Context for eventhandler
 * @param event:  Event to execute
 * @return 0 on success
 */
static esp_err_t ESP8266Driver__event_handler(void *ctx, system_event_t *event)
{
  esp_err_t uErrorCode = ESP_LOG_ERROR;
  ESP8266Driver* pDriverCtx = static_cast<ESP8266Driver*>(ctx);
  if(pDriverCtx->event(event))
  {
    uErrorCode = ESP_OK;
  }
  return uErrorCode;
}

ESP8266Driver::ESP8266Driver ()
{
  s_pInstance = this;
}

ESP8266Driver::~ESP8266Driver ()
{
}

CcStatus ESP8266Driver::entry()
{
  ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Setup CPU");
  CCNEW(m_pCpu, ESP8266Cpu);
  CcKernel::addDevice(CcDevice(m_pCpu, EDeviceType::Cpu));
  return true;
}

CcStatus ESP8266Driver::unload()
{
  CCDELETE(m_pWlan);
  CCDELETE(m_pGpio);
  CCDELETE(m_pCpu);
  CCDELETE(m_pEeprom);
  CCDELETE(m_pSpi);
  // System device should never fail, we would have big problems
  return true;
}

void ESP8266Driver::setupDrivers()
{
  nvs_flash_init();
  ESP_ERROR_CHECK(esp_event_loop_init(ESP8266Driver__event_handler, this));
  ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Setup GPIO");
  CCNEW(m_pGpio, ESP8266GpioPort);
  CcKernel::addDevice(CcDevice(m_pGpio, EDeviceType::GpioPort));
  ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Setup Network");

  CCNEW(m_pWlan, ESP8266Wlan);
  CcKernel::addDevice(CcDevice(m_pWlan, EDeviceType::Wlan));
  CcKernel::addDevice(CcDevice(m_pWlan->getAccessPoint(), EDeviceType::WlanAccessPoint));
  CcKernel::addDevice(CcDevice(m_pWlan->getClient(), EDeviceType::WlanClient));

  CCNEW(m_pEeprom, ESP8266Eeprom, m_pCpu);
  CcKernel::addDevice(CcDevice(m_pEeprom, EDeviceType::Eeprom));

  CCNEW(m_pSpi, ESP8266Spi, this);
  CcKernel::addDevice(CcDevice(m_pSpi, EDeviceType::Spi));

  CCNEW(m_pClk, ESP8266Clk);
  CcKernel::addDevice(CcDevice(m_pClk, EDeviceType::Clock));

  CCNEW(m_pTimer, ESP8266Timer);
  CcKernel::addDevice(CcDevice(m_pTimer, EDeviceType::Timer));
}

bool ESP8266Driver::event(void* event)
{
  system_event_t* pEvent = static_cast<system_event_t*>(event);
  CCUNUSED(pEvent);
  bool bHandled = false;
  if(m_pWlan && m_pWlan->event(event))
  {
    bHandled = true;
  }
  else
  {
    CCDEBUG("ESP8266Driver::event Unhandled event: " + CcString::fromNumber(pEvent->event_id));
  }
  return bHandled;
}
