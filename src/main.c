/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define CONFIG_ESP_LYRAT_V4_3_BOARD 1

#include <mjs.h>
// #include <arm.h>
#include <strings.h>
#include <amr.h>

#ifndef portTICK_PERIOD_MS
  #define portTICK_PERIOD_MS (( TickType_t) 1000 / configTICK_RATE_HZ)
#endif

void foo(int x) {
  printf("Hello %d!\n", x);
}

void *my_dlsym(void *handle, const char *name) {
  if (strcasecmp(name, "foo") == 0) return foo;
  return NULL;
}

void loop(struct amr_config *config)
{
  while (!amr_process(config))
  {
  }
}

void app_main()
{
    printf("Hello world!\n");

    struct amr_config config = amr_setup();

    struct mjs *mjs = mjs_create();
    mjs_set_ffi_resolver(mjs, my_dlsym);
    mjs_exec(mjs, "let f = ffi('void foo(int)'); f(1234)", NULL);

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    loop(&config);

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    amr_cleaning_up(&config);

    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

