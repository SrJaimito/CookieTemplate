#ifndef __COOKIE_RETARGET_H__
#define __COOKIE_RETARGET_H__

#include "em_gpio.h"

#define PRINTF_UART_PORT gpioPortA
#define PRINTF_UART_PIN  0

/**
 * Call this function to initialize the retargeting
 * of the stdio functions to the UART.
*/
void retarget_init();

#endif
