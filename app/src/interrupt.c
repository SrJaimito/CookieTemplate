/**
 * This file can be used to write your interruption
 * request handlers and keep them separated from
 * your main application code.
*/

#include "app_config.h"

#include "em_device.h"

void TIMER0_IRQHandler() {
    if (TIMER0->IF & TIMER_IF_OF) {
        TIMER0->IFC = TIMER_IFC_OF;

        GPIO->P[LED_PORT].DOUTTGL = 1 << LED_PIN;
    }
}
