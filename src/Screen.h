#ifndef Screen
#define Screen

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>
#include "DHT22.h"

void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data );
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );
void my_print(const char * buf);
void PotTimerCallback(lv_timer_t * timer);
void DHTReadCallback(lv_timer_t * timer);

extern lv_disp_draw_buf_t draw_buf;
extern lv_color_t buf[];
extern const uint16_t screenWidth;
extern const uint16_t screenHeight;

extern TFT_eSPI tft;
extern float lastPotRead;
extern float potRead;
extern int POT_PIN;

extern DHT22 dht;

#endif