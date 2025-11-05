#include "polling.h"
#include "ac.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "led.h"


void TCA0_init() {
    cli();
    // Set the period of the timer.
    // PER = period[s] * F_CPU / Prescaler = 0.01s * 4 000 000 Hz / 2
    TCA0.SINGLE.PER = 20000;
    // Enable timer overflow interrupt
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    // Run timer in standby mode, set prescaler to 2, enable timer
    TCA0.SINGLE.CTRLA = TCA_SINGLE_RUNSTDBY_bm | TCA_SINGLE_CLKSEL_DIV2_gc | TCA_SINGLE_ENABLE_bm;
    sei();
}

// Function to initialize sleep mode
void sleep_init(void) {
    set_sleep_mode(SLEEP_MODE_STANDBY);  // Standby sleep mode
}


// Task 4: Polling
void polling() {
    // Initialize peripherals
    VREF_init();
    AC_init();
    LED_init(); 
    TCA0_init();  
    sleep_init();
    
    while(1) {
        // Put device to sleep until timer interrupt
        sleep_mode();
    }
}
