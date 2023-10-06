#include "app.h"

#include "app_config.h"

#include "em_emu.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

#include "stdio.h"

/**
 * Called once before main infinite loop.
*/
void setup() {
    // Toggle LED config
    CMU_ClockEnable(cmuClock_GPIO, 1);

    GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);

    // Timer interrupt config
    CMU_ClockEnable(cmuClock_TIMER0, 1);

    TIMER_Init_TypeDef timer0_init_config = TIMER_INIT_DEFAULT;
    timer0_init_config.enable = 0;
    timer0_init_config.prescale = timerPrescale1024;

    TIMER_Init(TIMER0, &timer0_init_config);
    TIMER_TopSet(TIMER0, 37109);

    TIMER_IntEnable(TIMER0, TIMER_IF_OF);
    NVIC_EnableIRQ(TIMER0_IRQn);

    TIMER_Enable(TIMER0, 1);

    // Printf test
    printf("[*] Cookie ready!\n");
}

/**
 * Main infinite loop.
*/
void loop() {

}
