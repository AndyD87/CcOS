/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "CcBase.h"
#include "CcStatic.h"
#include "CcRemoteDeviceServer.h"

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
CCEXTERNC_END

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

static const char *TAG="APP";

#define GPIO_OUTPUT_IO_0    15
#define GPIO_OUTPUT_IO_1    16
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))

static xQueueHandle gpio_evt_queue = NULL;

static void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{
    uint32_t io_num;

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            ESP_LOGI(TAG, "GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(static_cast<gpio_num_t>(io_num)));
        }
    }
}

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

CCEXTERNC void app_main()
{
  // static httpd_handle_t server = NULL;
  // ESP_ERROR_CHECK(nvs_flash_init());
  // initialise_wifi(&server);

  gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  //bit mask of the pins that you want to set,e.g.GPIO15/16
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  //disable pull-down mode
  io_conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
  //disable pull-up mode
  io_conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;
  //configure GPIO with the given settings
  gpio_config(&io_conf);

  //interrupt of rising edge
  io_conf.intr_type = GPIO_INTR_POSEDGE;
  //bit mask of the pins, use GPIO4/5 here
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  //set as input mode
  io_conf.mode = GPIO_MODE_INPUT;
  //enable pull-up mode
  io_conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);

  //change gpio intrrupt type for one pin
  gpio_set_intr_type(static_cast<gpio_num_t>(GPIO_INPUT_IO_0), GPIO_INTR_ANYEDGE);

  //create a queue to handle gpio event from isr
  gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
  //start gpio task
  xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

  //install gpio isr service
  gpio_install_isr_service(0);
  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(static_cast<gpio_num_t>(GPIO_INPUT_IO_0), gpio_isr_handler, (void *) GPIO_INPUT_IO_0);
  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(static_cast<gpio_num_t>(GPIO_INPUT_IO_1), gpio_isr_handler, (void *) GPIO_INPUT_IO_1);

  //remove isr handler for gpio number.
  gpio_isr_handler_remove(static_cast<gpio_num_t>(GPIO_INPUT_IO_0));
  //hook isr handler for specific gpio pin again
  gpio_isr_handler_add(static_cast<gpio_num_t>(GPIO_INPUT_IO_0), gpio_isr_handler, (void *) GPIO_INPUT_IO_0);

  int cnt = 0;

  while (1) {
      ESP_LOGI(TAG, "cnt: %d\n", cnt++);
      vTaskDelay(1000 / portTICK_RATE_MS);
      gpio_set_level(static_cast<gpio_num_t>(GPIO_OUTPUT_IO_0), cnt % 2);
      gpio_set_level(static_cast<gpio_num_t>(GPIO_OUTPUT_IO_1), cnt % 2);
  }

  CcRemoteDeviceServer oServer;
  oServer.exec();
}
