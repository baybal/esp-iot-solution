/* Touch Sensor Example

   For other examples please check:
   https://github.com/espressif/esp-iot-solution/tree/master/examples

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "iot_touchpad.h"
#include "iot_led.h"
#include "evb.h"

static const char *TAG = "touch_slide";
#if CONFIG_TOUCH_EB_V1
#define TOUCH_SLIDE_0 TOUCH_PAD_NUM4
#define TOUCH_SLIDE_1 TOUCH_PAD_NUM5
#define TOUCH_SLIDE_2 TOUCH_PAD_NUM6
#define TOUCH_SLIDE_3 TOUCH_PAD_NUM7
#define TOUCH_SLIDE_4 TOUCH_PAD_NUM8
#define TOUCH_SLIDE_5 TOUCH_PAD_NUM9
#define TOUCH_SLIDE_LED_NUM_0     4
#define TOUCH_SLIDE_LED_NUM_1     5
#elif CONFIG_TOUCH_EB_V2
#define TOUCH_SLIDE_0 TOUCH_PAD_NUM9
#define TOUCH_SLIDE_1 TOUCH_PAD_NUM8
#define TOUCH_SLIDE_2 TOUCH_PAD_NUM7
#define TOUCH_SLIDE_3 TOUCH_PAD_NUM6
#define TOUCH_SLIDE_4 TOUCH_PAD_NUM5
#define TOUCH_SLIDE_5 TOUCH_PAD_NUM4
#define TOUCH_SLIDE_LED_NUM_0     3
#define TOUCH_SLIDE_LED_NUM_1     2
#endif

void evb_touch_slide_handle(int pos)
{
    static int pos_prev = 0;
    if (pos_prev != pos && pos != 0xff) {
        pos_prev = pos;
        ESP_LOGI(TAG, "slide pos evt[%d]", pos);
        ch450_write_dig(0, -1);
        ch450_write_dig(1, -1);
        ch450_write_dig(2, -1);
        ch450_write_dig(3, -1);
        ch450_write_dig(TOUCH_SLIDE_LED_NUM_0, pos % 10);
        ch450_write_dig(TOUCH_SLIDE_LED_NUM_1, pos / 10);
    }
}

void evb_touch_slide_init_then_run()
{
#if CONFIG_TOUCH_EB_V2
    evb_touch_button_init();
#endif
    //slide touch
    const touch_pad_t tps[] = { TOUCH_SLIDE_0, TOUCH_SLIDE_1, TOUCH_SLIDE_2, TOUCH_SLIDE_3, TOUCH_SLIDE_4,
            TOUCH_SLIDE_5 };
    CTouchPadSlide *tp_slide = new CTouchPadSlide(sizeof(tps) / sizeof(TOUCH_PAD_NUM4), tps, 5, TOUCHPAD_THRES_PERCENT,
            NULL);
    while (1) {
        uint8_t pos = tp_slide->get_position();
        evb_touch_slide_handle(pos);
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}
