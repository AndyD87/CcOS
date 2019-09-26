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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class ESP8266Driver
 */

#include "ESP8266Driver.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "ESP8266Cpu.h"
#include "ESP8266GpioPort.h"
#include "ESP8266Wlan.h"
#include "Devices/IWlanAccessPoint.h"
#include "Devices/IWlanClient.h"
#include "nvs_flash.h"

CCEXTERNC_BEGIN
#include <esp_event_loop.h>
#include <esp_log.h>
//#include <esp_system.h>
//#include <sys/param.h>
//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
//#include <freertos/queue.h>
CCEXTERNC_END

ESP8266Driver* ESP8266Driver::s_pInstance = nullptr;

#ifdef CCOS_MAIN_REPLACED
  #undef main
  int main(int argc, char** argv);
#endif

CCEXTERNC void app_main()
{
  ESP8266Driver::getInstance()->setupDrivers();
  ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Start Application");
  CcString sCcOS = CcGlobalStrings::CcOS;
  char *pCcOS = sCcOS.getCharString();
  main(1, &pCcOS);
}

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
  CcKernel::addDevice(CcDeviceHandle(m_pCpu, EDeviceType::Cpu));
  return true;
}

CcStatus ESP8266Driver::unload()
{
  CCDELETE(m_pWlan);
  CCDELETE(m_pGpio);
  CCDELETE(m_pCpu);
  // System device should never fail, we would have big problems
  return true;
}

void ESP8266Driver::setupDrivers()
{
  nvs_flash_init();
  ESP_ERROR_CHECK(esp_event_loop_init(ESP8266Driver__event_handler, this));
  ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Setup GPIO");
  CCNEW(m_pGpio, ESP8266GpioPort);
  CcKernel::addDevice(CcDeviceHandle(m_pGpio, EDeviceType::GpioPort));
  ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Setup Network");

  CCNEW(m_pWlan, ESP8266Wlan);
  CcKernel::addDevice(CcDeviceHandle(m_pWlan, EDeviceType::Wlan));
  CcKernel::addDevice(CcDeviceHandle(m_pWlan->getAccessPoint(), EDeviceType::WlanAccessPoint));
  CcKernel::addDevice(CcDeviceHandle(m_pWlan->getClient(), EDeviceType::WlanClient));
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
