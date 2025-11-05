#include "ac.h"
#include <avr/io.h>

// needs to be called in main
void VREF_init() {
    VREF.ACREF = VREF_REFSEL_1V024_gc;
}

void AC_init() {

    // Configure pin 2 of PORT D as input pin. See chapter 18.3.1 of the datasheet
    PORTD.DIRCLR = PIN2_bm;
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Disable digital input buffer and pull-up resistor for all pins using PINCONFIG
    PORTB.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTE.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTB.PINCTRLUPD = 0xFF; 
    PORTC.PINCTRLUPD = 0xFF;  
    PORTE.PINCTRLUPD = 0xFF;   

    // Set the positive input source to AINP0 and the negative input source to the DAC reference (DACREF)
    AC0.MUXCTRL = AC_MUXPOS_AINP0_gc | AC_MUXNEG_DACREF_gc;

    // Set DACREF threshold: VDACREF = (DACREF/256) * VREF = (25/256) * 1.024V = 0.1V
    AC0.DACREF = 25;

    // Enable interrupt on comparator output (triggers on both edges - above/below threshold)
    AC0.INTCTRL = AC_CMP_bm;

    // Enable the Analog Comparator and set to run in standby mode
    AC0.CTRLA = AC_ENABLE_bm | AC_RUNSTDBY_bm;

}

bool AC_above_threshold() {
    if(AC0.STATUS & AC_CMPSTATE_bm) {
        return true; // Signal is above threshold
    }
    return false; // Signal is below threshold
}

