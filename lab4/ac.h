#include <avr/io.h>
#include <stdbool.h>
#include "usart.h"

void AC_init();
void VREF_init();
bool AC_above_threshold();