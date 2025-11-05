#include "led.h"

void LED_init() {
    PORTA.DIRSET = PIN2_bm;
}
void set_LED_on(){
    // LED is active low. Set pin LOW to turn LED on
    PORTA.OUTCLR = PIN2_bm;
}
void set_LED_off(){
    // LED is active low. Set pin HIGH to turn LED off
    PORTA.OUTSET = PIN2_bm;
}