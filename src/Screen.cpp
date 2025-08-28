#include "Screen.h"
#include "DHT22.h"

/* --- Potentiometer variables --- */
float potRead;
float lastPotRead;
int POT_PIN = 14;


/* --- LVGL Display Buffers ---*/
 const uint16_t screenWidth  = 480;
 const uint16_t screenHeight = 320;
 lv_disp_draw_buf_t draw_buf;
 lv_color_t buf[ screenWidth * screenHeight / 10 ];


/* --- TFT instance --- */
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif


/**
 * Display flush callback for LVGL
 * Pushes a block of pixels from LVGL to TFT
 */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}


/**
 * Touchpad read callback for LVGL (currently dummy)
 * @param indev_driver: pointer to input device driver
 * @param data: pointer to store touch data
 */
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX = 0, touchY = 0;

    bool touched = false;//tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print( "Data x " );
        Serial.println( touchX );

        Serial.print( "Data y " );
        Serial.println( touchY );
    }
}


/**
 * Potentiometer timer callback
 * Reads analog value and updates LVGL slider and ring
 */
void PotTimerCallback(lv_timer_t * timer)
{
    potRead = analogRead(POT_PIN)* 10000.0 / 4095.0 ;
    if (lastPotRead != potRead) {
        lastPotRead = potRead;
        lv_slider_set_value(ui_OhmBar, potRead, LV_ANIM_ON);
        lv_arc_set_value(ui_PotValRing, potRead);
        char buf[16];
        snprintf(buf, sizeof(buf), "%.1f Ohms", potRead);
        lv_label_set_text(ui_PotVal, buf);
    }
}


/**
 * DHT22 timer callback
 * Reads DHT22 and updates humidity arc
 */
void DHTReadCallback(lv_timer_t * timer){
    dht.readDHT22();
    float temp = dht.getTemperature();
    float humidity = dht.getHumidity();
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("  Humidity: ");
    Serial.println(humidity);
    // lv_label_set_text(ui_TempVal, String(temp).c_str());
    // lv_arc_set_value(ui_Arc1, humidity);
}