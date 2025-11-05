#ifndef POLLING_H
#define POLLING_H

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void TCA0_init();
void sleep_init();
void polling();



#endif // POLLING_H