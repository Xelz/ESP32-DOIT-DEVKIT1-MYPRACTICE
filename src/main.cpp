#include "Screen.h"
#include "DHT22.h"

#define BAUD_RATE 115200
#define POT_PIN 14
#define DHT_PIN_1 16

/* --- DHT22 sensor instance --- */
DHT22 dht(DHT_PIN_1);

void setup()
{
    Serial.begin( BAUD_RATE ); //serial debug 

    /* --- LVGL Init ---*/
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );
    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    /* --- TFT Init ---*/
    tft.begin();          /* TFT init */
    tft.setRotation( 3 ); /* Landscape orientation, flipped */
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

    /* --- Configure the display driver ---*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    /* --- Initialize the UI --- */
    ui_init();

    /* --- Configure UI elements --- */
    lv_slider_set_range(ui_OhmBar, 0, 10000);
    lv_arc_set_range(ui_PotValRing, 0, 10000);
    potRead = analogRead(POT_PIN);
    Serial.println("Potentiometer Initializing");
    Serial.println( "Setup done" );
    delay(2000);

    /* --- Setup periodic timers callback --- */
    lv_timer_create(PotTimerCallback, 16, NULL);
    lv_timer_create(DHTReadCallback, 2000, NULL);
}

void loop()
{
    // potRead = analogRead(POT_PIN) / 4095.0 * 100.0;
    // lastPotRead = potRead;
    // Serial.print("Serial Start ");
    // Serial.print("Baud Rate: ");
    // Serial.println(BAUD_RATE);
    // Serial.println("***********************************************");
    // Serial.print("Potentiometer Reading: ");
    // Serial.println(potRead);

    lv_timer_handler(); /* let the GUI do its work */
}


