/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "CcBase.h"

CCEXTERNC_BEGIN
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <esp_http_server.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <driver/gpio.h>
#include "driver/uart.h"
CCEXTERNC_END

#include "CcStatic.h"
#include "CcRemoteDeviceServer.h"
#include "CcKernel.h"
#include "IThread.h"
#include "Devices/IGpioPort.h"

static const char *TAG="APP";

class CTestThread : public IThread
{
public:
  CTestThread() :
    IThread("CTestThread")
  {}

  virtual void run() override
  {
    IGpioPort* pPort = CcKernel::getDevice(EDeviceType::GpioPort).cast<IGpioPort>().ptr();
    if(pPort)
    {
      pPort->setDirection(2, IGpioPin::EDirection::Output);
      while(pPort && getThreadState() == EThreadState::Running)
      {
        pPort->setValue(2, !pPort->getValue(2));
        CcKernel::sleep(200);
      }
    }
  }
};

#

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 * The examples use simple WiFi configuration that you can set via
 * 'make menuconfig'.
 * If you'd rather not, just change the below entries to strings
 * with the config you want -
 * ie. #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    /* For accessing reason codes in case of disconnection */
    system_event_info_t *info = &event->event_info;

    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        ESP_LOGI(TAG, "Got IP: '%s'",
                ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
        ESP_LOGE(TAG, "Disconnect reason : %d", info->disconnected.reason);
        if (info->disconnected.reason == WIFI_REASON_BASIC_RATE_NOT_SUPPORT) {
            /*Switch to 802.11 bgn mode */
            esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCAL_11B | WIFI_PROTOCAL_11G | WIFI_PROTOCAL_11N);
        }
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    default:
        break;
    }
    return ESP_OK;
}

void initialise_wifi(void *arg)
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, arg));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config;
    CcStatic_memsetZeroObject(wifi_config);
    CcStatic::memcpy(wifi_config.sta.ssid, EXAMPLE_WIFI_SSID, sizeof(EXAMPLE_WIFI_SSID));
    CcStatic::memcpy(wifi_config.sta.password, EXAMPLE_WIFI_PASS, sizeof(EXAMPLE_WIFI_PASS));
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void startRemoteDevice()
{
  CcRemoteDeviceServer* pServer = new CcRemoteDeviceServer();
  pServer->exec();
}

CCEXTERNC void app_main()
{
  // static httpd_handle_t server = NULL;
  // initialise_wifi(&server);

  ESP_LOGI(TAG, "Create thread object");
  CTestThread *pThread = new CTestThread();
  pThread->start();

  for (int i = 0; i < 10; i++)
  {
    ESP_LOGI(TAG, "delay left: %ds", 10 - i);
    vTaskDelay(1000 / portTICK_RATE_MS);
  }

  ESP_LOGI(TAG, "Stop thread object");
  delete pThread;

  ESP_LOGI(TAG, "Create remote device");
  startRemoteDevice();

}
