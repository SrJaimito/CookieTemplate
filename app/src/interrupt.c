/**
 * This file can be used to write your interruption
 * request handlers and keep them separated from
 * your main application code.
*/

#include "app_config.h"

#include "em_timer.h"
#include "em_gpio.h"

void TIMER0_IRQHandler() {
    if (TIMER_IntGet(TIMER0) & TIMER_IF_OF) {
        TIMER_IntClear(TIMER0, TIMER_IF_OF);

        GPIO_PinOutToggle(LED_PORT, LED_PIN);
    }
}
