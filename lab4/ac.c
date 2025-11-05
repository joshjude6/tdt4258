#include "ac.h"
#include <avr/io.h>

// needs to be called in main
void VREF_init() {
    VREF.ACREF = VREF_REFSEL_1V024_gc;
}

void AC_init() {
    // Configure pin 2 of PORT D as an output pin. See chapter 18.3.1 of the datasheet
    PORTD.DIRCLR = PIN2_bm;
    // Disable digital input buffer and pull-up resistor for pin PD2
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    // Set the positive input source to AINP0 and the negative input source to the DAC reference (DACREF)
    AC0.MUXCTRL = AC_MUXPOS_AINP0_gc | AC_MUXNEG_DACREF_gc;
    // Set DACREF threshold: VDACREF = (DACREF/256) * VREF = (25/256) * 1.024V = 0.1V
    AC0.DACREF = 25;
    // Enable the Analog Comparator by writing AC_ENABLE_bm to the CTRLA register
    AC0.CTRLA = AC_ENABLE_bm | AC_RUNSTDBY_bm; // allows comparator to run in sleep mode

}

bool AC_above_threshold() {
    if(AC0.STATUS & AC_CMPSTATE_bm) {
        return true; // Signal is above threshold
    }
    return false; // Signal is below threshold
}

