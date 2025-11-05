/**
 * @file main.c
 * @author joshjude
 * @date 2025-10-27
 * @brief Main function
 */

#include "usart.h"
#include "ac.h"
#include "polling.h"
#include <avr/io.h>
#include "led.h"


// Task 1: Busy waiting
void busy_waiting(){

    VREF_init();
    AC_init();
    LED_init();
    
    while(1) {
        if (AC_above_threshold()) {
            set_LED_off();
        } else {
            set_LED_on();
        }
    }
}

// Timer overflow ISR - triggers every 10ms
ISR(TCA0_OVF_vect)
{
    if(AC_above_threshold()){
        set_LED_off();
    } else {
        set_LED_on();
    }
    
    // Clear interrupt flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main(){

    // busy_waiting();
    // Uncomment the line below to use polling instead:
    polling();
    return 0;

}
