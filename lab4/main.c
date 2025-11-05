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
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdbool.h>
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

// ISR for polling task
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

// ISR for interrupt task
ISR(AC0_AC_vect)
{
    // Check if signal is above or below threshold and control LED
    if(AC_above_threshold()){
        set_LED_off();  // Bright light - LED off
    } else {
        set_LED_on();   // Dark - LED on
    }
    
    // Clear interrupt flag
    AC0.STATUS = AC_CMPIF_bm;
}

// Task 4: Interrupt-driven
void interrupt_driven() {
   
    VREF_init();
    AC_init();  
    LED_init();
    
    // setting sleep mode to standby
    set_sleep_mode(SLEEP_MODE_STANDBY);
    
    // enabling interrupts
    sei();
    
    while(1) {
        sleep_mode();  // sleep until interrupt
    }
}

int main(){

    // busy_waiting();
    // polling();
    interrupt_driven();  
    return 0;

}
