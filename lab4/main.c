/**
 * @file main.c
 * @author joshjude
 * @date 2025-10-27
 * @brief Main function
 */

#include "usart.h"

int main(){

    // Add your code here and press Ctrl + Shift + B to build
    USART3_Init();
    USART3_SendChar('A');
    return 0;
}
